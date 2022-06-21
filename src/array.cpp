#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "../include/dynarray/array.hpp"

namespace dynarray {

#define ARRAY_PANIC_TAG "[arraylib] "

#define ARRAY_PANIC_PRELUDE(reason) \
    ARRAY_PANIC_LOG("!!!! Aborted !!!!") \
    ARRAY_PANIC_LOG("Reason: " << reason)\
    std::cout << ARRAY_PANIC_TAG << std::endl;

#define ARRAY_PANIC_LOG(part) \
    std::cout << ARRAY_PANIC_TAG << part << std::endl;

#define ARRAY_PANIC_END_SEG \
    ARRAY_PANIC_LOG("If you want to get the source of this panic, please use Valgrind (for GNU Linux) or similar tools.") \
    ARRAY_PANIC_LOG("Aborting program because of this error.") \
    abort();

#define ARRAY_PANIC(reason) \
    ARRAY_PANIC_PRELUDE(reason) \
    ARRAY_PANIC_END_SEG

#define ARRAY_PANIC_WITH_PART(reason, part) \
    ARRAY_PANIC_PRELUDE(reason) \
    part \
    ARRAY_PANIC_END_SEG

template<typename T>
T *alloc(uint size) {
    T *ptr = reinterpret_cast<T *>(malloc(sizeof(T) * size));
    if (ptr == nullptr) {
        ARRAY_PANIC("allocation failed")
    }
    return ptr;
}

template<typename T>
T *reallocate(T *ptr, uint size) {
    T *new_ptr = reinterpret_cast<T *>(realloc(ptr, sizeof(T) * size));
    if (new_ptr == nullptr) {
        ARRAY_PANIC("reallocation failed")
    }
    return new_ptr;
}

template<typename T>
Array<T>::Array()
    : m_ptr(nullptr)
    , m_isPtrFromClass(true)
    , m_length(0)
    , m_capacitySize(0)
{
}

#if __x86_64
    #define UINT_MAX UINT64_MAX
#else
    #define UINT_MAX UINT32_MAX
#endif

template<typename T>
Array<T>::Array(T *ptr, uint size)
    : m_ptr(ptr)
    , m_isPtrFromClass(false)
    , m_length(size)
    , m_capacitySize(0)
{
}

template<typename T>
Array<T>::~Array<T>() {
    if (this->m_isPtrFromClass)
        free(this->m_ptr);
}

template<typename T>
uint Array<T>::push(T element) {
    if (this->m_capacitySize != 0 && this->m_length == this->m_capacitySize) {
        ARRAY_PANIC_WITH_PART("Attempt to push array exceeding the capacity size",
            ARRAY_PANIC_LOG("capacity = " << this->m_capacitySize << ", size = " << this->m_length + 1);
            ARRAY_PANIC_LOG(""))
    }
    if (this->m_ptr == nullptr) {
        if (this->m_isPtrFromClass) {
            this->m_ptr = alloc<T>(1);
            this->m_ptr[0] = element;
            this->m_isPtrFromClass = true;
        } else {
            ARRAY_PANIC("Attempt to push array with undefined pointer")
        }
    } else {
        this->m_ptr = reallocate<T>(this->m_ptr, this->m_length + 1);
        this->m_ptr[this->m_length] = element;
    }
    uint assigned_index = this->m_length;
    this->m_length++;
    return assigned_index;
}

template<typename T>
void Array<T>::pop() {
    if (this->m_length == 0) {
        ARRAY_PANIC("Attempt to pop empty array")
    }
    uint new_size = this->m_length - 1;
    this->m_ptr[new_size] = 0;
    this->m_length = new_size;
}

template<typename T>
T Array<T>::pop_return() {
    if (this->m_length == 0) {
        ARRAY_PANIC("Attempt to pop empty array")
    }
    T element = this->m_ptr[this->m_length - 1];
    uint new_size = this->m_length - 1;
    this->m_ptr[new_size] = 0;
    this->m_length = new_size;
    return element;
}

template<typename T>
void Array<T>::insert(uint index, T element) {
    if (this->m_capacitySize != 0 && this->m_length == this->m_capacitySize) {
        ARRAY_PANIC_WITH_PART("Attempt to insert array exceeding the capacity size",
            ARRAY_PANIC_LOG("capacity = " << this->m_capacitySize << ", size = " << this->m_length + 1);
            ARRAY_PANIC_LOG(""))
    }

    T *new_ptr = alloc<T>(this->m_length + 1);

    // transfering things into a new array
    uint newSize = 0;
    for (uint i = 0; i < index; i++) {
        new_ptr[newSize] = this->m_ptr[i];
        newSize++;
    }

    new_ptr[newSize] = element;
    newSize++;

    for (uint i = index; i < this->m_length; i++) {
        new_ptr[newSize] = this->m_ptr[i];
        newSize++;
    }

    if (newSize != this->m_length + 1) {
        ARRAY_PANIC("unequal new size and expected growth size")
    }

    free(this->m_ptr);
    this->m_ptr = new_ptr;
    this->m_length++;
}

template<typename T>
T Array<T>::remove(uint index) {
    if (index >= this->m_length) {
        ARRAY_PANIC_WITH_PART("Attempt to remove a member of array that is out of bounds",
            ARRAY_PANIC_LOG("index to remove = " << index << ", size = " << this->m_length)
            ARRAY_PANIC_LOG(""))
    }

    T element = this->m_ptr[index];
    T *new_ptr = alloc<T>(this->m_length - 1);

    // transfering things into a new array
    uint newSize = 0;
    for (uint i = 0; i < index; i++) {
        new_ptr[newSize] = this->m_ptr[i];
        newSize++;
    }

    for (uint i = index + 1; i < this->m_length ; i++) {
        new_ptr[newSize] = this->m_ptr[i];
        newSize++;
    }

    free(this->m_ptr);
    this->m_ptr = new_ptr;

    return element;
}

template<typename T>
typename Array<T>::Iterator Array<T>::begin() const noexcept {
    return Array<T>::Iterator(this);
}

template<typename T>
typename Array<T>::Iterator Array<T>::end() const noexcept {
    return Array<T>::Iterator(this, this->m_length);
}

template<typename T>
T *Array<T>::get(uint index) const noexcept {
    if (index > this->m_length) {
        return nullptr;
    } else {
        return &this->m_ptr[index];
    }
}

template<typename T>
T Array<T>::expect_get(uint index) {
    if (index > this->m_length) {
        ARRAY_PANIC_WITH_PART("Attempt to get a member of array that is out of bounds",
            ARRAY_PANIC_LOG("index = " << index << ", size = " << this->m_length)
            ARRAY_PANIC_LOG(""))
    } else {
        return this->m_ptr[index];
    }
}

template<typename T>
uint Array<T>::len() const noexcept {
    return this->m_length;
}

template<typename T>
T *Array<T>::c_array() const noexcept {
    return this->m_ptr;
}

template<typename T>
T *Array<T>::operator*() const noexcept {
    return this->m_ptr;
};

template<typename T>
inline T Array<T>::operator[] (uint index) {
    return this->expect_get(index);
}

}
