#include <iostream>
#include "FCFileInfo.hpp"

int main()
{
    FCFileInfo file = FCFileInfo::factory.constructFCFileInfoFromFs("/tmp/file1");
    std::cout << file << std::endl;
}