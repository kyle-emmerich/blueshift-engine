#pragma once
#include <string>
#include <inttypes.h>
#include <map>
#include <typeindex>
#include <memory>
#include <iostream>
#include <mutex>

template<typename T>
class EventStream {
protected:
	T* buffer;
	T* buffer_end;
	T* head;
	T* tail;
	T* frame_head;
	T* frame_tail;
	size_t count;
	size_t lost;
	size_t capacity;

//	std::mutex read_mutex;
//	std::mutex write_mutex;
public:
	inline EventStream(size_t BufferSize) {
		buffer = new T[BufferSize];
		buffer_end = buffer + BufferSize;
		head = buffer;
		tail = buffer;
		count = 0;
		lost = 0;
		capacity = BufferSize;
	}
	inline ~EventStream() {
		delete[] buffer;
	}

	inline void BeginWrite() {
		frame_tail = head;
		tail = frame_tail;
		count = 0;
	}
	inline void EndWrite() {
		frame_head = head;
		//something else maybe?
	}

	inline T* PushEvent() {
		//If we go past the end of the buffer, reset to the beginning
		if (head == buffer_end) {
			head = buffer;
		}
		
		count++;
#ifdef _DEBUG
		if (count > capacity) {
			//whoa there, we're losing events. size up that buffer!
			//throw 0;
			lost++;
		}
#endif

		return head++;
	}
	

	inline T* PopEvent() {
		if (tail == frame_head) {
			//Nothing to get.
			return nullptr;
		}
		if (tail == buffer_end) {
			tail = buffer;
		}
		

		return tail++;
	}

	inline size_t GetNumberLost() {
		return lost;
	}
};