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

#include <iostream>
#include <tuple>
#include "FCFileInfoHelpers.hpp"
#include "FCFileInfo.hpp"


static FCFileType convertCharToType(const char type)
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

std::string FCFileInfo::getFilePath() const noexcept
{
    return filePath;
}
uint64_t FCFileInfo::getFileCrc() const noexcept
{
    return fileCrc;
}
std::string FCFileInfo::getFileAcls() const noexcept
{
    return fileAcls;
}
uint64_t FCFileInfo::getFileSize() const noexcept
{
    return fileSize;
}
uint32_t FCFileInfo::getFilePerms() const noexcept
{
    return filePerms;
}
std::string FCFileInfo::getFileCaps() const noexcept
{
    return fileCaps;
}

FCFileType FCFileInfo::getFileType() const noexcept
{
    return fileType;
}

uint32_t FCFileInfo::getFileOwner() const noexcept
{
    return fileOwner;
}

uint32_t FCFileInfo::getFileOwnerGroup() const noexcept
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

    std::string fileAcls;
    bool isReadAclsOk{ false };
    std::tie(isReadAclsOk, fileAcls) = FCFileInfoHelpers::readAcls(fileName);
    if (!isReadAclsOk) {
        throw std::runtime_error("Failed to read acls for file.");
    }

    std::string fileCaps;
    bool isReadCapsOk{ false };
    std::tie(isReadCapsOk, fileCaps) = FCFileInfoHelpers::readCaps(fileName);
    if (!isReadCapsOk) {
        throw std::runtime_error("Failed to read caps for file.");
    }

    FCFileType fileType{ FCFileType::ERR };
    bool isReadFileTypeOk{ false };
    std::tie(isReadFileTypeOk, fileType) = FCFileInfoHelpers::readFileType(fileStat, fileName);
    if (!isReadFileTypeOk) {
        throw std::runtime_error("Failed to read type for file.");
    }

    uint64_t fileCrc{ 0 };
    bool isReadFileCrcOk{ false };
    std::tie(isReadFileCrcOk, fileCrc) = FCFileInfoHelpers::readCrc(fileName);
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

template<size_t Index, typename TupleType, typename Functor>
auto tuple_at(const TupleType &tpl, const Functor &func) -> void
{
    const auto &val = std::get<Index>(tpl);
    func(val);
}

template<typename TupleType, typename Functor, size_t Index = 0>
auto tuple_for_each(const TupleType &tpl, const Functor &ifunctor) -> void
{
    constexpr auto tuple_size = std::tuple_size_v<TupleType>;
    if constexpr (Index < tuple_size) {
        tuple_at<Index>(tpl, ifunctor);
        tuple_for_each<TupleType, Functor, Index + 1>(tpl, ifunctor);
    }
}

std::ostream &operator<<(std::ostream &output, const FCFileInfo &f)
{
    tuple_for_each(f.reflect(), [&output](const auto &field) {
        output << field << " ";
    });
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