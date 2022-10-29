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
#include "FCSqliteLib.hpp"
#include "../IFCSqliteStorage.hpp"
#include <type_traits>
class FCSqliteIO : public IFCSqliteStorage
{
  public:
    FCFileInfo::FCFiles ReadFromDb(const std::string& db_name) const override;
    bool WriteToDb(const std::string& db_name, const FCFileInfo::FCFiles& files) const override;
};
#endif  //  SRC_SQLITE_LIB_OWN_IMPLEMENTATION_FCSQLITEIO_HPP_