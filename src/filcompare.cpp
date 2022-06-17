#include <iostream>
#include <string>
#include "src/FCFileInfo.hpp"
#include "src/sqlite_lib_own_implementation/FCSqliteIO.hpp"
#include "src/sqlitecpp_lib/FCSqliteCppImpl.hpp"

#include <stdarg.h> /* Needed for the definition of va_list */

int main()
{
    FCFileInfo file1 = FCFileInfo::factory.constructFCFileInfoFromFs("/tmp/file1");
    FCFileInfo file2 = FCFileInfo::factory.constructFCFileInfoFromFs("/tmp/file2");
    FCFileInfo::FCFiles files{ file1, file2 };
    std::unique_ptr<IFCSqliteStorage> sqlitewriter = std::make_unique<FCSqliteCppImpl>();
    ;
    sqlitewriter->WriteToDb("/tmp/mydb.db", files);
    for (const auto &file_read : sqlitewriter->ReadFromDb("/tmp/mydb2.db")) {
        std::cout << "we read " << file_read << std::endl;
    }
}