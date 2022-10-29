/**
 * @file FCHelpers.cpp
 *
 * @brief a set of helper functions to be used by any class from the application
 *
 * @ingroup filcomp
 *
 * @author Serhii Karasov
 * Contact: sergeyvkarasyov@gmail.com
 *
 */

#include "FCHelpers.hpp"

#include <fmt/core.h>
#include <string>
#include <unistd.h>

uint64_t FCHelpers::getProcessId()
{
    return getpid();
}

std::string FCHelpers::makeUniqueMountPointName(const std::string &deviceName)
{
    return fmt::format("/tmp/{}_{}", FCHelpers::getProcessId(), deviceName);
}