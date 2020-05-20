#include <iostream>
#include "FCFileInfo.hpp"
#include "FCSqliteHandle.hpp"
#include <sqlite3.h>



int main()
{
    FCFileInfo file = FCFileInfo::factory.constructFCFileInfoFromFs("/tmp/file1");
    std::cout << file << std::endl;


    FCSqliteHandle connection;
    int result = sqlite3_open(":memory:", connection.Set());
    if (SQLITE_OK != result) {
        std::cout << sqlite3_errmsg(connection.Get()) << std::endl;
        return result;
    }
}