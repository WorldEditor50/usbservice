#ifndef ASYNCQUEUE_HPP
#define ASYNCQUEUE_HPP
#include <queue>
#include <list>
#include <mutex>


template<typename T>
class ObjectPool
{
public:
    using Type = T;
    using Ptr = T*;
private:
    std::size_t maxCapacity;
    std::list<Ptr> objects;
public:
    ObjectPool():maxCapacity(512)
    {

    }
    ~ObjectPool()
    {
        for (auto it = objects.begin(); it != objects.end(); it++) {
            Ptr ptr = *it;
            delete ptr;
            ptr = nullptr;
        }
    }
    inline Ptr allocate()
    {
        if (objects.empty()) {
            return new Type();
        }
        Ptr ptr = objects.front();
        objects.pop_front();
        return ptr;
    }

    inline void free(Ptr &ptr)
    {
        if (ptr == nullptr) {
            return;
        }
        if (objects.size() >= maxCapacity) {
            delete ptr;
            ptr = nullptr;
            return;
        }
        objects.push_back(ptr);
        return;
    }

    void reserve(std::size_t reserveSize)
    {
        if (objects.size() >= reserveSize) {
            return;
        }
        long long delta = reserveSize - objects.size();
        for (int i = 0; i < delta; i++) {
            objects.push_back(new Type());
        }
        maxCapacity = reserveSize;
        return;
    }

};
template<typename T>
class AsyncQueue
{
public:
    using Type = T;
    using Ptr = T*;
private:
    std::mutex mutex;
    ObjectPool<T> mempool;
    std::queue<Ptr> container;
public:
    AsyncQueue()
    {
        mempool.reserve(1024);
    }
    inline Ptr front()
    {
        return container.front();
    }

    inline void pop()
    {
        std::lock_guard<std::mutex> locker(mutex);
        Ptr ptr = container.front();
        container.pop();
        mempool.free(ptr);
        return;
    }

    inline void push(Ptr &ptr)
    {
        std::lock_guard<std::mutex> locker(mutex);
        container.push(ptr);
        return;
    }

    inline Ptr allocate()
    {
        std::lock_guard<std::mutex> locker(mutex);
        return mempool.allocate();
    }

};

using BYTE = unsigned char;


template<long long capacity>
class Part
{
public:
    constexpr static long long CAPACITY = capacity;
    using Ptr = BYTE*;
    class Allocator
    {
    public:
        constexpr static std::size_t max_capacity = 64;
    private:
        std::list<Ptr> container;
    public:
        Allocator(){}
        ~Allocator()
        {

        }
        inline Ptr allocate() noexcept
        {
            if (container.empty()) {
                return new BYTE[capacity];
            }
            Ptr ptr = container.front();
            container.pop_front();
            return ptr;
        }

        inline void free(Ptr &ptr) noexcept
        {
            if (ptr == nullptr) {
                return;
            }
            if (container.size() > max_capacity) {
                delete [] ptr;
                ptr = nullptr;
                return;
            }
            container.push_back(ptr);
            return;
        }
    };
public:
    std::size_t used;
    BYTE* data;
    static Allocator allocator;
public:
    Part():used(0),data(nullptr)
    {
        data = allocator.allocate();
    }

    ~Part()
    {
        if (data != nullptr) {
            allocator.free(data);
        }
    }

    Part(const Part &r) noexcept
        :used(r.used)
    {
        data = allocator.allocate();
        memcpy(data, r.data, used);
    }

    Part& operator=(const Part &r) noexcept
    {
        if (this == &r) {
            return *this;
        }
        if (data == nullptr) {
            data = allocator.allocate();
        }
        memcpy(data, r.data, used);
        return *this;
    }

    Part(Part &&r) noexcept
        :used(r.used),data(r.data)
    {
        r.used = 0;
        r.data = nullptr;
    }

    Part& operator=(Part &&r) noexcept
    {
        if (this == &r) {
            return *this;
        }
        used = r.used;
        data = r.data;
        r.used = 0;
        r.data = nullptr;
        return *this;
    }

    inline operator BYTE*() noexcept
    {
        return data;
    }

    inline int write(const BYTE* data_, int datasize)
    {
        int size = capacity - used;
        if (size == 0) {
            return 0;
        }
        int wsize = datasize >= size ? size : datasize;
        memcpy(data + used, data_, wsize);
        used += wsize;
        return wsize;
    }

    inline static void write(std::list<Part> &parts, const BYTE* data, int datasize)
    {
        int pos = 0;
        while (pos < datasize) {
            Part part;
            int len = part.write(data + pos, datasize - pos);
            pos += len;
            parts.push_back(std::move(part));
        }
        return;
    }

    inline void clear()
    {
        used = 0;
        memset(data, 0, capacity);
        return;
    }

    inline bool isFull() const { return used == capacity; }
    inline bool isEmpty() const { return used == 0; }
};

template<long long capacity>
typename Part<capacity>::Allocator Part<capacity>::allocator;


class RingBuffer
{
public:
    constexpr static long long CAPACITY = 4096;
    using PartType = Part<CAPACITY>;
private:
    std::list<PartType> parts;
    std::size_t totalSize;
public:
    RingBuffer():totalSize(0)
    {

    }

    ~RingBuffer()
    {

    }

    int write(const BYTE* &data, int datasize)
    {
        if (parts.empty()) {
            PartType::write(parts, data, datasize);
            totalSize += datasize;
            return 0;
        }
        PartType &part = parts.back();
        int len = part.write(data, datasize);
        if (part.isFull() && len < datasize) {
            PartType::write(parts, data + len, datasize - len);
        }
        totalSize += datasize;
        return 0;
    }

    BYTE* read()
    {
        if (totalSize == 0) {
            return nullptr;
        }
        BYTE *data = new BYTE[totalSize];
        std::size_t pos = 0;
        for (auto it = parts.begin(); it != parts.end(); it++) {
            memcpy(data + pos, it->data, it->used);
            pos += it->used;
        }
        parts.clear();
        return data;
    }
};

#endif // ASYNCQUEUE_HPP
