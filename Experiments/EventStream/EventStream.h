#pragma once
#include <inttypes.h>
#include <memory>
#include <iostream>
#include <mutex>
#include <atomic>


template<typename T>
class EventQueue {
protected:
	T* buffer;
	T* buffer_end;
	T* read_ptr;
	T* write_ptr;
	bool locked;
public:
	inline EventQueue(size_t BufferSize) {
		buffer = new T[BufferSize];
		buffer_end = buffer + BufferSize;
		
		write_ptr = buffer;
		read_ptr = buffer;

		type_size = sizeof(T);
	}
	inline ~EventQueue() {
		delete[] buffer;
	}

	T* Lock() {
		if (locked) {
			throw 0;
		}
		locked = true;

		T* locked_section = write_ptr;

		++write_ptr;
		if (write_ptr == buffer_end) {
			write_ptr = buffer;
		}
		
		return locked_section;
	}
	void Unlock() {
		if (!locked) {
			throw 1;
		}
		locked = false;
	}

	T* Read() {
		if (read_ptr == write_ptr) {
			return nullptr;
		}

		T* data = read_ptr;

		++read_ptr;
		if (read_ptr == buffer_end) {
			read_ptr = buffer;
		}

		return data;
	}
};