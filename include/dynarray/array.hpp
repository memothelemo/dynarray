#ifndef DYNARRAY_INNER_LIB
#define DYNARRAY_INNER_LIB

#include <cstdint>

namespace dynarray
{

#if __x86_64
    typedef uint64_t uint;
#else
    typedef uint32_t uint;
#endif
template<typename T>
class Array
{
public:
    /** Creates a new empty array */
    Array();

    /**
     * Creates a new array based on the array pointer.
     *
     * @param ptr Non-readonly array pointer
     * @param size Size of the array pointer
     */
    Array(T *ptr, uint size);
    ~Array();

    /**
     * Creates a new array with limited capacity.
     */
    static Array<T> with_capacity(uint elements) {
        Array<T> array;
        array.m_capacitySize = elements;
        return array;
    }

    struct Iterator {
    public:
        Iterator(const Array<T> *m_array)
            : m_array(m_array)
            , m_index(0)
        {}

        Iterator(const Array<T> *m_array, uint index)
            : m_array(m_array)
            , m_index(index)
        {}

        Iterator &operator++() noexcept
        {
            if (this->m_index < this->m_array->m_length) {
                this->m_index++;
            }
            return *this;
        }

        Iterator operator++(int) noexcept
        {
            Iterator tempIter = *this;
            ++*this;
            return tempIter;
        }

        bool operator!=(const Iterator &other) noexcept
        {
            return this->m_index != other.m_index;
        }

        T operator*() const noexcept
        {
            return this->m_array->m_ptr[this->m_index];
        }

    private:
        const Array<T> *m_array;
        uint m_index;
    };

    uint len() const noexcept;

    void insert(uint index, T element);
    uint push(T element);

    T pop_return();
    void pop();

    T remove(uint index);

    T *get(uint index) const noexcept;
    T expect_get(uint index);

    T *c_array() const noexcept;

    inline T operator[] (uint index);

    T *operator*() const noexcept;

    Iterator begin() const noexcept;
    Iterator end() const noexcept;

private:
    T *m_ptr;
    bool m_isPtrFromClass;
    uint m_length;
    uint m_capacitySize;
};

}

#endif
