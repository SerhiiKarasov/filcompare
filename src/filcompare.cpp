#include <iostream>
#include <string>
#include "FCFileInfo.hpp"
#include "FCSqliteIO.hpp"
#include <stdarg.h> /* Needed for the definition of va_list */

int main()
{
    FCFileInfo file1 = FCFileInfo::factory.constructFCFileInfoFromFs("/tmp/file1");
    FCFileInfo file2 = FCFileInfo::factory.constructFCFileInfoFromFs("/tmp/file2");
    FCFileInfo::FCFiles files{ file1, file2 };
    FCSqliteIO sqlitewriter;
    sqlitewriter.WriteToDb("/tmp/mydb.db", files);
    for (const auto &file_read : sqlitewriter.ReadFromDb("/tmp/mydb.db")) {
        std::cout << "we read " << file_read << std::endl;
    }
}