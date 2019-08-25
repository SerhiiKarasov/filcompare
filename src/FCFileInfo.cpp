/**
 * @file FCFileInfo.cpp
 *
 * @brief a class to store info on files
 *
 * @ingroup filcomp
  *
 * @author Serhii Karasov
 * Contact: sergeyvkarasyov@gmail.com
 *
 */

#include "FCFileInfo.hpp"
#include "FCFileInfoHelpers.hpp"
#include <tuple>



std::string FCFileInfo::getFilePath() const noexcept
{
    return filePath;
}
constexpr uint64_t FCFileInfo::getFileCrc() const noexcept
{
    return fileCrc;
}
std::string FCFileInfo::getFileAcls() const noexcept
{
    return fileAcls;
}
constexpr uint64_t FCFileInfo::getSize() const noexcept
{
    return fileSize;
}
constexpr uint32_t FCFileInfo::getFilePerms() const noexcept
{
    return filePerms;
}
std::string FCFileInfo::getFileCaps() const noexcept
{
    return fileCaps;
}

constexpr FCFileType FCFileInfo::getFileType() const noexcept
{
    return fileType;
}

constexpr uint32_t FCFileInfo::getFileOwner() const noexcept
{
    return fileOwner;
}

constexpr uint32_t FCFileInfo::getFileOwnerGroup() const noexcept
{
    return fileOwnerGroup;
}

FCFileInfo FCFileInfo::FCFileInfoFactory::constructFCFileInfo(const std::string &fileName)
{

    struct stat fileStat;
    bool isReadFileStatOk{false};
    std::tie(isReadFileStatOk, fileStat) = FCFileInfoHelpers::readFileStat(fileName);
    if (!isReadFileStatOk)
    {
        throw std::runtime_error("Failed to read file type of file.");
    }

    uint32_t fileOwner = FCFileInfoHelpers::readFileOwner(fileStat);
    uint32_t fileOwnerGroup = FCFileInfoHelpers::readFileOwnerGroup(fileStat);
    uint64_t fileSize = FCFileInfoHelpers::readFileSize(fileStat);
    uint32_t filePerms = FCFileInfoHelpers::readFilePerms(fileStat);

    std::string fileAcls;
    bool isReadAclsOk{false};
    std::tie(isReadAclsOk, fileAcls) = FCFileInfoHelpers::readAcls(fileName);
    if (!isReadAclsOk)
    {
        throw std::runtime_error("Failed to read acls for file.");
    }

    std::string fileCaps;
    bool isReadCapsOk{false};
    std::tie(isReadCapsOk, fileCaps) = FCFileInfoHelpers::readCaps(fileName);
    if (!isReadCapsOk)
    {
        throw std::runtime_error("Failed to read caps for file.");
    }
    FCFileType fileType{FCFileType::ERR};
    bool isReadFileTypeOk{false};
    std::tie(isReadFileTypeOk, fileType) = FCFileInfoHelpers::readFileType(fileStat, fileName);
    if (!isReadFileTypeOk)
    {
        throw std::runtime_error("Failed to read type for file.");
    }
    // std::pair<bool, uint64_t> readCrc(const std::string &file) noexcept;
    uint64_t fileCrc{0};
    bool isReadFileCrcOk{false};
    std::tie(isReadFileCrcOk, fileCrc) = FCFileInfoHelpers::readCrc(fileName);
    if (!isReadFileCrcOk)
    {
        throw std::runtime_error("Failed to read crc of file.");
    }

    FCFileInfo fileInfo(fileName,
                        fileAcls,
                        fileCaps,
                        fileSize,
                        fileCrc,
                        filePerms,
                        fileType,
                        fileOwner,
                        fileOwnerGroup);
    return fileInfo;
}
