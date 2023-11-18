#ifndef SINGLETON_HPP
#define SINGLETON_HPP
#include <mutex>
#include <memory>

template <class T>
class Singleton
{
public:
    using Ptr = T*;
private:
    static std::mutex mutex;
    static std::shared_ptr<T> object;
private:
    Singleton(){}
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;
public:
    ~Singleton(){}
    inline static Ptr instance()
    {
        if (object == nullptr) {
            std::lock_guard<std::mutex> locker(mutex);
            if (object == nullptr) {
                object = std::make_shared<T>();
            }
        }
        return object.get();
    }
};
template <class T>
std::shared_ptr<T> Singleton<T>::object = nullptr;
template <class T>
std::mutex Singleton<T>::mutex;
#endif // SINGLETON_HPP
