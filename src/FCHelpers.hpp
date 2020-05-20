#ifndef SRC_FCHELPERS_HPP_
#define SRC_FCHELPERS_HPP_

/**
 * @file FCHelpers.hpp
 *
 * @brief a set of helper functions to be used by any class from the application
 *
 * @ingroup filcomp
  *
 * @author Serhii Karasov
 * Contact: sergeyvkarasyov@gmail.com
 *
 */

#include <cstdint>
#include <string>

namespace FCHelpers {
uint64_t getProcessId();
std::string makeUniqueMountPointName(const std::string &deviceName);

}  // namespace FCHelpers
#endif  // SRC_FCHELPERS_HPP_