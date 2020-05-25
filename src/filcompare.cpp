#include <sqlite3.h>
#include <iostream>
#include <string>
#include "FCFileInfo.hpp"
#include "FCSqliteLib.hpp"


static void SaveToDisk(FCSqliteConnection const& source, char const* const filename)
{
    FCSqliteConnection target(filename);
    Backup backup(target, source);
    backup.Step();
}

static char const* TypeName(FCSQLITE_TYPE const type)
{
    switch (type) {
    case FCSQLITE_TYPE::Integer:
        return "int";
    case FCSQLITE_TYPE::Text:
        return "txt";
    case FCSQLITE_TYPE::Float:
        return "float";
    case FCSQLITE_TYPE::Blob:
        return "blob";
    case FCSQLITE_TYPE::Null:
        return "null";
    }
    assert(false);
    return "invalid";
}

int main()
{
    try {
        FCSqliteConnection connection = FCSqliteConnection::Memory();
        FCSqliteStatement statement;
        std::string hello = "hello";
        std::wstring world = L"world";
        statement.Prepare(connection, "select ?1 union all select ?2;", "hello", "world");
        // statement.Bind(1, std::string("Hello"));
        // statement.Bind(2, std::wstring(L"world"));
        // statement.Bind(1, hello);
        // statement.Bind(2, world);
        statement.BindAll(std::string("hello"), world);
        while (statement.Step()) {
            // const auto* w = statement.GetWideString(0);
            // const int wlength = statement.GetWideStringLength(0);
            const auto* s = statement.GetString(0);
            const int length = statement.GetStringLength(0);
            std::cout << s << " " << length << " " << /* w << " " << wlength <<*/ std::endl;
        }
        for (const auto& row : statement) {
            std::cout << row.GetString(0);
        }
        std::cout << std::endl;


        // FCSqliteConnection connection1 = FCSqliteConnection::Memory();
        // Execute(connection1, "create table Users (Name)");
        // Execute(connection1, "insert into Users values (?)", "Joe");
        // Execute(connection1, "insert into Users values (?)", "Doe");

        // for(const auto & row : FCSqliteStatement(connection1, "select Name from Users"))
        // {
        //     std::cout << row.GetString(0) << std::endl;
        // }

        FCSqliteConnection connection2 = FCSqliteConnection::Memory();
        Execute(connection2, "create table Things (Content real)");  // can be text
        Execute(connection2, "insert into Things values (?)", "AAA");
        std::cout << "get row id " << connection2.RowId() << std::endl;

        Execute(connection2, "insert into Things values (?)", 111);
        std::cout << "get row id " << connection2.RowId() << std::endl;

        for (const auto& row : FCSqliteStatement(connection2, "select Content from Things")) {
            std::cout << row.GetString(0) << TypeName(row.GetType(0)) << std::endl;
        }

        FCSqliteConnection connection3 = FCSqliteConnection::Memory();
        Execute(connection3, "create table Hens (Name primary key) without rowid");
        Execute(connection3, "insert into Hens (Name) values (?)", "VVV");
        Execute(connection3, "insert into Hens (Name) values (?)", "VBV");
        std::cout << "get row id " << connection3.RowId() << std::endl;

        for (const auto& row : FCSqliteStatement(connection3, "select Name from Hens")) {
            std::cout << row.GetString(0) << std::endl;
        }

        // FCSqliteConnection connection4 = FCSqliteConnection::Memory();
        FCSqliteConnection connection4{ "/tmp/file" };
        Execute(connection4, "create table Atata(Content)");
        FCSqliteStatement insert(connection4, "insert into Atata (Content) values (?1)");
        Execute(connection4, "begin");
        for (auto i = 0; i != 1000; i++) {
            insert.Reset(i);
            insert.Execute();
        }
        Execute(connection4, "commit");


        FCSqliteConnection connection5 = FCSqliteConnection::Memory();
        connection.Profile([](void*, char const* const statement, unsigned long long time)  //NOLINT
            {
                const uint64_t ms = time / 1000000;
                if (ms > 10) {
                    std::cout << "Profiler: " << ms << " " << statement << std::endl;
                }
            });
        Execute(connection5, "create table Atata(Content)");
        FCSqliteStatement insert5(connection5, "insert into Atata (Content) values (?1)");
        Execute(connection5, "begin");
        for (auto i = 0; i != 1000; i++) {
            insert5.Reset(i);
            insert5.Execute();
        }
        Execute(connection5, "commit");

        FCSqliteConnection connection6 = FCSqliteConnection::Memory();
        Execute(connection6, "create table Ururu(Content real)");
        FCSqliteStatement st6(connection6, "insert into Ururu (Content) values (?)");
        Execute(connection6, "begin");
        for (auto i = 0; i != 1009999; i++) {
            st6.Reset(i);
            st6.Execute();
        }
        Execute(connection6, "commit");
        Execute(connection6, "vacuum");
        SaveToDisk(connection6, "/tmp/databasaaaaaaa.db");
    } catch (const FCSqliteException& ex) {
        std::cout << "Db open failed. Message: " << ex.Message << ". Error : " << ex.Result << std::endl;
    }
    FCFileInfo file = FCFileInfo::factory.constructFCFileInfoFromFs("/tmp/file1");
    std::cout << file << std::endl;
}