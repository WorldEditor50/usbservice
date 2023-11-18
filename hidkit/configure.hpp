#ifndef CONFIGURE_HPP
#define CONFIGURE_HPP
#include <string>
#include "singleton.hpp"

class Configure_
{
private:
    std::string filePath;
    std::string imagePath;
public:
    Configure_(){}
    std::string getFilePath() const {return filePath;}
    void setFilePath(const std::string &path) {filePath = path;}
    std::string getImagePath() const {return imagePath;}
    void setImagePath(const std::string &path) {imagePath = path;}
};

using Configure = Singleton<Configure_>;
#endif // CONFIGURE_HPP
