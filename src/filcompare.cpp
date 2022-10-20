#include "src/log.hpp"
#include "src/FCFileInfo.hpp"
#include "src/sqlite_lib_own_implementation/FCSqliteIO.hpp"
#include "src/sqlitecpp_lib/FCSqliteCppImpl.hpp"

#include <string>

int main()
{
    FCFileInfo file1 = FCFileInfo::factory.constructFCFileInfoFromFs("/tmp/file1");
    FCFileInfo file2 = FCFileInfo::factory.constructFCFileInfoFromFs("/tmp/file2");
    FCFileInfo::FCFiles files{ file1, file2 };
    std::unique_ptr<IFCSqliteStorage> sqlitewriter1 = std::make_unique<FCSqliteCppImpl>();
    sqlitewriter1->WriteToDb("/tmp/mydb.db", files);
    for (const auto &file_read : sqlitewriter1->ReadFromDb("/tmp/mydb.db")) {
        fclog::info("we read {}", file_read);
    }
    std::unique_ptr<IFCSqliteStorage> sqlitewriter2 = std::make_unique<FCSqliteIO>();
    for (const auto &file_read : sqlitewriter2->ReadFromDb("/tmp/mydb.db")) {
        fclog::info("we read {}", file_read);
    }
}
