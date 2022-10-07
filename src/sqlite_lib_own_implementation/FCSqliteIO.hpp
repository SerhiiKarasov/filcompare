#ifndef SRC_SQLITE_LIB_OWN_IMPLEMENTATION_FCSQLITEIO_HPP_
#define SRC_SQLITE_LIB_OWN_IMPLEMENTATION_FCSQLITEIO_HPP_

/**
 * @file FCSqliteIO.hpp
 *
 * @brief a class to read and write sqlite db
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
#include "src/sqlite_lib_own_implementation/FCSqliteLib.hpp"
#include "src/IFCSqliteStorage.hpp"
#include <type_traits>
class FCSqliteIO : public IFCSqliteStorage
{
  public:
    virtual FCFileInfo::FCFiles ReadFromDb(const std::string& db_name) const override;
    virtual bool WriteToDb(const std::string& db_name, const FCFileInfo::FCFiles& files) const override;
};
#endif  //  SRC_SQLITE_LIB_OWN_IMPLEMENTATION_FCSQLITEIO_HPP_