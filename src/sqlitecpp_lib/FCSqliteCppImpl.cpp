/**
 * @file FCSqliteCppImpl.cpp
 *
 * @brief a class to read and write sqlite db
 * based on https://github.com/SRombauts/SQLiteCpp.git
 *
 * @ingroup filcomp
 *
 * @author Serhii Karasov
 * Contact: sergeyvkarasyov@gmail.com
 *
 */
#include <iostream>
#include <sstream>
#include "src/sqlitecpp_lib/FCSqliteCppImpl.hpp"
#include "submodules/SQLiteCpp/include/SQLiteCpp/SQLiteCpp.h"
#include "submodules/SQLiteCpp/include/SQLiteCpp/VariadicBind.h"
#include "submodules/SQLiteCpp/include/SQLiteCpp/Database.h"
#include "submodules/SQLiteCpp/include/SQLiteCpp/Statement.h"
#include <tuple>

FCFileInfo::FCFiles FCSqliteCppImpl::ReadFromDb(const std::string& db_name) const
{
    std::cout << db_name << std::endl;
    FCFileInfo::FCFiles files{};

    try {
        SQLite::Database db(db_name);
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

        std::cout << select_command.str() << std::endl;
        SQLite::Statement query(db, select_command.str());
        // FCSqliteStatement select;
        while (query.executeStep()) {
            files.push_back(FCFileInfo::FCFileInfoFactory::constructFCFileInfo(
                query.getColumn(to_integral_type(FCDataColumns::file)),
                query.getColumn(to_integral_type(FCDataColumns::acls)),
                query.getColumn(to_integral_type(FCDataColumns::caps)),
                static_cast<uint64_t>(query.getColumn(to_integral_type(FCDataColumns::size)).getInt64()),
                static_cast<uint64_t>(query.getColumn(to_integral_type(FCDataColumns::crc)).getInt64()),
                query.getColumn(to_integral_type(FCDataColumns::perm)),
                query.getColumn(to_integral_type(FCDataColumns::type)),
                query.getColumn(to_integral_type(FCDataColumns::owner)),
                query.getColumn(to_integral_type(FCDataColumns::group))));
        }

    } catch (const std::exception& e) {
        std::cout << "exception: " << e.what() << std::endl;
        return files;
    }
    return files;
}

bool FCSqliteCppImpl::WriteToDb(const std::string& db_name, const FCFileInfo::FCFiles& files) const
{
    try {
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
        std::cout << create_command.str() << std::endl;
        SQLite::Database db(db_name, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        std::cout << db_name << " constructor!" << std::endl;
        SQLite::Transaction transaction(db);
        std::cout << db_name << " transaction creation!" << std::endl;
        // create the db
        db.exec(create_command.str());
        std::cout << db_name << " created!" << std::endl;

        std::stringstream insert_command;

        for (const auto& file : files) {
            std::cout << file << std::endl;
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
                           << "values ("
                           << "\"" << file.getFilePath() << "\""
                           << " , "
                           << "\"" << file.getFileSize() << "\""
                           << " , "
                           << "\"" << file.getFileCrc() << "\""
                           << " , "
                           << "\"" << file.getFilePerms() << "\""
                           << " , "
                           << "\"" << file.getFileTypeChar() << "\""
                           << " , "
                           << "\"" << file.getFileOwner() << "\""
                           << " , "
                           << "\"" << file.getFileOwnerGroup() << "\""
                           << " , "
                           << "\"" << file.getFileAcls() << "\""
                           << " , "
                           << "\"" << file.getFileCaps() << "\""
                           << ") ;";

            std::cout << insert_command.str() << std::endl;

            SQLite::Statement query(db, insert_command.str());
            query.exec();
            query.reset();
        }
        transaction.commit();


    } catch (const std::exception& e) {
        std::cout << "exception: " << e.what() << std::endl;
        return false;
    }
    return true;
}
