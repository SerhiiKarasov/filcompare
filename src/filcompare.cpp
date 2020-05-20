#include <iostream>
#include "FCFileInfo.hpp"
#include "Handle.hpp"
#include <sqlite3.h>


class FCSqliteHandleTraits : HandleTraits<sqlite3 *>
{
  public:
    static void Close(Type value) noexcept
    {
        assert(SQLITE_OK == sqlite3_close(value));
    }
};
using FCSqliteHandle = Handle<FCSqliteHandleTraits>;

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