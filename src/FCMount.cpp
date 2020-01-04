/**
 * @file FCMount.cpp
 *
 * @brief a class to handle mounting of filesystems, images
 *
 * @ingroup filcomp
  *
 * @author Serhii Karasov
 * Contact: sergeyvkarasyov@gmail.com
 *
 */

#include "FCMount.hpp"
#include <sstream>
#include <iostream>
#include <cerrno>
#include <cstring>
#include "FCHelpers.hpp"
#include "FCFileInfoHelpers.hpp"

FCMount::FCMount(const std::string &what) : mountedDevice(what),
                                            mountPoint(makeUniqueMountPoint(what))
{
    mountFS(mountPoint, mountedDevice);
}

FCMount::~FCMount()
{
    umountFS(mountPoint);
}

bool FCMount::mountFS(const std::string &what, const std::string &where)
{
    auto mount_result = false;
    if (!FCFileInfoHelpers::fileExists(what))
    {
        std::cerr << "File doesn't exist: " << what << std::endl;
    }
    std::string mount_command = "";
    if (FCFileInfoHelpers::isBlockDev(what))
    {
        mount_command = "mount  -o ro " + what + " " + where;
    }
    else
    {
        mount_command = "mount  -o ro,loop " + what + " " + where;
    }
    mount_result = system(mount_command.c_str());
    if (0 != mount_result)
    {
        std::cerr << "Cannot mount " << what << " to mount point " << where << std::endl;
        std::cerr << "Return code from mount command " << mount_result << " " << std::strerror(errno) << std::endl;
        mount_result = false;
    }
    return mount_result;
}

bool FCMount::umountFS(const std::string &where)
{
    auto unmount_command = "umount " + where;
    const auto umount_result = system(unmount_command.c_str());
    if (0 != umount_result)
    {
        std::cerr << "Cannot unmount from: " << where << std::endl;
        std::cerr << "Return code from umount command " << umount_result << " " << std::strerror(errno) << std::endl;
        return false;
    }
    return true;
}

std::string FCMount::makeUniqueMountPoint(const std::string &deviceName)
{
    auto mountPointName = FCHelpers::makeUniqueMountPointName(deviceName);
    FCFileInfoHelpers::createDir(mountPointName);
    return mountPointName;
}
