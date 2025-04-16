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

MEMORIA_BEGIN

template<typename T>
class Vector
{
private:
    void ensure_capacity(size_t min_capacity)
    {
        if (min_capacity <= _capacity)
            return;

        reserve(min_capacity);
    }

    T *_data = nullptr;
    size_t _size = 0;
    size_t _capacity = 0;

public:
    using iterator = T *;
    using const_iterator = const T *;

    Vector() = default;

    Vector(size_t count)
        : _data(nullptr), _size(0), _capacity(0)
    {
        if (count == 0)
            return;

        _data = static_cast<T *>(Memoria::New(count * sizeof(T)));
        _capacity = count;
        for (size_t i = 0; i < count; ++i)
        {
            std::construct_at(_data + i);
        }
        _size = count;
    }

    ~Vector()
    {
        clear();
        Memoria::Free(_data);
    }

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
        size_t index = pos - _data;

        if (index >= _size)
            return end();

        std::destroy_at(_data + index);

        for (size_t i = index; i < _size - 1; ++i)
        {
            _data[i] = std::move(_data[i + 1]);
        }

        --_size;
        return _data + index;
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

    size_t capacity() const
    {
        return _capacity;
    }

    void resize(size_t new_size)
    {
        if (new_size < _size)
        {
            for (size_t i = new_size; i < _size; ++i)
            {
                std::destroy_at(_data + i);
            }
            _size = new_size;
        }
        else if (new_size > _size)
        {
            ensure_capacity(new_size);
            for (size_t i = _size; i < new_size; ++i)
            {
                std::construct_at(_data + i);
            }
            _size = new_size;
        }
    }

    void reserve(size_t new_capacity)
    {
        if (new_capacity <= _capacity)
            return;

        T *new_data = static_cast<T *>(Memoria::New(new_capacity * sizeof(T)));

        for (size_t i = 0; i < _size; ++i)
        {
            std::construct_at(new_data + i, std::move(_data[i]));
            std::destroy_at(_data + i);
        }

        Memoria::Free(_data);
        _data = new_data;
        _capacity = new_capacity;
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
};

MEMORIA_END