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
#include "FCSqliteCppImpl.hpp"

#include "../log.hpp"

#include <SQLiteCpp/Database.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/Statement.h>
#include <SQLiteCpp/VariadicBind.h>

#include <fmt/core.h>
#include <tuple>

FCFileInfo::FCFiles FCSqliteCppImpl::ReadFromDb(const std::string& db_name) const
{
    fclog::info("Reading from db: {}", db_name);
    FCFileInfo::FCFiles files{};

    try {
        SQLite::Database db(db_name);
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

        fclog::debug("{}", select_command);
        SQLite::Statement query(db, select_command);
        // FCSqliteStatement select;
        while (query.executeStep()) {
            files.push_back(FCFileInfo::FCFileInfoFactory::constructFCFileInfo(
                query.getColumn(to_integral_type(FCDataColumns::file)),
                query.getColumn(to_integral_type(FCDataColumns::acls)),
                query.getColumn(to_integral_type(FCDataColumns::caps)),
                static_cast<uint64_t>(query.getColumn(to_integral_type(FCDataColumns::size)).getInt64()),
                static_cast<uint64_t>(query.getColumn(to_integral_type(FCDataColumns::crc)).getInt64()),
                static_cast<uint64_t>(query.getColumn(to_integral_type(FCDataColumns::perm)).getInt64()),
                static_cast<FCFileType>(static_cast<uint32_t>(query.getColumn(to_integral_type(FCDataColumns::type)))),
                query.getColumn(to_integral_type(FCDataColumns::owner)),
                query.getColumn(to_integral_type(FCDataColumns::group))));
        }

    } catch (const std::exception& e) {
        fclog::error("Exception happened while reading from {}: '{}'", db_name, e.what());
        return files;
    }
    return files;
}

bool FCSqliteCppImpl::WriteToDb(const std::string& db_name, const FCFileInfo::FCFiles& files) const
{
    try {
        static constexpr const auto create_cmd_format = R"(
            CREATE TABLE files_data (
                {} TEXT PRIMARY KEY NOT NULL,
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

        fclog::debug("{}", create_command);
        SQLite::Database db(db_name, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        fclog::debug("{} constructor!", db_name);
        SQLite::Transaction transaction(db);
        fclog::debug("{} transaction creation!", db_name);
        // create the db
        db.exec(create_command);
        fclog::debug("{} created!", db_name);

        for (const auto& file : files) {
            fclog::debug("{}", file);

            const auto insert_command = fmt::format(
                R"(INSERT INTO files_data ({}, {}, {}, {}, {}, {}, {}, {}, {}) values ("{}", "{}", "{}", "{}", "{}", "{}", "{}", "{}", "{}");)",
                FCDataColumnNames::file,
                FCDataColumnNames::size,
                FCDataColumnNames::crc,
                FCDataColumnNames::perm,
                FCDataColumnNames::type,
                FCDataColumnNames::owner,
                FCDataColumnNames::group,
                FCDataColumnNames::acls,
                FCDataColumnNames::caps,
                file.getFilePath(),
                file.getFileSize(),
                file.getFileCrc(),
                file.getFilePerms(),
                file.getFileTypeChar(),
                file.getFileOwner(),
                file.getFileOwnerGroup(),
                file.getFileAcls(),
                file.getFileCaps());

            fclog::debug("{}", insert_command);

            SQLite::Statement query(db, insert_command);
            query.exec();
            query.reset();
        }
        transaction.commit();


    } catch (const std::exception& e) {
        fclog::error("Exception happened while writing to {}: {}", db_name, e.what());
        return false;
    }
    return true;
}
