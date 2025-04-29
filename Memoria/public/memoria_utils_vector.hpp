//
// memoria_utils_vector.hpp
// 
// An alternative to std::vector<T> to eliminate dependency on the CRT.
//
// Many decisions in this file may seem unusual, but each one is based on principles 
// that helped the MSVC compiler understand that we do not require the generation 
// of global dynamic constructors, destructors, virtual tables, or other elements 
// that could introduce binary overhead.
// 
// [*] Fact: Although I did my best to make the compiler stop injecting
// calls like `memset` (as I often mention below), I was ultimately unable to 
// eliminate them entirely without sacrificing C++ language conveniences 
// (e.g., fully adhering to POD principles).
// Therefore, if you're developing a NoCRT project, the simplest solution is to
// override functions like `memset` or `memmove` in your own files, using 
// Memoria equivalents such as `Memoria::MemSet` and `Memoria::MemMove`, respectively.
//

#pragma once

#include <cstddef>
#include <utility>
#include <memory>

#include "memoria_common.hpp"
#include "memoria_core_mempool.hpp"
#include "memoria_utils_assert.hpp"
#include "memoria_utils_string.hpp"

MEMORIA_BEGIN

//
// Base class for both dynamic and fixed-size vectors. Holds information about the
// pointer to the managed data and the overall state of the vector.
//
// All methods related to adding elements (push, emplace) are defined 
// in the derived classes so that we do not need to generate a constructor 
// call. This is because some container elements require initialization; 
// for example, `FixedVector` requires assigning `_container` to 
// `VectorBase<T>::_data`. However, if this is done in the constructor, 
// the compiler usually forces the addition of the constructor to the CRT table. 
// This is particularly important if we place the vector in the global scope.
//
// Therefore, if we want to avoid this, we must implement the container's 
// constructor as trivial. To achieve this, we need to use workarounds, such as 
// moving some parent class methods into the derived classes.
// 
// The `ensure_capacity` method is used as an alternative to a virtual method
// in order to avoid the creation of a virtual table.
//
template<typename T>
class VectorBase
{
protected:
	//
	// `mutable` had to be added to allow these fields to be modified
	// inside `const` methods such as `capacity` and `full`, in order to satisfy
	// the initialization requirements of the vector when accessing them.
	//

	mutable T *_data = nullptr;
	mutable size_t _size = 0;
	mutable size_t _capacity = 0;

public:
	using iterator = T *;
	using const_iterator = const T *;

	using predicate_t = bool(*)(const T &value, void *context);
	using compare_t = int(*)(const T &a, const T &b, void *context);

	constexpr VectorBase() = default;

	void pop_back()
	{
		if (_size > 0)
		{
			std::destroy_at(_data + _size - 1);
			--_size;
		}
	}

	VectorBase<T>::iterator erase(VectorBase<T>::iterator pos)
	{
		return erase(pos, pos + 1);
	}

	VectorBase<T>::iterator erase(VectorBase<T>::iterator first, VectorBase<T>::iterator last)
	{
		size_t start = first - _data;
		size_t end = last - _data;

		if (start >= _size || start >= end)
			return _data + start;

		size_t count = end - start;
		for (size_t i = start; i < end; ++i)
			std::destroy_at(_data + i);

		for (size_t i = end; i < _size; ++i)
			_data[i - count] = std::move(_data[i]);

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

	void clear()
	{
		for (size_t i = 0; i < _size; ++i)
			std::destroy_at(_data + i);

		_size = 0;
	}

	VectorBase<T>::iterator begin()
	{
		return _data;
	}

	VectorBase<T>::const_iterator begin() const
	{
		return _data;
	}

	VectorBase<T>::iterator end()
	{
		return _data + _size;
	}

	VectorBase<T>::const_iterator end() const
	{
		return _data + _size;
	}

	VectorBase<T>::iterator find(const T &value)
	{
		for (size_t i = 0; i < _size; ++i)
		{
			if (_data[i] == value)
				return _data + i;
		}
		return end();
	}

	VectorBase<T>::const_iterator find(const T &value) const
	{
		for (size_t i = 0; i < _size; ++i)
		{
			if (_data[i] == value)
				return _data + i;
		}
		return end();
	}

	VectorBase<T>::iterator find_if(predicate_t pred, void *context = nullptr)
	{
		for (size_t i = 0; i < _size; ++i)
		{
			if (pred(_data[i], context))
				return _data + i;
		}
		return end();
	}

	VectorBase<T>::const_iterator find_if(predicate_t pred, void *context = nullptr) const
	{
		for (size_t i = 0; i < _size; ++i)
		{
			if (pred(_data[i], context))
				return _data + i;
		}
		return end();
	}

	void sort(compare_t cmp, void *context = nullptr)
	{
		for (size_t i = 1; i < _size; ++i)
		{
			T temp = std::move(_data[i]);
			size_t j = i;

			while (j > 0 && cmp(temp, _data[j - 1], context) < 0)
			{
				_data[j] = std::move(_data[j - 1]);
				--j;
			}

			_data[j] = std::move(temp);
		}
	}

	void zero_initialize()
	{
		if (_data)
			MemFill(_data, 0, sizeof(T) * _capacity);
	}

	bool would_overflow(size_t count = 1) const
	{
		return _size + count > _capacity;
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
	void ensure_capacity(size_t min_capacity)
	{
		if (min_capacity <= this->_capacity)
			return;

		reserve(min_capacity);
	}

public:
	Vector() : VectorBase<T>()
	{
		this->_data = nullptr;
		this->_size = 0;
		this->_capacity = 0;
	}

	Vector(size_t count)
	{
		this->_data = nullptr;
		this->_size = 0;
		this->_capacity = 0;

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

	//
	// The parent `push`, `emplace`, and `insert` methods are defined in the derived class 
	// to avoid generating constructor calls in the global scope.
	//

	void push_back(const T &value)
	{
		ensure_capacity(this->_size + 1);
		std::construct_at(this->_data + this->_size, value);
		++this->_size;
	}

	void push_back(T &&value)
	{
		ensure_capacity(this->_size + 1);
		std::construct_at(this->_data + this->_size, std::move(value));
		++this->_size;
	}

	template<typename... Args>
	void emplace_back(Args&&... args)
	{
		ensure_capacity(this->_size + 1);
		std::construct_at(this->_data + this->_size, std::forward<Args>(args)...);
		++this->_size;
	}

	template<typename... Args>
	VectorBase<T>::iterator emplace(VectorBase<T>::iterator pos, Args&&... args)
	{
		size_t index = pos - this->_data;
		ensure_capacity(this->_size + 1);
		pos = this->_data + index;

		if (index < this->_size)
		{
			std::construct_at(this->_data + this->_size, std::move(this->_data[this->_size - 1]));

			for (size_t i = this->_size - 1; i > index; --i)
				this->_data[i] = std::move(this->_data[i - 1]);

			std::destroy_at(this->_data + index);
		}

		std::construct_at(this->_data + index, std::forward<Args>(args)...);
		++this->_size;
		return this->_data + index;
	}

	VectorBase<T>::iterator insert(VectorBase<T>::iterator pos, T &&value)
	{
		size_t index = pos - this->_data;
		ensure_capacity(this->_size + 1);
		pos = this->_data + index;

		if (index < this->_size)
		{
			std::construct_at(this->_data + this->_size, std::move(this->_data[this->_size - 1]));

			for (size_t i = this->_size - 1; i > index; --i)
				this->_data[i] = std::move(this->_data[i - 1]);


			std::destroy_at(this->_data + index);
		}

		std::construct_at(this->_data + index, std::move(value));
		++this->_size;
		return this->_data + index;
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

	bool full() const
	{
		ensure_capacity(0);

		return this->_size == this->_capacity;
	}

	size_t capacity() const
	{
		ensure_capacity(0);

		return this->_capacity;
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
// Fact: if you add `{}` or `= {}` to a class field, the compiler stops generating 
// a constructor that includes a `memset` call. I do not fully understand this point, 
// because theoretically, adding `{}` should have the same effect.
// 
// [*] You can use the `VectorBase<T>::full` method to check whether the container is full.
//
template<typename T, size_t MaxItems>
class FixedVector : public VectorBase<T>
{
private:
	//
	// I had to abandon the declaration style `T _container[MaxItems]{}`
	// to prevent the compiler from injecting unnecessary `memset` calls
	// for the array.
	//
	alignas(T) uint8_t _container[sizeof(T) * MaxItems]{};

	void ensure_capacity(size_t min_capacity)
	{
		if (!this->_capacity)
			this->_capacity = MaxItems;

		if (!this->_data)
			this->_data = reinterpret_cast<T *>(&_container);

		if (min_capacity > this->_capacity)
			AssertMsg(false, "This method should not be executed for a fixed-size vector. Reconsider the logic of the caller.");
	}

public:
	constexpr FixedVector() = default;

	//
	// The parent `push`, `emplace`, and `insert` methods are defined in the derived class 
	// to avoid generating constructor calls in the global scope.
	//

	void push_back(const T &value)
	{
		ensure_capacity(this->_size + 1);
		std::construct_at(this->_data + this->_size, value);
		++this->_size;
	}

	void push_back(T &&value)
	{
		ensure_capacity(this->_size + 1);
		std::construct_at(this->_data + this->_size, std::move(value));
		++this->_size;
	}

	template<typename... Args>
	void emplace_back(Args&&... args)
	{
		ensure_capacity(this->_size + 1);
		std::construct_at(this->_data + this->_size, std::forward<Args>(args)...);
		++this->_size;
	}
	
	void emplace_back()
	{
		ensure_capacity(this->_size + 1);
		++this->_size;
	}

	template<typename... Args>
	VectorBase<T>::iterator emplace(VectorBase<T>::iterator pos, Args&&... args)
	{
		size_t index = pos - this->_data;
		ensure_capacity(this->_size + 1);
		pos = this->_data + index;

		if (index < this->_size)
		{
			std::construct_at(this->_data + this->_size, std::move(this->_data[this->_size - 1]));

			for (size_t i = this->_size - 1; i > index; --i)
				this->_data[i] = std::move(this->_data[i - 1]);

			std::destroy_at(this->_data + index);
		}

		std::construct_at(this->_data + index, std::forward<Args>(args)...);
		++this->_size;
		return this->_data + index;
	}

	VectorBase<T>::iterator insert(VectorBase<T>::iterator pos, T &&value)
	{
		size_t index = pos - this->_data;
		ensure_capacity(this->_size + 1);
		pos = this->_data + index;

		if (index < this->_size)
		{
			std::construct_at(this->_data + this->_size, std::move(this->_data[this->_size - 1]));

			for (size_t i = this->_size - 1; i > index; --i)
				this->_data[i] = std::move(this->_data[i - 1]);


			std::destroy_at(this->_data + index);
		}

		std::construct_at(this->_data + index, std::move(value));
		++this->_size;
		return this->_data + index;
	}

	bool full() const
	{
		if (!this->_capacity)
			this->_capacity = MaxItems;

		return this->_size == this->_capacity;
	}

	size_t capacity() const
	{
		if (!this->_capacity)
			this->_capacity = MaxItems;

		return this->_capacity;
	}
};

//
// A slightly different version of the container compared to `FixedVector`, but it serves 
// the same purpose, except that the data storage is explicitly provided by the caller 
// through the constructor.
//
// This version of the vector was created when I was trying to find a way to avoid 
// the generation of global constructors. Over time, I found a solution without the need 
// to create alternative classes, but I would prefer to keep this version for now.
//
template<typename T>
class InlineVector : public VectorBase<T>
{
private:
	void ensure_capacity(size_t min_capacity)
	{
		AssertMsg(this->_data, "Data storage has not been initialized. A call to an appropriate constructor is required.");

		if (min_capacity > this->_capacity)
			AssertMsg(false, "This method should not be executed for a fixed-size vector. Reconsider the logic of the caller.");
	}

public:
	constexpr InlineVector() noexcept
	{
		this->_data = nullptr;
		this->_size = 0;
		this->_capacity = 0;
	}

	template<size_t N>
	constexpr InlineVector(int8_t(&buffer)[N]) noexcept
	{
		static_assert(N % sizeof(T) == 0, "Buffer size must be divisible by element size.");

		this->_data = reinterpret_cast<T *>(buffer);
		this->_size = 0;
		this->_capacity = N / sizeof(T);
	}


	template<size_t N>
	constexpr InlineVector(uint8_t(&buffer)[N]) noexcept
	{
		static_assert(N % sizeof(T) == 0, "Buffer size must be divisible by element size.");

		this->_data = reinterpret_cast<T *>(buffer);
		this->_size = 0;
		this->_capacity = N / sizeof(T);
	}

	template<size_t N>
	constexpr InlineVector(std::byte(&buffer)[N]) noexcept
	{
		static_assert(N % sizeof(T) == 0, "Buffer size must be divisible by element size.");

		this->_data = reinterpret_cast<T *>(buffer);
		this->_size = 0;
		this->_capacity = N / sizeof(T);
	}

	//
	// The parent `push`, `emplace`, and `insert` methods are defined in the derived class 
	// to avoid generating constructor calls in the global scope.
	//

	void push_back(const T &value)
	{
		ensure_capacity(this->_size + 1);
		std::construct_at(this->_data + this->_size, value);
		++this->_size;
	}

	void push_back(T &&value)
	{
		ensure_capacity(this->_size + 1);
		std::construct_at(this->_data + this->_size, std::move(value));
		++this->_size;
	}

	template<typename... Args>
	void emplace_back(Args&&... args)
	{
		ensure_capacity(this->_size + 1);
		std::construct_at(this->_data + this->_size, std::forward<Args>(args)...);
		++this->_size;
	}

	template<typename... Args>
	VectorBase<T>::iterator emplace(VectorBase<T>::iterator pos, Args&&... args)
	{
		size_t index = pos - this->_data;
		ensure_capacity(this->_size + 1);
		pos = this->_data + index;

		if (index < this->_size)
		{
			std::construct_at(this->_data + this->_size, std::move(this->_data[this->_size - 1]));

			for (size_t i = this->_size - 1; i > index; --i)
				this->_data[i] = std::move(this->_data[i - 1]);

			std::destroy_at(this->_data + index);
		}

		std::construct_at(this->_data + index, std::forward<Args>(args)...);
		++this->_size;
		return this->_data + index;
	}

	VectorBase<T>::iterator insert(VectorBase<T>::iterator pos, T &&value)
	{
		size_t index = pos - this->_data;
		ensure_capacity(this->_size + 1);
		pos = this->_data + index;

		if (index < this->_size)
		{
			std::construct_at(this->_data + this->_size, std::move(this->_data[this->_size - 1]));

			for (size_t i = this->_size - 1; i > index; --i)
				this->_data[i] = std::move(this->_data[i - 1]);


			std::destroy_at(this->_data + index);
		}

		std::construct_at(this->_data + index, std::move(value));
		++this->_size;
		return this->_data + index;
	}

	bool full() const
	{
		ensure_capacity(0);

		return this->_size == this->_capacity;
	}

	size_t capacity() const
	{
		ensure_capacity(0);

		return this->_capacity;
	}
};

MEMORIA_END

//
// Use when creating an inline vector within a limited scope 
// (e.g., inside a function).
//
#define DEFINE_INLINE_VECTOR(type, name, max_elements) \
    uint8_t name##_buffer[(max_elements) * sizeof(type)]{}; \
    Memoria::InlineVector<type> name(name##_buffer)

//
// Use when creating an inline vector within the global scope.
// Using this inside a limited scope may cause TLS (Thread-Local Storage) overhead.
//
#define DEFINE_STATIC_INLINE_VECTOR(type, name, max_elements) \
    static uint8_t name##_buffer[(max_elements) * sizeof(type)]{}; \
    static Memoria::InlineVector<type> name(name##_buffer)