#include "FCSqliteIO.hpp"
#include <map>
#include <utility>
#include <iostream>
#include <sstream>


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

enum class FCDataColumns : uint32_t {
    file = 0,
    size = 1,
    crc = 2,
    perm = 3,
    type = 4,
    owner = 5,
    group = 6,
    acls = 7,
    caps = 8
};

namespace FCDataColumnNames {
const constexpr auto file = "name";
const constexpr auto size = "size";
const constexpr auto crc = "crc";
const constexpr auto perm = "perm";
const constexpr auto type = "type";
const constexpr auto owner = "uid";
const constexpr auto group = "guid";
const constexpr auto acls = "acls";
const constexpr auto caps = "caps";
};  // namespace FCDataColumnNames

FCFileInfo::FCFiles FCSqliteIO::ReadFromDb(const std::string& db_name) const
{
    FCFileInfo::FCFiles files{};

    try {
        FCSqliteConnection connection(db_name.c_str());
        //TODO read count and resize files
        std::stringstream select_command;
        select_command << "SELECT "
                       << FCDataColumnNames::file << " ,"
                       << FCDataColumnNames::size << " , "
                       << FCDataColumnNames::crc << " , "
                       << FCDataColumnNames::perm << " , "
                       << FCDataColumnNames::type << " , "
                       << FCDataColumnNames::owner << " , "
                       << FCDataColumnNames::group << " , "
                       << FCDataColumnNames::acls << " , "
                       << FCDataColumnNames::caps << " "
                       << " FROM files_data ;";


        // FCSqliteStatement select;
        for (const auto& row : FCSqliteStatement(connection, select_command.str().c_str())) {
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
        std::stringstream create_command;
        create_command << "CREATE TABLE files_data ( "
                       << FCDataColumnNames::file << " TEXT PRIMARY KEY NOT NULL, "
                       << FCDataColumnNames::size << " INT NOT NULL, "
                       << FCDataColumnNames::crc << " INT NOT NULL, "
                       << FCDataColumnNames::perm << " INT NOT NULL, "
                       << FCDataColumnNames::type << " INT NOT NULL, "
                       << FCDataColumnNames::owner << " INT NOT NULL, "
                       << FCDataColumnNames::group << " INT NOT NULL, "
                       << FCDataColumnNames::acls << " TEXT NOT NULL, "
                       << FCDataColumnNames::caps << " TEXT NOT NULL) "
                       << " WITHOUT ROWID; ";

        Execute(connection, create_command.str().c_str());

        std::stringstream insert_command;
        insert_command << "INSERT INTO files_data( "
                       << FCDataColumnNames::file << " , "
                       << FCDataColumnNames::size << " , "
                       << FCDataColumnNames::crc << " , "
                       << FCDataColumnNames::perm << " , "
                       << FCDataColumnNames::type << " , "
                       << FCDataColumnNames::owner << " , "
                       << FCDataColumnNames::group << " , "
                       << FCDataColumnNames::acls << " , "
                       << FCDataColumnNames::caps << " ) "
                       << "values (?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9) ;";

        FCSqliteStatement insert(connection, insert_command.str().c_str());

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


void FCSqliteIO::SaveToBackupToDisk(FCSqliteConnection const& source, char const* const filename)
{
    FCSqliteConnection target(filename);
    Backup backup(target, source);
    backup.Step();
}
