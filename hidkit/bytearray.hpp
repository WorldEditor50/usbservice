#ifndef BYTEARRAY_HPP
#define BYTEARRAY_HPP
#include "allocator.hpp"
#include <string>

class ByteArray
{
protected:
    std::size_t used;
    std::size_t totalSize;
    std::size_t capacity;
    unsigned char* ptr;
public:
    static Allocator allocator;
public:
    ByteArray():used(0),totalSize(0),capacity(0),ptr(nullptr){}
    ~ByteArray()
    {
        if (ptr !=  nullptr) {
            allocator.free(capacity, ptr);
        }
    }
    ByteArray(std::size_t s)
        :used(0),totalSize(s)
    {
        capacity = Allocator::align(s);
        ptr = allocator.allocate(capacity);
    }

    ByteArray(const unsigned char* data_, std::size_t s)
        :used(s),totalSize(s)
    {
        capacity = Allocator::align(s);
        ptr = allocator.allocate(capacity);
        memcpy(ptr, data_, s);
    }

    ByteArray(const ByteArray &r)
        :used(r.used),totalSize(r.totalSize),capacity(r.capacity)
    {
        ptr = allocator.allocate(capacity);
        memcpy(ptr, r.ptr, r.used);
    }

    ByteArray& operator=(const ByteArray &r)
    {
        if (this == &r) {
            return *this;
        }
        if (ptr == nullptr) {
            ptr = allocator.allocate(r.capacity);
        }
        if (r.used > capacity) {
            allocator.free(capacity, ptr);
            ptr = allocator.allocate(r.capacity);
        }
        memcpy(ptr, r.ptr, r.used);
        used = r.used;
        totalSize = r.totalSize;
        capacity = r.capacity;
        return *this;
    }

    ByteArray(ByteArray &&r)
        :used(r.used),totalSize(r.totalSize),capacity(r.capacity),ptr(r.ptr)
    {
        r.used = 0;
        r.totalSize = 0;
        r.capacity = 0;
        r.ptr = nullptr;
    }

    ByteArray& operator=(ByteArray &&r)
    {
        if (this == &r) {
            return *this;
        }
        ptr = r.ptr;
        used = r.used;
        totalSize = r.totalSize;
        capacity = r.capacity;
        r.used = 0;
        r.totalSize = 0;
        r.capacity = 0;
        r.ptr = nullptr;
        return *this;
    }

    inline operator unsigned char*() noexcept
    {
        return ptr;
    }

    inline unsigned char operator[](std::size_t i) {return ptr[i];}

    inline std::string toString() const
    {
        return std::string((char*)ptr, used);
    }

    inline std::size_t size() const {return used;}
    static ByteArray fromString(const std::string &str)
    {
        return ByteArray((unsigned char*)str.c_str(), str.size());
    }
    void swap(ByteArray& r);
};

#endif
