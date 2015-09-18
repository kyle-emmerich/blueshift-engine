#pragma once
#include <memory>

//Note: Someday, this could be intensely sped up
//by allocating pools of 8MB or so and linking them.
//If an array spans more than 8MB, it'd just store a
//link to the next pool and do so invisibly under this
//API.

//However, that might be too badass and no one would ever notice. :(

template<typename T>
class Array;

template<typename T>
class ArrayIterator {
private:
	const Array<T>* container;
	size_t position;
public:
	ArrayIterator(const Array<T>* Container, size_t Position)
		: container(Container), position(Position) {}

	bool operator!= (const ArrayIterator<T> other) const {
		return position != other.position;
	}

	T& operator* () const;

	const ArrayIterator<T> operator++ () {
		position++;
		return *this;
	}
};

template<typename T>
class Array {
private:
	T* _internal = 0;
	size_t size;
	size_t reserved;

	static const size_t margin = 5;
public:
	Array(size_t reserve = margin)
		: reserved(reserve), size(0) {
		_internal = (T*)realloc(_internal, sizeof(T) * reserved);
	}
	Array(Array& other)
		: reserved(other.reserved), size(other.size) {
		_internal = (T*)realloc(_internal, sizeof(T) * reserved);
		memcpy(_internal, other._internal, sizeof(T) * size);
	}
	~Array() {
		free(_internal);
	}

	inline Array<T>& operator=(const Array<T>& other) {
		Array<T> tmp(other);
		std::swap(reserved, tmp.reserved);
		std::swap(size, tmp.size);
		std::swap(_internal, tmp._internal);
		return *this;
	}

	inline Array<T>& operator=(Array<T>&& other) {
		swap(reserved, other.reserved);
		swap(size, other.size);
		swap(_internal, other._internal);
	}

	inline void Resize(size_t new_size) {
		if (reserved == new_size) {
			return;
		}
		_internal = (T*)realloc(_internal, sizeof(T) * new_size);
		reserved = new_size;
	}

	inline void Add(T&& Item) {
		if (size + 1 > reserved) {
			Resize(reserved + margin);
		}
		_internal[size++] = Item;
	}
	inline void Add(T& Item) {
		if (size + 1 > reserved) {
			Resize(reserved + margin);
		}
		new(_internal + size++) T(Item);
	}
	inline void Add(const T& Item) {
		if (size + 1 > reserved) {
			Resize(reserved + margin);
		}
		new(_internal + size++) T(Item);
	}

	inline bool AddUnique(T&& Item) {
		for (size_t i = 0; i < size; i++) {
			if (_internal[i] == Item) {
				return false;
			}
		}
		Add(Item);
		return true;
	}
	inline bool AddUnique(T& Item) {
		for (size_t i = 0; i < size; i++) {
			if (_internal[i] == Item) {
				return false;
			}
		}
		Add(Item);
		return true;
	}
	inline bool AddUnique(const T& Item) {
		for (size_t i = 0; i < size; i++) {
			if (_internal[i] == Item) {
				return false;
			}
		}
		Add(Item);
		return true;
	}
	
	inline bool Remove(T& Item) {
		bool found = false;
		for (size_t i = 0; i < size; i++) {
			if (!found) {
				if (_internal[i] == Item) {
					found = true;
				}
			} else {
				_internal[i - 1] = _internal[i];
			}
		}
		size--;

		if (size + margin < reserved) {
			Resize(reserved - margin);
		}

		return found;
	}

	inline void RemoveAt(size_t index) {
		//TODO: check range and throw exception
		for (size_t i = index + 1; i < size; i++) {
			_internal[i - 1] = _internal[i];
		}
		size--;

		if (size + margin < reserved) {
			Resize(reserved - margin);
		}
	}

	inline T& operator[](size_t index) const {
		//TODO: check range and throw exception
		return _internal[index];
	}

	inline size_t GetSize() const {
		return size;
	}
	inline size_t GetReservedSize() const {
		return reserved;
	}

	inline bool Contains(T& value) const {
		for (size_t i = 0; i < size; i++) {
			if (_internal[i] == value) {
				return true;
			}
		}
		return false;
	}

	ArrayIterator<T> begin() const {
		return ArrayIterator<T>(this, 0);
	}
	ArrayIterator<T> end() const {
		return ArrayIterator<T>(this, size);
	}
};

template<typename T>
T& ArrayIterator<T>::operator* () const {
	//TODO: throw exception if out of range
	return (*container)[position];
}