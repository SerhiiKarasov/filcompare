
#ifndef SRC_FCFILEINFO_HPP_
#define SRC_FCFILEINFO_HPP_

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
#include <functional>//for std::hash
#include <tuple>//for tuple

enum class FCFileType : char {
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
    friend std ::ostream &operator<<(std ::ostream &output, const FCFileInfo &f);
    friend bool operator<(const FCFileInfo &lhs, const FCFileInfo &rhs);
    friend bool operator==(const FCFileInfo &lhs, const FCFileInfo &rhs);
    friend bool operator>(const FCFileInfo &a, const FCFileInfo &b);
    friend bool operator<=(const FCFileInfo &a, const FCFileInfo &b);
    friend bool operator>=(const FCFileInfo &a, const FCFileInfo &b);
    friend bool operator!=(const FCFileInfo &a, const FCFileInfo &b);

    FCFileInfo(const std::string &mFilePath,
        const std::string &mFileAcls,
        const std::string &mFileCaps,
        const uint64_t mFileSize,
        const uint64_t mFileCrc,
        const uint32_t mFilePerms,
        const FCFileType mFileType,
        const uint32_t mFileOwner,
        const uint32_t mFileOwnerGroup) : fileSize{ mFileSize },
                                          fileCrc{ mFileCrc },
                                          filePerms{ mFilePerms },
                                          fileType{ mFileType },
                                          fileOwner{ mFileOwner },
                                          fileOwnerGroup{ mFileOwnerGroup },
                                          filePath{ mFilePath },
                                          fileAcls{ mFileAcls },
                                          fileCaps{ mFileCaps },
                                          filePathHash{ std::hash<std::string>{}(mFilePath) },
                                          fileCrcHash{ std::hash<std::uint64_t>{}(mFileCrc) }
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
    std::hash<std ::string>::result_type filePathHash;
    std::hash<std ::string>::result_type fileCrcHash;

    //implementation of reflect solution to iterate over class members
    auto reflect() const;

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
    FCFileInfo() = delete;

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

    FCFileInfo(const FCFileInfo &) = default;
    FCFileInfo &operator=(const FCFileInfo &) = default;
    FCFileInfo(FCFileInfo &&) = default;
    FCFileInfo &operator=(FCFileInfo &&) = default;
};
#endif// SRC_FCFILEINFO_HPP_
