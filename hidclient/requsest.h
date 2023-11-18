#ifndef REQUSEST_H
#define REQUSEST_H
#include <string>

class Requsest
{
public:
    Requsest();
    static std::string build(int messageType);
    static std::string build(int messageType, const std::string &message);
    static std::string build(int messageType, const std::string &message, const std::string &data);
    static std::string connect();
    static std::string disconnect();
    static std::string text(const std::string &content);
};

#endif // REQUSEST_H
