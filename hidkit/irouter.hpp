#ifndef IROUTER_H
#define IROUTER_H
#include <string>

class IRouter
{
public:
    IRouter(){}
    virtual ~IRouter(){}
    virtual int invoke(const std::string &)  {return 0;}
};


#endif // IROUTER_H
