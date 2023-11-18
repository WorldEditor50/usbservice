#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP
#include <map>
#include <list>
#include <mutex>


class Allocator
{
public:
    using Ptr =  unsigned char*;
    constexpr static std::size_t capacity = 1024;
private:
    std::mutex mutex;
    std::map<std::size_t, std::list<Ptr> > container;
public:

    Allocator(){}

    ~Allocator()
    {
        std::lock_guard<std::mutex> guard(mutex);
        for (auto it = container.begin(); it != container.end(); it++) {
             std::list<Ptr> &list = it->second;
             for (auto x = list.begin(); x != list.end(); x++) {
                 delete [] *x;
             }
        }
    }

    inline Ptr allocate(std::size_t size) noexcept
    {
        std::lock_guard<std::mutex> guard(mutex);
        std::list<Ptr> &list = container[size];
        if (list.empty()) {
            return new unsigned char[size];
        }
        Ptr ptr = list.front();
        list.pop_front();
        return ptr;
    }

    inline void free(std::size_t size, Ptr &ptr)
    {
        if (ptr == nullptr || ptr == 0) {
            return;
        }
        std::lock_guard<std::mutex> guard(mutex);
        std::list<Ptr> &list = container[size];
        if (list.size() > capacity) {
            delete [] ptr;
            ptr = nullptr;
            return;
        }
        list.push_back(ptr);
        return;
    }

    static std::size_t align(std::size_t s)
    {
        std::size_t alignSize = s;
        if (alignSize & 0x3ff) {
            alignSize = ((s>>10) + 1)<<10;
        }
        return alignSize;
    }


};

#endif // ALLOCATOR_HPP
