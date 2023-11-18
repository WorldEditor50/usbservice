#include "bytearray.hpp"

Allocator ByteArray::allocator;

void ByteArray::swap(ByteArray& r)
{
    ByteArray tmp;
    tmp.ptr = ptr;
    tmp.used = used;
    tmp.capacity = capacity;
    ptr = r.ptr;
    used = r.used;
    capacity = r.capacity;
    r.used = tmp.used;
    r.capacity = tmp.capacity;
    r.ptr = tmp.ptr;
    return;
}
