#pragma once

#include "FCFileInfo.hpp"

#include <sys/stat.h>           // for stat()

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
namespace FCFileInfoHelpers
{
std::pair<bool, uint64_t> readCrc(const std::string &mFile) noexcept;
std::pair<bool, std::string> readAcls(const std::string &mFile) noexcept;
std::pair<bool, std::string> readCaps(const std::string &mFile) noexcept;
std::pair<bool, struct stat> readFileStat(const std::string &mFile) noexcept;
std::pair<bool, FCFileType> readFileType(const struct stat &mFileStat, const std::string &mFileName) noexcept;

uint32_t readFileOwner(const struct stat &mFileStat) noexcept;
uint32_t readFileOwnerGroup(const struct stat &mFileStat) noexcept;
uint64_t readFileSize(const struct stat &mFileStat) noexcept;
uint32_t readFilePerms(const struct stat &mFileStat) noexcept;

} // namespace FCFileInfoHelpers