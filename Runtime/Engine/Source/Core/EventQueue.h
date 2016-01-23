#pragma once
#include <vector>
#include <functional>

using std::bind;
using std::function;
using namespace std::placeholders;

namespace Blueshift {
	namespace Core {

		/*!	\class EventQueue
			\brief Holds a statically sized queue of event messages in a ring buffer format.

			\todo Explore threadsafe options
		*/
		template<typename T>
		class EventQueue {
		protected:
			T* buffer;
			T* buffer_end;
			T* read_ptr;
			T* write_ptr;

			size_t buffer_size;

			//std::vector<std::function<T*>> listeners;

			std::mutex mtx;
		public:
			/*!	\brief Initialize a queue with a set capacity for elements of type T.

				\param BufferSize The size of the buffer in number of elements.
			*/
			inline EventQueue(size_t BufferSize = 1024) {
				buffer = new T[BufferSize];
				buffer_end = buffer + BufferSize;
				buffer_size = BufferSize;
				write_ptr = buffer;
				read_ptr = buffer;
			}
			inline ~EventQueue() {
				delete[] buffer;
			}

			/*!	\brief Grabs the next slot in the queue for writing.

				\returns A pointer to the next slot in the queue, wrapping to the beginning if out of space.
			*/
			T* Request() {
				mtx.lock();
				T* next = write_ptr;

				++write_ptr;
				if (write_ptr == buffer_end) {
					write_ptr = buffer;
				}
				mtx.unlock();
				return next;
			}

			/*!	\brief Grabs the next valid element in the queue, or returns a nullptr if the queue is empty.

				\returns A pointer to the next readable slot, or nullptr
			*/
			T* Pop() {
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

			/*!	\brief Grabs the next valid element in the queue, but leaves it there; returns nullptr if the queue is empty.
			*/
			T* Peek() {
				if (read_ptr == write_ptr) {
					return nullptr;
				}

				T* data = read_ptr;
				return data;
			}

			void Resize(size_t new_size) {
				T* new_buffer = new T[new_size];

			}
		};

	}
}