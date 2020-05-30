#ifndef SRC_FCFILEINFOHELPERS_HPP_
#define SRC_FCFILEINFOHELPERS_HPP_

/**
 * @file FCFileInfoHelpers.hpp
 *
 * @brief a set of functions to get info on files from filesystem(calculate crc, get size, etc)
 *
 * @ingroup filcomp
  *
 * @author Serhii Karasov
 * Contact: sergeyvkarasyov@gmail.com
 *
 */

#include <sys/stat.h>  // for stat()
#include <utility>     // for pair<>
#include <string>      // for string
#include "FCFileInfo.hpp"

namespace FCFileInfoHelpers {
std::pair<bool, uint64_t> readCrc(const std::string &mFile) noexcept;
std::pair<bool, std::string> readAcls(const std::string &mFile) noexcept;
std::pair<bool, std::string> readCaps(const std::string &mFile) noexcept;
struct stat readFileStat(const std::string &mFile) noexcept;
std::pair<bool, FCFileType> readFileType(const struct stat &mFileStat, const std::string &mFileName) noexcept;
uint32_t readFileOwner(const struct stat &mFileStat) noexcept;
uint32_t readFileOwnerGroup(const struct stat &mFileStat) noexcept;
uint64_t readFileSize(const struct stat &mFileStat) noexcept;
uint32_t readFilePerms(const struct stat &mFileStat) noexcept;
bool isBlockDev(const std::string &filename) noexcept;
void createDir(const std::string &dirName) noexcept;
bool fileExists(const std::string &fileName) noexcept;
};  // namespace FCFileInfoHelpers
#endif  // SRC_FCFILEINFOHELPERS_HPP_
