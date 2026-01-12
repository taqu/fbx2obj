#ifndef INC_FBX2OBJ_ARRAY_H
#define INC_FBX2OBJ_ARRAY_H
#include "fbx2obj.h"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <utility>

namespace fbx2obj
{
template<class T>
class Array
{
public:
    Array();
    Array(const Array& other);
    Array(Array&& other);
    ~Array();

    Array& operator=(const Array& other);
    Array& operator=(Array&& other);

    inline int32_t capacity() const;
    inline int32_t size() const;

    void clear();
    void reserve(int32_t capacity);
    void resize(int32_t size);

    void push_back(const T& x);
    void push_back(T&& x);
    template<class... Args>
    T& emplace_back(Args&&... args);
    void pop_back();

    inline const T& operator[](int32_t index) const;
    inline T& operator[](int32_t index);

private:
    void expand(int32_t size);
    int32_t capacity_;
    int32_t size_;
    T* data_;
};

template<class T>
Array<T>::Array()
    : capacity_(0)
    , size_(0)
    , data_(nullptr)
{
}

template<class T>
Array<T>::Array(const Array& other)
    : capacity_(other.capacity_)
    , size_(other.size_)
{
    assert(this != &other);
    data_ = (T*)::malloc(sizeof(T) * capacity_);
    for(int32_t i = 0; i < size_; ++i) {
        new(&data_[i]) T(other.data_[i]);
    }
}

template<class T>
Array<T>::Array(Array&& other)
    : capacity_(other.capacity_)
    , size_(other.size_)
    , data_(other.data_)
{
    assert(this != &other);
    other.capacity_ = 0;
    other.size_ = 0;
    other.data_ = nullptr;
}

template<class T>
Array<T>::~Array()
{
    for(int32_t i = 0; i < size_; ++i) {
        data_[i].~T();
    }
    ::free(data_);
    capacity_ = 0;
    size_ = 0;
    data_ = nullptr;
}

template<class T>
Array<T>& Array<T>::operator=(const Array& other)
{
    if(this != &other) {
        ~Array();

        capacity_ = other.capacity_;
        size_ = other.size_;
        data_ = (T*)::malloc(sizeof(T) * capacity_);
        for(int32_t i = 0; i < size_; ++i) {
            new(&data_[i]) T(other.data_[i]);
        }
    }
    return *this;
}

template<class T>
Array<T>& Array<T>::operator=(Array&& other)
{
    if(this != &other) {
        ~Array();

        capacity_ = other.capacity_;
        size_ = other.size_;
        data_ = other.data_;

        other.capacity_ = 0;
        other.size_ = 0;
        other.data_ = nullptr;
    }
    return *this;
}

template<class T>
inline int32_t Array<T>::capacity() const
{
    return capacity_;
}

template<class T>
inline int32_t Array<T>::size() const
{
    return size_;
}

template<class T>
void Array<T>::clear()
{
    for(int32_t i=0; i<size_; ++i){
        data_[i].~T();
    }
    size_ = 0;
}

template<class T>
void Array<T>::reserve(int32_t capacity)
{
    assert(0 <= capacity);
    if(capacity <= capacity_) {
        return;
    }
    if(capacity<size_){
        capacity = size_;
    }
    capacity = (capacity + 15) & ~15;
    T* data = (T*)::malloc(sizeof(T) * capacity);
    for(int32_t i = 0; i < size_; ++i) {
        new(&data[i]) T(std::move(data_[i]));
    }
    ::free(data_);
    capacity_ = capacity;
    data_ = data;
}

template<class T>
void Array<T>::resize(int32_t size)
{
    assert(0 <= size);
    expand(size);
    if(size_ < size) {
        for(int32_t i = size_; i < size; ++i) {
            new(&data_[i]) T();
        }
    } else {
        for(int32_t i = size; i < size_; ++i) {
            data_[i].~T();
        }
    }
    size_ = size;
}

template<class T>
void Array<T>::push_back(const T& x)
{
    expand(size_ + 1);
    new(&data_[size_]) T(x);
    ++size_;
}

template<class T>
void Array<T>::push_back(T&& x)
{
    expand(size_ + 1);
    new(&data_[size_]) T(std::forward(x));
    ++size_;
}

template<class T>
template<class... Args>
T& Array<T>::emplace_back(Args&&... args)
{
    expand(size_ + 1);
    int32_t index = size;
    new(&data_[index]) T(std::forward<decltype(args)>(args)...);
    ++size_;
    return data_[index];
}

template<class T>
void Array<T>::pop_back()
{
    assert(0 < size_);
    --size_;
    data_[size_].~T();
}

template<class T>
inline const T& Array<T>::operator[](int32_t index) const
{
    assert(0 <= index && index < size_);
    return data_[index];
}

template<class T>
inline T& Array<T>::operator[](int32_t index)
{
    assert(0 <= index && index < size_);
    return data_[index];
}

template<class T>
void Array<T>::expand(int32_t size)
{
    if(size <= capacity_) {
        return;
    }
    int32_t capacity = (size + 15) & ~15;
    T* data = (T*)::malloc(sizeof(T) * capacity);
    for(int32_t i = 0; i < size_; ++i) {
        new(&data[i]) T(std::move(data_[i]));
    }
    ::free(data_);
    capacity_ = capacity;
    data_ = data;
}
} // namespace fbx2obj
#endif // INC_FBX2OBJ_ARRAY_H
