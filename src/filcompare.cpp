#include "FCFileInfo.hpp"
#include <iostream>
int main()
{
    FCFileInfo file = FCFileInfo::factory.constructFCFileInfoFromFs("/tmp/file1");
}