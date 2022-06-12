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
#include "src/FCFileInfo.hpp"

namespace FCFileInfoHelpers {
std::pair<bool, uint64_t> readCrc(const std::string &t_File) noexcept;
std::pair<bool, std::string> readAcls(const std::string &t_File) noexcept;
std::pair<bool, std::string> readCaps(const std::string &t_File) noexcept;
struct stat readFileStat(const std::string &t_File) noexcept;
std::pair<bool, FCFileType> readFileType(const struct stat &t_FileStat, const std::string &t_FileName) noexcept;
uint32_t readFileOwner(const struct stat &t_FileStat) noexcept;
uint32_t readFileOwnerGroup(const struct stat &t_FileStat) noexcept;
uint64_t readFileSize(const struct stat &t_FileStat) noexcept;
uint32_t readFilePerms(const struct stat &t_FileStat) noexcept;
bool isBlockDev(const std::string &filename) noexcept;
void createDir(const std::string &dirName) noexcept;
bool fileExists(const std::string &fileName) noexcept;
};      // namespace FCFileInfoHelpers
#endif  // SRC_FCFILEINFOHELPERS_HPP_
