#ifndef SRC_IFCSQLITESTORAGE_HPP_
#define SRC_IFCSQLITESTORAGE_HPP_
/**
 * @file IFCSqliteStorage.hpp
 *
 * @brief an interface class to read and write sqlite db
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
#include "src/FCFileInfo.hpp"

class IFCSqliteStorage
{
  public:
    virtual FCFileInfo::FCFiles ReadFromDb(const std::string& db_name) const = 0;
    virtual bool WriteToDb(const std::string& db_name, const FCFileInfo::FCFiles& files) const = 0;
    virtual ~IFCSqliteStorage() = default;
};

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
}  // namespace FCDataColumnNames
#endif  //  SRC_IFCSQLITESTORAGE_HPP_