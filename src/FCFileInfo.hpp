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
#include <ostream>

enum class FCFileType : char
{
    REG_FILE = 'r',
    DIR = 'd',
    CHAR_DEVICE = 'c',
    BLOCK_DEVICE = 'b',
    FIFO = 'f',
    LINK = 'l',
    SOCKET = 's',
    ERR = '?'

};

inline std::ostream &operator<<(std::ostream &os, const FCFileType &type)
{
    return os << static_cast<std::underlying_type<FCFileType>::type>(type);
}

class FCFileInfoFactory;

class FCFileInfo
{

private:
    friend class FCFileInfoFactory;
    FCFileInfo(const std::string &mFilePath,
               const std::string &mFileAcls,
               const std::string &mFileCaps,
               const uint64_t mFileSize,
               const uint64_t mFileCrc,
               const uint32_t mFilePerms,
               const FCFileType mFileType,
               const uint32_t mFileOwner,
               const uint32_t mFileOwnerGroup) : fileSize{mFileSize},
                                                 fileCrc{mFileCrc},
                                                 filePerms{mFilePerms},
                                                 fileType{mFileType},
                                                 fileOwner{mFileOwner},
                                                 fileOwnerGroup{mFileOwnerGroup},
                                                 filePath{mFilePath},
                                                 fileAcls{mFileAcls},
                                                 fileCaps{mFileCaps}
    {
    }

    uint64_t fileSize;
    uint64_t fileCrc;
    uint32_t filePerms;
    FCFileType fileType;
    uint32_t fileOwner;
    uint32_t fileOwnerGroup;
    std::string filePath;
    std::string fileAcls;
    std::string fileCaps;

public:
    class FCFileInfoFactory
    {
    public:
        static FCFileInfo constructFCFileInfoFromFs(const std::string &fileName);
        static FCFileInfo constructFCFileInfo(const std::string &mFilePath,
               const std::string &mFileAcls,
               const std::string &mFileCaps,
               const uint64_t mFileSize,
               const uint64_t mFileCrc,
               const uint32_t mFilePerms,
               const FCFileType mFileType,
               const uint32_t mFileOwner,
               const uint32_t mFileOwnerGroup);
    };
    virtual ~FCFileInfo() = default;
    FCFileInfo() = default;

    uint64_t getFileCrc() const noexcept;
    uint64_t getFileSize() const noexcept;
    uint32_t getFilePerms() const noexcept;
    FCFileType getFileType() const noexcept;
    uint32_t getFileOwner() const noexcept;
    uint32_t getFileOwnerGroup() const noexcept;
    std::string getFilePath() const noexcept;
    std::string getFileAcls() const noexcept;
    std::string getFileCaps() const noexcept;

    static FCFileInfoFactory factory;
};
