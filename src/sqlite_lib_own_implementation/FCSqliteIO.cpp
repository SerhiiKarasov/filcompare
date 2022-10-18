#include "src/sqlite_lib_own_implementation/FCSqliteIO.hpp"

#include <fmt/core.h>

#include <map>
#include <utility>
#include <iostream>

/**
 * @file FCSqliteIO.cpp
 *
 * @brief a class to read and write sqlite db
 *
 * @ingroup filcomp
  *
 * @author Serhii Karasov
 * Contact: sergeyvkarasyov@gmail.com
 *
 */

// static char const* TypeName(FCSQLITE_TYPE const type)
// {
//     switch (type) {
//     case FCSQLITE_TYPE::Integer:
//         return "int";
//     case FCSQLITE_TYPE::Text:
//         return "txt";
//     case FCSQLITE_TYPE::Float:
//         return "float";
//     case FCSQLITE_TYPE::Blob:
//         return "blob";
//     case FCSQLITE_TYPE::Null:
//         return "null";
//     }
//     assert(false);
//     return "invalid";
// }



FCFileInfo::FCFiles FCSqliteIO::ReadFromDb(const std::string& db_name) const
{
    FCFileInfo::FCFiles files{};

    try {
        FCSqliteConnection connection(db_name.c_str());
        // TODO read count and resize files
        const auto select_command = fmt::format("SELECT {}, {}, {}, {}, {}, {}, {}, {}, {} FROM files_data;",
            FCDataColumnNames::file,
            FCDataColumnNames::size,
            FCDataColumnNames::crc,
            FCDataColumnNames::perm,
            FCDataColumnNames::type,
            FCDataColumnNames::owner,
            FCDataColumnNames::group,
            FCDataColumnNames::acls,
            FCDataColumnNames::caps);


        // FCSqliteStatement select;
        for (const auto& row : FCSqliteStatement(connection, select_command.c_str())) {
            files.push_back(FCFileInfo::FCFileInfoFactory::constructFCFileInfo(
                row.GetString(to_integral_type(FCDataColumns::file)),
                row.GetString(to_integral_type(FCDataColumns::acls)),
                row.GetString(to_integral_type(FCDataColumns::caps)),
                row.GetInt(to_integral_type(FCDataColumns::size)),
                row.GetInt(to_integral_type(FCDataColumns::crc)),
                row.GetInt(to_integral_type(FCDataColumns::perm)),
                static_cast<FCFileType>(row.GetInt(to_integral_type(FCDataColumns::type))),
                row.GetInt(to_integral_type(FCDataColumns::owner)),
                row.GetInt(to_integral_type(FCDataColumns::group))));
        }

    } catch (const FCSqliteException& ex) {
        std::cerr << "Db read failed. Message: " << ex.Message << ". Error : " << ex.Result << std::endl;
    }
    return files;
}

bool FCSqliteIO::WriteToDb(const std::string& db_name, const FCFileInfo::FCFiles& files) const
{
    try {
        FCSqliteConnection connection(db_name.c_str());
        static constexpr const auto create_cmd_format = R"(
            CREATE TABLE files_data (
                {} TEXT PRIMARY KEY NOT NUL,
                {} INT NOT NULL,
                {} INT NOT NULL,
                {} INT NOT NULL,
                {} INT NOT NULL,
                {} INT NOT NULL,
                {} INT NOT NULL,
                {} TEXT NOT NULL,
                {} TEXT NOT NULL
            ) WITHOUT ROWID;
        )";
        const auto create_command = fmt::format(create_cmd_format,
            FCDataColumnNames::file,
            FCDataColumnNames::size,
            FCDataColumnNames::crc,
            FCDataColumnNames::perm,
            FCDataColumnNames::type,
            FCDataColumnNames::owner,
            FCDataColumnNames::group,
            FCDataColumnNames::acls,
            FCDataColumnNames::caps);

        Execute(connection, create_command.c_str());

        const auto insert_command = fmt::format(
            "INSERT INTO files_data ({}, {}, {}, {}, {}, {}, {}, {}, {}) values (?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9);",
            FCDataColumnNames::file,
            FCDataColumnNames::size,
            FCDataColumnNames::crc,
            FCDataColumnNames::perm,
            FCDataColumnNames::type,
            FCDataColumnNames::owner,
            FCDataColumnNames::group,
            FCDataColumnNames::acls,
            FCDataColumnNames::caps);


        FCSqliteStatement insert(connection, insert_command.c_str());

        Execute(connection, "begin");
        for (const auto& file : files) {
            insert.Reset(
                file.getFilePath().c_str(),
                file.getFileSize(),
                file.getFileCrc(),
                file.getFilePerms(),
                file.getFileTypeChar(),
                file.getFileOwner(),
                file.getFileOwnerGroup(),
                file.getFileAcls().c_str(),
                file.getFileCaps().c_str());
            insert.Execute();
        }
        Execute(connection, "commit");
    } catch (const FCSqliteException& ex) {
        std::cerr << "Db write failed. Message: " << ex.Message << ". Error : " << ex.Result << std::endl;
    }
    return true;
}