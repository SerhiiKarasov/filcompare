#ifndef SRC_FCSQLITEIO_HPP_
#define SRC_FCSQLITEIO_HPP_

/**
 * @file FCSqliteConnectionHandle.cpp
 *
 * @brief a handle class for sqlite resources
 *
 * @ingroup filcomp
  *
 * @author Serhii Karasov
 * Contact: sergeyvkarasyov@gmail.com
 *
 */

#include <memory>
#include <vector>
#include <string>
#include "FCSqliteLib.hpp"
#include "FCFileInfo.hpp"

class FCSqliteIO
{
  public:
    FCFileInfo::FCFiles ReadFromDb(const std::string& db_name) const;
    bool WriteToDb(const std::string& db_name, const FCFileInfo::FCFiles& files) const;
    static void SaveToBackupToDisk(FCSqliteConnection const& source, char const* const filename);

};
#endif  //  SRC_FCSQLITEIO_HPP_