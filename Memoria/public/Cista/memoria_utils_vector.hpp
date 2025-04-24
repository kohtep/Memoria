//
// memoria_utils_vector.hpp
// 
// An alternative to std::vector<T> to eliminate dependency on the CRT.
//

#pragma once

#include <cstddef>
#include <utility>
#include <memory>

#include "memoria_common.hpp"
#include "memoria_core_mempool.hpp"
#include "memoria_utils_assert.hpp"

MEMORIA_BEGIN

//
// Base class for both dynamic and fixed-size vectors. Holds information about the
// pointer to the managed data and the overall state of the vector.
//
// The `_on_ensure_capacity` callback is used as an alternative to a virtual method
// in order to avoid the creation of a virtual table.
//
template<typename T>
class VectorBase
{
protected:
	using on_ensure_capacity_t = void(*)(void *obj, size_t min_capacity);

	void ensure_capacity(size_t min_capacity)
	{
		if (min_capacity <= _capacity)
			return;

		if (_on_ensure_capacity)
			_on_ensure_capacity(this, min_capacity);
	}

	T *_data = nullptr;
	size_t _size = 0;
	size_t _capacity = 0;

	on_ensure_capacity_t _on_ensure_capacity = nullptr;

public:
	using iterator = T *;
	using const_iterator = const T *;

	using predicate_t = bool(*)(const T &value, void *context);

	constexpr VectorBase() = default;

	void push_back(const T &value)
	{
		ensure_capacity(_size + 1);
		std::construct_at(_data + _size, value);
		++_size;
	}

	void push_back(T &&value)
	{
		ensure_capacity(_size + 1);
		std::construct_at(_data + _size, std::move(value));
		++_size;
	}

	template<typename... Args>
	void emplace_back(Args&&... args)
	{
		ensure_capacity(_size + 1);
		std::construct_at(_data + _size, std::forward<Args>(args)...);
		++_size;
	}

	template<typename... Args>
	iterator emplace(iterator pos, Args&&... args)
	{
		size_t index = pos - _data;
		ensure_capacity(_size + 1);
		pos = _data + index;

		if (index < _size)
		{
			std::construct_at(_data + _size, std::move(_data[_size - 1]));
			for (size_t i = _size - 1; i > index; --i)
			{
				_data[i] = std::move(_data[i - 1]);
			}
			std::destroy_at(_data + index);
		}

		std::construct_at(_data + index, std::forward<Args>(args)...);
		++_size;
		return _data + index;
	}

	iterator insert(iterator pos, T &&value)
	{
		size_t index = pos - _data;
		ensure_capacity(_size + 1);
		pos = _data + index;

		if (index < _size)
		{
			std::construct_at(_data + _size, std::move(_data[_size - 1]));
			for (size_t i = _size - 1; i > index; --i)
			{
				_data[i] = std::move(_data[i - 1]);
			}
			std::destroy_at(_data + index);
		}

		std::construct_at(_data + index, std::move(value));
		++_size;
		return _data + index;
	}

	void pop_back()
	{
		if (_size > 0)
		{
			std::destroy_at(_data + _size - 1);
			--_size;
		}
	}

	iterator erase(iterator pos)
	{
		return erase(pos, pos + 1);
	}

	iterator erase(iterator first, iterator last)
	{
		size_t start = first - _data;
		size_t end = last - _data;

		if (start >= _size || start >= end)
			return _data + start;

		size_t count = end - start;
		for (size_t i = start; i < end; ++i)
		{
			std::destroy_at(_data + i);
		}

		for (size_t i = end; i < _size; ++i)
		{
			_data[i - count] = std::move(_data[i]);
		}

		_size -= count;
		return _data + start;
	}

	T &at(size_t index)
	{
		return _data[index];
	}

	const T &at(size_t index) const
	{
		return _data[index];
	}

	T &operator[](size_t index)
	{
		return _data[index];
	}

	const T &operator[](size_t index) const
	{
		return _data[index];
	}

	T &front()
	{
		return _data[0];
	}

	const T &front() const
	{
		return _data[0];
	}

	T &back()
	{
		return _data[_size - 1];
	}

	const T &back() const
	{
		return _data[_size - 1];
	}

	bool empty() const
	{
		return _size == 0;
	}

	bool full() const
	{
		return _size == _capacity;
	}

	T *data()
	{
		return _data;
	}

	const T *data() const
	{
		return _data;
	}

	size_t size() const
	{
		return _size;
	}

	size_t capacity() const
	{
		return _capacity;
	}

	void clear()
	{
		for (size_t i = 0; i < _size; ++i)
		{
			std::destroy_at(_data + i);
		}
		_size = 0;
	}

	iterator begin()
	{
		return _data;
	}

	const_iterator begin() const
	{
		return _data;
	}

	iterator end()
	{
		return _data + _size;
	}

	const_iterator end() const
	{
		return _data + _size;
	}

	iterator find(const T &value)
	{
		for (size_t i = 0; i < _size; ++i)
		{
			if (_data[i] == value)
				return _data + i;
		}
		return end();
	}

	const_iterator find(const T &value) const
	{
		for (size_t i = 0; i < _size; ++i)
		{
			if (_data[i] == value)
				return _data + i;
		}
		return end();
	}

	iterator find_if(predicate_t pred, void *context = nullptr)
	{
		for (size_t i = 0; i < _size; ++i)
		{
			if (pred(_data[i], context))
				return _data + i;
		}
		return end();
	}

	const_iterator find_if(predicate_t pred, void *context = nullptr) const
	{
		for (size_t i = 0; i < _size; ++i)
		{
			if (pred(_data[i], context))
				return _data + i;
		}
		return end();
	}
};

//
// An alternative to std::vector<T>. Functions similarly to the classic vector
// from the standard library.
//
template<typename T>
class Vector : public VectorBase<T>
{
private:
	static void on_ensure_capacity(void *obj, size_t min_capacity)
	{
		reinterpret_cast<Vector<T> *>(obj)->reserve(min_capacity);
	}

public:
	Vector() : VectorBase<T>()
	{
		this->_data = nullptr;
		this->_size = 0;
		this->_capacity = 0;

		this->_on_ensure_capacity = on_ensure_capacity;
	}

	Vector(size_t count)
	{
		this->_data = nullptr;
		this->_size = 0;
		this->_capacity = 0;

		this->_on_ensure_capacity = on_ensure_capacity;

		if (count > 0)
		{
			this->_data = static_cast<T *>(::operator new(count * sizeof(T)));
			this->_capacity = count;

			for (size_t i = 0; i < count; ++i)
				std::construct_at(this->_data + i);

			this->_size = count;
		}
	}

	~Vector()
	{
		VectorBase<T>::clear();
		::operator delete(this->_data);
	}

	void resize(size_t new_size)
	{
		if (new_size < this->_size)
		{
			for (size_t i = new_size; i < this->_size; ++i)
			{
				std::destroy_at(this->_data + i);
			}

			this->_size = new_size;
		}
		else if (new_size > this->_size)
		{
			this->ensure_capacity(new_size);

			for (size_t i = this->_size; i < new_size; ++i)
			{
				std::construct_at(this->_data + i);
			}
			this->_size = new_size;
		}
	}

	void reserve(size_t new_capacity)
	{
		if (new_capacity <= this->_capacity)
			return;

		T *new_data = static_cast<T *>(::operator new(new_capacity * sizeof(T)));

		for (size_t i = 0; i < this->_size; ++i)
		{
			std::construct_at(new_data + i, std::move(this->_data[i]));
			std::destroy_at(this->_data + i);
		}

		::operator delete(this->_data);
		this->_data = new_data;
		this->_capacity = new_capacity;
	}
};

//
// A fixed-size vector variant with no capacity for expansion when full.
//
// Intended for scenarios where heap allocation is not an option, such as when stack
// memory is preferable, or when it's necessary to avoid the use of `atexit` callbacks
// in the global initialization table.
//
// This container requires more careful handling. Make sure not to insert new elements
// when it is full — any such insertions will be ignored, and the new data will be lost.
//
// [*] You can use the `VectorBase<T>::full` method to check whether the container is full.
//
template<typename T, size_t MaxItems>
class InlineVector : public VectorBase<T>
{
private:
	T _container[MaxItems];

	static void on_ensure_capacity(void *obj, size_t min_capacity)
	{
		AssertMsg(false, "This method should not be called for a fixed-size vector. Reconsider the logic of the caller.");
	}

public:
	constexpr InlineVector() : VectorBase<T>()
	{
		this->_data = _container;
		this->_size = 0;
		this->_capacity = MaxItems;

		this->_on_ensure_capacity = on_ensure_capacity;
	}
};

MEMORIA_END