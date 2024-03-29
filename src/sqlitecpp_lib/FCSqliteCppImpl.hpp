#ifndef SRC_SQLITECPP_LIB_FCSQLITECPPIMPL_HPP_
#define SRC_SQLITECPP_LIB_FCSQLITECPPIMPL_HPP_

/**
 * @file FCSqliteCppImpl.hpp
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

#include <memory>
#include <vector>
#include <string>
#include "src/IFCSqliteStorage.hpp"

class FCSqliteCppImpl : public IFCSqliteStorage
{
  public:
    FCFileInfo::FCFiles ReadFromDb(const std::string& db_name) const override;
    bool WriteToDb(const std::string& db_name, const FCFileInfo::FCFiles& files) const override;
};
#endif  //  SRC_SQLITECPP_LIB_FCSQLITECPPIMPL_HPP_