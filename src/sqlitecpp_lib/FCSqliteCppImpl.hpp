#ifndef SRC_FCSQLITECPPIMPL_HPP_
#define SRC_FCSQLITECPPIMPL_HPP_

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
    virtual FCFileInfo::FCFiles ReadFromDb(const std::string& db_name) const override;
    virtual bool WriteToDb(const std::string& db_name, const FCFileInfo::FCFiles& files) const override;
};
#endif  //  SRC_FCSQLITECPPIMPL_HPP_