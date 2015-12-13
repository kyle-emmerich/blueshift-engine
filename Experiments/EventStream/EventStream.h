#pragma once
#include <string>
#include <inttypes.h>
#include <map>
#include <typeindex>
#include <memory>
#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>

template<typename T>
class EventStream {
protected:
	T* buffer;
	T* buffer_end;
	std::atomic<T*> head;
	std::atomic<T*> tail;
	std::atomic<T*> frame_head;
	std::atomic<T*> frame_tail;
	size_t count;
	std::atomic<size_t> dropped;
	size_t capacity;

	std::mutex mutex;
	bool writing;
	bool reading;
	std::condition_variable write_done;
	std::condition_variable read_done;
	std::unique_lock<std::mutex> write_lock;
	std::unique_lock<std::mutex> read_lock;
public:
	inline EventStream(size_t BufferSize)
		: mutex(), write_lock(mutex, std::defer_lock), read_lock(mutex, std::defer_lock) {
		buffer = new T[BufferSize];
		buffer_end = buffer + BufferSize;
		head = buffer;
		tail = buffer;
		count = 0;
		capacity = BufferSize;
	}
	inline ~EventStream() {
		delete[] buffer;
	}

	inline void Initialize() {
		writing = true;
		reading = false;
		read_done.notify_one();
	}

	inline void BeginWrite() {
		//Begin critical section
		write_lock.lock();
		while (reading) {
			read_done.wait(write_lock);
		}
		writing = true;
		frame_tail = head.load();
		tail = frame_tail.load();
		count = 0;
		dropped = 0;
	}
	inline bool TryWrite() {
		if (write_lock.try_lock()) {
			while (reading) {
				read_done.wait(write_lock);
			}
			writing = true;
			frame_tail = head.load();
			tail = frame_tail.load();
			count = 0;
			return true;
		}
		return false;
	}
	inline void EndWrite() {
		writing = false;
		reading = true;
		frame_tail = frame_head.load();
		frame_head = head.load();
		write_lock.unlock();
		write_done.notify_one();
		//End critical section
	}

	inline T* PushEvent() {
		count++;
		if (count > capacity) {
			dropped++;
			count = capacity;
		}

		T* value = head++;
		T* head_ptr = head.load();
		if (head_ptr == buffer_end) {
			head = buffer;
		}
		return value;
	}
	
	inline void BeginRead() {
		//Begin critical section
		read_lock.lock();
		while (writing) {
			write_done.wait(read_lock);
		}
		reading = true;
	}
	inline bool TryRead() {
		if (read_lock.try_lock()) {
			while (writing) {
				write_done.wait(read_lock);
			}
			reading = true;
			return true;
		}
		return false;
	}

	inline void EndRead() {
		reading = false;
		writing = true;
		read_lock.unlock();
		read_done.notify_one();
		//End critical section
	}

	inline T* PopEvent() {
		if (count == 0) {
			return nullptr;
		}
		count--;

		T* value = tail++;
		T* tail_ptr = tail.load();
		if (tail_ptr == buffer_end) {
			tail = buffer;
		}
		return value;
	}

	inline size_t GetNumberDropped() {
		return dropped;
	}
};