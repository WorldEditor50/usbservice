#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <cstdio>
#include <chrono>

namespace utils {

class Clock
{
public:
    static std::string currentDateTime()
    {
        auto to_string = [](const std::chrono::system_clock::time_point& t)->std::string
        {
            auto as_time_t = std::chrono::system_clock::to_time_t(t);
            struct tm tm;
#ifdef WIN32
            localtime_s(&tm, &as_time_t);  //win api
#else
            localtime_r(&as_time_t, &tm);//linux api
#endif
            //std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(t.time_since_epoch());
            char buf[128];
            sprintf(buf,  "%04d%02d%02d%02d%02d%02d",
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
            return buf;
        };

        std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
        return to_string(t);
    }
    static unsigned int timestamp()
    {
        auto now = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    }
};


}

#endif // UTILS_H
