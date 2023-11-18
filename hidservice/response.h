#ifndef RESPONSE_H
#define RESPONSE_H
#include <string>
#include <iostream>
#include <fstream>
#include <memory>

class Response
{
public:
    static std::string build(int messageType, const std::string &message,
                             std::shared_ptr<unsigned char[]> data, std::size_t datasize);
    static std::string build(int messageType, const std::string &message, const std::string &data);
    static std::string build(int messageType, const std::string &message);
    static std::string ok(int messageType);
    static std::string text(const std::string &content);
};

#endif // RESPONSE_H
