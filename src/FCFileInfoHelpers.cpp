
#include <sys/stat.h>           // for struct stat
#include <sys/acl.h>            // for acl_get_file
#include <sys/capability.h>     // for cap_get_file()
#include <zlib.h>               // for crc32()
#include <fstream>              // for ifstream
#include <iostream>             // for cerr
#include <boost/filesystem.hpp> // for boost::filestystem
#include "FCFileInfoHelpers.hpp"

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

std::streamsize const kBufferSize = 4096;

std::pair<bool, uint64_t> FCFileInfoHelpers::readCrc(const std::string &mFile) noexcept
{
    uint64_t crc{0};
    bool result{true};
    try
    {
        std::ifstream ifs(mFile, std::ios_base::binary);
        ifs.seekg(0, std::ios::beg);
        if (ifs)
        {
            do
            {
                char buffer[kBufferSize];
                ifs.read(buffer, kBufferSize);
                crc = crc32(crc, reinterpret_cast<const Bytef *>(&buffer), ifs.gcount());
            } while (ifs);
        }
        else
        {
            std::cerr << "Problem with opening the file: " << mFile << "'." << std::endl;
            throw std::runtime_error("Failed to open file.");
        }
    }
    catch (std::exception &e)
    {
        std::cout << "Exception was caught during calculating of crc for file: " << mFile << ". Message: " << e.what() << '\n';
        result = false;
    }
    catch (...)
    {
        std::cerr << "Caught an unknown exception during calculation crc for file: " << mFile << std::endl;
        result = false;
    }
    return std::pair<bool, uint64_t>(result, crc);
}

std::pair<bool, std::string> FCFileInfoHelpers::readAcls(const std::string &mFile) noexcept
{
    std::string aclstring{};
    bool result{true};
    ssize_t fileAclSize{};
    try
    {
        acl_t fileAcl = acl_get_file(mFile.c_str(), ACL_TYPE_ACCESS);
        if (nullptr == fileAcl)
        {
            std::cerr << "Failed to get ACL for file: " << mFile << "Errno= " << errno << std::endl;
            result = false;
        }
        fileAclSize = acl_size(fileAcl);
        aclstring = std::string(acl_to_text(fileAcl, &fileAclSize));
        acl_free(fileAcl);
        aclstring.erase(std::remove_if(aclstring.begin(), aclstring.end(), ::isspace), aclstring.end());
    }
    catch (std::exception &e)
    {
        std::cout << "Exception was caught during reading acls for file: " << mFile << ". Message: " << e.what() << '\n';
        result = false;
    }
    catch (...)
    {
        std::cerr << "Caught an unknown exception during reading acls for file: " << mFile << std::endl;
        result = false;
    }
    return std::pair<bool, std::string>{result, aclstring};
}

std::pair<bool, std::string> FCFileInfoHelpers::readCaps(const std::string &mFile) noexcept
{
    bool result{true};
    std::string CAPS{};
    try
    {
        cap_t fileCaps = cap_get_file(mFile.c_str());

        if (fileCaps)
        {
            const char *txt_caps = cap_to_text(fileCaps, nullptr);
            cap_free(fileCaps);
            if (!txt_caps)
            {
                std::cout << "Problem while running cap_to_text()." << mFile << std::endl;
                result = false;
            }
            CAPS = std::string(txt_caps);
        }
    }
    catch (std::exception &e)
    {
        std::cout << "Exception was caught during reading caps for file: " << mFile << ". Message: " << e.what() << '\n';
        result = false;
    }
    catch (...)
    {
        std::cerr << "Caught an unknown exception during reading of caps for file: " << mFile << std::endl;
        result = false;
    }
    return std::pair<bool, std::string>(result, CAPS);
}

struct stat FCFileInfoHelpers::readFileStat(const std::string &mFile) noexcept
{
    struct stat fileattrib{};
    //lstat() is identical to stat(), except that if path is a symbolic link, then the link itself is stat-ed, not the file that it refers to.
    if (-1 == lstat(mFile.c_str(), &fileattrib))
    {
        std::cout << "Problem while running stat() on " << mFile << std::endl;
    }
    return fileattrib;
}

uint64_t FCFileInfoHelpers::readFileSize(const struct stat &mFileStat) noexcept
{
    return mFileStat.st_size;
}

uint32_t FCFileInfoHelpers::readFilePerms(const struct stat &mFileStat) noexcept
{
    return mFileStat.st_mode;
}

std::pair<bool, FCFileType> FCFileInfoHelpers::readFileType(const struct stat &mFileStat, const std::string &mFileName) noexcept
{
    bool result{true};
    FCFileType fileType{FCFileType::ERR};

    try
    {
        switch (mFileStat.st_mode & S_IFMT)
        {
        case S_IFBLK:
            fileType = FCFileType::BLOCK_DEVICE;
            break;
        case S_IFCHR:
            fileType = FCFileType::CHAR_DEVICE;
            break;
        case S_IFDIR:
            fileType = FCFileType::DIR;
            break;
        case S_IFIFO:
            fileType = FCFileType::FIFO;
            break;
        case S_IFLNK:
            fileType = FCFileType::LINK;
            break;
        case S_IFREG:
            fileType = FCFileType::REG_FILE;
            break;
        case S_IFSOCK:
            fileType = FCFileType::SOCKET;
            break;
        default:
            fileType = FCFileType::ERR;
            break;
        }

        if (FCFileType::ERR == fileType)
        {
            std::cout << "Problem while running checking file type on " << mFileName << std::endl;
            result = false;
        }
    }
    catch (std::exception &e)
    {
        std::cout << "Exception was caught during reading file type for file: " << mFileName << ". Message: " << e.what() << '\n';
        result = false;
    }
    catch (...)
    {
        std::cerr << "Caught an unknown exception during reading of file type for file: " << mFileName << std::endl;
        result = false;
    }

    return std::pair<bool, FCFileType>(result, fileType);
}

uint32_t FCFileInfoHelpers::readFileOwner(const struct stat &mFileStat) noexcept
{
    return mFileStat.st_uid;
}

uint32_t FCFileInfoHelpers::readFileOwnerGroup(const struct stat &mFileStat) noexcept
{
    return mFileStat.st_gid;
}