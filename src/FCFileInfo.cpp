/**
 * @file FCFileInfo.cpp
 *
 * @brief class to store info on files
 *
 * @ingroup filcomp
  *
 * @author Serhii Karasov
 * Contact: sergeyvkarasyov@gmail.com
 *
 */

#include "FCFileInfo.hpp"

std::string FCFileInfo::getFilePath() const
{
    return filePath;
}
uint32_t FCFileInfo::getFileCrc() const
{
    return fileCrc;
}
std::string FCFileInfo::getFileAcls() const
{
    return fileAcls;
}
uint64_t FCFileInfo::getSize() const
{
    return fileSize;
}
uint32_t FCFileInfo::getFilePerms() const
{
    return filePerms;
}
std::string FCFileInfo::getFileCaps() const
{
    return fileCaps;
}
time_t FCFileInfo::getFileModTime() const
{
    return fileModTime;
}

uint32_t FCFileInfo::getFileType() const
{
    return fileType;
}