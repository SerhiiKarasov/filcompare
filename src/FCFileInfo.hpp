#pragma once

/**
 * @file FCFileInfo.hpp
 *
 * @brief class to store info on files
 *
 * @ingroup filcomp
  *
 * @author Serhii Karasov
 * Contact: sergeyvkarasyov@gmail.com
 *
 */

#include <string>

class FCFileInfo
{

private:
    std::string filePath;
    uint32_t fileCrc;
    std::string fileAcls;
    uint64_t fileSize;
    uint32_t filePerms;
    std::string fileCaps;
    time_t fileModTime;
    uint32_t fileType;

public:
    virtual ~FCFileInfo() = default;
    std::string getFilePath() const;
    uint32_t getFileCrc() const;
    std::string getFileAcls() const;
    uint64_t getSize() const;
    uint32_t getFilePerms() const;
    std::string getFileCaps() const;
    time_t getFileModTime() const;
    uint32_t getFileType() const;
};