/**
 * @file FCFileInfoHelpers.cpp
 *
 * @brief a set of functions to get info on files from filesystem(calculate crc, get size, etc)
 *
 * @ingroup filcomp
 *
 * @author Serhii Karasov
 * Contact: sergeyvkarasyov@gmail.com
 *
 */

#include "src/FCFileInfoHelpers.hpp"

#include "src/log.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>           // for ifstream
#include <sys/acl.h>         // for acl_get_file
#include <sys/capability.h>  // for cap_get_file()
#include <sys/stat.h>        // for struct stat
#include <zlib.h>            // for crc32()

std::streamsize const kBufferSize = 4096;

std::pair<bool, uint64_t> FCFileInfoHelpers::readCrc(const std::string &t_File) noexcept
{
    uint64_t crc{ 0 };
    bool result{ true };
    try {
        std::ifstream ifs(t_File, std::ios_base::binary);
        ifs.seekg(0, std::ios::beg);
        if (ifs) {
            do {
                char buffer[kBufferSize];
                ifs.read(buffer, kBufferSize);
                crc = crc32(crc, reinterpret_cast<const Bytef *>(&buffer), ifs.gcount());
            } while (ifs);
        } else {
            fclog::error("Problem with opening the file: {}", t_File);
            throw std::runtime_error("Failed to open file.");
        }
    } catch (std::exception &e) {
        fclog::error("Exception was caught during calculating of crc for file: {}. Message: {}", t_File, e.what());
        result = false;
    } catch (...) {
        fclog::error("Caught an unknown exception during calculation crc for file: {}", t_File);
        result = false;
    }
    return std::pair<bool, uint64_t>(result, crc);
}

std::pair<bool, std::string> FCFileInfoHelpers::readAcls(const std::string &t_File) noexcept
{
    std::string aclstring{};
    bool result{ true };
    ssize_t fileAclSize{};
    try {
        acl_t fileAcl = acl_get_file(t_File.c_str(), ACL_TYPE_ACCESS);
        if (nullptr == fileAcl) {
            fclog::error("Failed to get ACL for file: {}. Errno = {}", t_File, errno);
            result = false;
        }
        fileAclSize = acl_size(fileAcl);
        aclstring = std::string(acl_to_text(fileAcl, &fileAclSize));
        acl_free(fileAcl);
        aclstring.erase(std::remove_if(aclstring.begin(), aclstring.end(), ::isspace), aclstring.end());
    } catch (std::exception &e) {
        fclog::error("Exception was caught during reading acls for file: {}. Message: {}", t_File, e.what());
        result = false;
    } catch (...) {
        fclog::error("Caught an unknown exception during reading acls for file: {}", t_File);
        result = false;
    }
    return std::pair<bool, std::string>{ result, aclstring };
}

std::pair<bool, std::string> FCFileInfoHelpers::readCaps(const std::string &t_File) noexcept
{
    bool result{ true };
    std::string CAPS{};
    try {
        cap_t fileCaps = cap_get_file(t_File.c_str());

        if (fileCaps) {
            const char *txt_caps = cap_to_text(fileCaps, nullptr);
            cap_free(fileCaps);
            if (!txt_caps) {
                fclog::error("Problem while running cap_to_text() on {}", t_File);
                result = false;
            }
            CAPS = std::string(txt_caps);
        }
    } catch (std::exception &e) {
        fclog::error("Exception was caught during reading caps for file: {}. Message: {}", t_File, e.what());
        result = false;
    } catch (...) {
        fclog::error("Caught an unknown exception during reading of caps for file: {}", t_File);
        result = false;
    }
    return std::pair<bool, std::string>(result, CAPS);
}

struct stat FCFileInfoHelpers::readFileStat(const std::string &t_File) noexcept
{
    struct stat fileattrib
    {
    };
    //lstat() is identical to stat(), except that if path is a symbolic link, then the link itself is stat-ed, not the file that it refers to.
    if (-1 == lstat(t_File.c_str(), &fileattrib)) {
        fclog::error("Problem while running stat() on {}", t_File);
    }
    return fileattrib;
}

uint64_t FCFileInfoHelpers::readFileSize(const struct stat &t_FileStat) noexcept
{
    return t_FileStat.st_size;
}

uint32_t FCFileInfoHelpers::readFilePerms(const struct stat &t_FileStat) noexcept
{
    return t_FileStat.st_mode;
}

std::pair<bool, FCFileType> FCFileInfoHelpers::readFileType(const struct stat &t_FileStat, const std::string &t_FileName) noexcept
{
    bool result{ true };
    FCFileType fileType{ FCFileType::ERR };

    try {
        switch (t_FileStat.st_mode & S_IFMT) {
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

        if (FCFileType::ERR == fileType) {
            fclog::error("Problem while running checking file type on {}", t_FileName);
            result = false;
        }
    } catch (std::exception &e) {
        fclog::error("Exception was caught during reading file type for file: {}. Message: {}", t_FileName, e.what());
        result = false;
    } catch (...) {
        fclog::error("Caught an unknown exception during reading of file type for file: {}", t_FileName);
        result = false;
    }

    return std::pair<bool, FCFileType>(result, fileType);
}

uint32_t FCFileInfoHelpers::readFileOwner(const struct stat &t_FileStat) noexcept
{
    return t_FileStat.st_uid;
}

uint32_t FCFileInfoHelpers::readFileOwnerGroup(const struct stat &t_FileStat) noexcept
{
    return t_FileStat.st_gid;
}
bool FCFileInfoHelpers::isBlockDev(const std::string &filename) noexcept
{
    bool result{ false };
    try {
        auto filestat = FCFileInfoHelpers::readFileStat(filename);
        switch (filestat.st_mode & S_IFMT) {
        case S_IFBLK:
            result = true;
            break;
        default:
            result = false;
            break;
        }
    } catch (std::exception &e) {
        fclog::error("Exception was caught in isBlockDev for file: {}. Message: {}", filename, e.what());
        exit(-1);
    } catch (...) {
        fclog::error("Caught an unknown exception in isBlockDev for file: {}", filename);
        exit(-1);
    }
    return result;
}

void FCFileInfoHelpers::createDir(const std::string &dirName) noexcept
{
    std::error_code err;
    std::filesystem::create_directory(dirName, err);
    if (!err) {
        fclog::error("Cannot create folder: {}", dirName);
        exit(-1);
    }
}

bool FCFileInfoHelpers::fileExists(const std::string &fileName) noexcept
{
    std::error_code err;
    auto result = std::filesystem::exists(fileName, err);
    if (!err) {
        fclog::error("Cannot check if file exists: {}", fileName);
        exit(-1);
    }
    return result;
}