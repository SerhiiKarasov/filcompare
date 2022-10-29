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

#include <tuple>
#include "FCFileInfoHelpers.hpp"
#include "FCFileInfo.hpp"


static auto convertCharToType(const char type) -> FCFileType
{
    switch (type) {
    case 'r':
        return FCFileType::REG_FILE;
    case 'd':
        return FCFileType::DIR;
    case 'c':
        return FCFileType::CHAR_DEVICE;
    case 'b':
        return FCFileType::BLOCK_DEVICE;
    case 'f':
        return FCFileType::FIFO;
    case 'l':
        return FCFileType::LINK;
    case 's':
        return FCFileType::SOCKET;
    case '?':
        return FCFileType::ERR;
    default:
        throw("failed to convert char to FCFileType");
    }
}

auto FCFileInfo::getFilePath() const noexcept -> std::string
{
    return filePath;
}
auto FCFileInfo::getFileCrc() const noexcept -> uint64_t
{
    return fileCrc;
}
auto FCFileInfo::getFileAcls() const noexcept -> std::string
{
    return fileAcls;
}
auto FCFileInfo::getFileSize() const noexcept -> uint64_t
{
    return fileSize;
}
auto FCFileInfo::getFilePerms() const noexcept -> uint32_t
{
    return filePerms;
}
auto FCFileInfo::getFileCaps() const noexcept -> std::string
{
    return fileCaps;
}

auto FCFileInfo::getFileType() const noexcept -> FCFileType
{
    return fileType;
}

auto FCFileInfo::getFileOwner() const noexcept -> uint32_t
{
    return fileOwner;
}

auto FCFileInfo::getFileOwnerGroup() const noexcept -> uint32_t
{
    return fileOwnerGroup;
}

auto FCFileInfo::reflect() const
{
    return std::tie(filePath, fileSize, fileCrc, filePerms, fileType, fileOwner, fileOwnerGroup, fileAcls, fileCaps, filePathHash, fileCrcHash);
}

FCFileInfo FCFileInfo::FCFileInfoFactory::constructFCFileInfoFromFs(const std::string &fileName)
{

    struct stat fileStat
    {
    };
    fileStat = FCFileInfoHelpers::readFileStat(fileName);

    uint32_t fileOwner = FCFileInfoHelpers::readFileOwner(fileStat);
    uint32_t fileOwnerGroup = FCFileInfoHelpers::readFileOwnerGroup(fileStat);
    uint64_t fileSize = FCFileInfoHelpers::readFileSize(fileStat);
    uint32_t filePerms = FCFileInfoHelpers::readFilePerms(fileStat);

    const auto [isReadAclsOk, fileAcls] = FCFileInfoHelpers::readAcls(fileName);
    if (!isReadAclsOk) {
        throw std::runtime_error("Failed to read acls for file.");
    }

    const auto [isReadCapsOk, fileCaps] = FCFileInfoHelpers::readCaps(fileName);
    if (!isReadCapsOk) {
        throw std::runtime_error("Failed to read caps for file.");
    }

    const auto [isReadFileTypeOk, fileType] = FCFileInfoHelpers::readFileType(fileStat, fileName);
    if (!isReadFileTypeOk) {
        throw std::runtime_error("Failed to read type for file.");
    }

    const auto [isReadFileCrcOk, fileCrc] = FCFileInfoHelpers::readCrc(fileName);
    if (!isReadFileCrcOk) {
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

FCFileInfo FCFileInfo::FCFileInfoFactory::constructFCFileInfo(const std::string &t_FilePath,
    const std::string &t_FileAcls,
    const std::string &t_FileCaps,
    const uint64_t t_FileSize,
    const uint64_t t_FileCrc,
    const uint32_t t_FilePerms,
    const FCFileType t_FileType,
    const uint32_t t_FileOwner,
    const uint32_t t_FileOwnerGroup)
{
    return FCFileInfo{ t_FilePath,
        t_FileAcls,
        t_FileCaps,
        t_FileSize,
        t_FileCrc,
        t_FilePerms,
        t_FileType,
        t_FileOwner,
        t_FileOwnerGroup };
}


FCFileInfo FCFileInfo::FCFileInfoFactory::constructFCFileInfo(const std::string &t_FilePath,
    const std::string &t_FileAcls,
    const std::string &t_FileCaps,
    const uint64_t t_FileSize,
    const uint64_t t_FileCrc,
    const uint32_t t_FilePerms,
    const char t_FileType,
    const uint32_t t_FileOwner,
    const uint32_t t_FileOwnerGroup)
{
    return FCFileInfo{ t_FilePath,
        t_FileAcls,
        t_FileCaps,
        t_FileSize,
        t_FileCrc,
        t_FilePerms,
        convertCharToType(t_FileType),
        t_FileOwner,
        t_FileOwnerGroup };
}

bool operator==(const FCFileInfo &lhs, const FCFileInfo &rhs)
{
    bool result = (lhs.fileCrcHash == rhs.fileCrcHash);
    result = result && (lhs.filePathHash == rhs.filePathHash);
    return result && (lhs.fileSize == rhs.fileSize);
}

bool operator<(const FCFileInfo &lhs, const FCFileInfo &rhs)
{
    if (lhs.fileCrcHash == rhs.fileCrcHash) {
        if (lhs.filePathHash == rhs.filePathHash) {
            return lhs.fileSize < rhs.fileSize;
        }
        return lhs.filePathHash < rhs.filePathHash;
    }
    return lhs.fileCrcHash < rhs.fileCrcHash;
}

std::ostream &operator<<(std::ostream &output, const FCFileInfo &f)
{
    std::apply([&output](const auto &... fields) {
        ((output << fields << " "), ...);
    },
        f.reflect());
    return output;
}

bool operator>(const FCFileInfo &a, const FCFileInfo &b)
{
    return (b < a);
}
bool operator<=(const FCFileInfo &a, const FCFileInfo &b)
{
    return !(a > b);
}
bool operator>=(const FCFileInfo &a, const FCFileInfo &b)
{
    return !(a < b);
}
bool operator!=(const FCFileInfo &a, const FCFileInfo &b)
{
    return !(a == b);
}

char FCFileInfo::getFileTypeChar() const noexcept
{
    return to_integral_type(getFileType());
}