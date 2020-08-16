
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
#include <functional>  //for std::hash
#include <tuple>       //for tuple
#include <vector>

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

    FCFileInfo(const std::string &t_FilePath,
        const std::string &t_FileAcls,
        const std::string &t_FileCaps,
        const uint64_t t_FileSize,
        const uint64_t t_FileCrc,
        const uint32_t t_FilePerms,
        const FCFileType t_FileType,
        const uint32_t t_FileOwner,
        const uint32_t t_FileOwnerGroup) : fileSize{ t_FileSize },
                                           fileCrc{ t_FileCrc },
                                           filePerms{ t_FilePerms },
                                           fileType{ t_FileType },
                                           fileOwner{ t_FileOwner },
                                           fileOwnerGroup{ t_FileOwnerGroup },
                                           filePath{ t_FilePath },
                                           fileAcls{ t_FileAcls },
                                           fileCaps{ t_FileCaps },
                                           filePathHash{ std::hash<std::string>{}(t_FilePath) },
                                           fileCrcHash{ std::hash<std::uint64_t>{}(t_FileCrc) }
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
    std::size_t filePathHash;
    std::size_t fileCrcHash;

    //implementation of reflect solution to iterate over class t_embers
    auto reflect() const;

  public:
    using FCFiles = std::vector<FCFileInfo>;
    class FCFileInfoFactory
    {
      public:
        static FCFileInfo constructFCFileInfoFromFs(const std::string &fileName);
        static FCFileInfo constructFCFileInfo(const std::string &t_FilePath,
            const std::string &t_FileAcls,
            const std::string &t_FileCaps,
            const uint64_t t_FileSize,
            const uint64_t t_FileCrc,
            const uint32_t t_FilePerms,
            const FCFileType t_FileType,
            const uint32_t t_FileOwner,
            const uint32_t t_FileOwnerGroup);
        static FCFileInfo constructFCFileInfo(const std::string &t_FilePath,
            const std::string &t_FileAcls,
            const std::string &t_FileCaps,
            const uint64_t t_FileSize,
            const uint64_t t_FileCrc,
            const uint32_t t_FilePerms,
            const char t_FileType,
            const uint32_t t_FileOwner,
            const uint32_t t_FileOwnerGroup);
    };
    virtual ~FCFileInfo() = default;
    FCFileInfo() = delete;

    uint64_t getFileCrc() const noexcept;
    uint64_t getFileSize() const noexcept;
    uint32_t getFilePerms() const noexcept;
    FCFileType getFileType() const noexcept;
    char getFileTypeChar() const noexcept;
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

template<typename E>
constexpr auto to_integral_type(E e) -> typename std::underlying_type<E>::type
{
    return static_cast<typename std::underlying_type<E>::type>(e);
}


#endif  // SRC_FCFILEINFO_HPP_
