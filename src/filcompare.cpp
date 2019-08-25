#include "FCFileInfo.hpp"
#include <iostream>
int main()
{
    std::cout << FCFileType::ERR << std::endl;
    FCFileInfo file = FCFileInfo::factory.constructFCFileInfo("/tmp/file1");
}