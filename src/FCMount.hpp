#ifndef SRC_FCMOUNT_HPP_
#define SRC_FCMOUNT_HPP_

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

#include <string>
class FCMount
{
  public:
    explicit FCMount(const std::string &what);
    FCMount() = delete;
    virtual ~FCMount();
    FCMount(const FCMount &other) = delete;             // copy constructor
    FCMount(FCMount &&other) = delete;                  // move constructor
    FCMount &operator=(const FCMount &other) = delete;  // copy assignment
    FCMount &operator=(FCMount &&other) = delete;       // move assignment
  private:
    bool mountFS(const std::string &what, const std::string &where);
    bool umountFS(const std::string &where);
    std::string makeUniqueMountPoint(const std::string &deviceName);
    std::string mountedDevice;
    std::string mountPoint;
};
#endif  // SRC_FCMOUNT_HPP_