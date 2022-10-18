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

#include "src/FCHelpers.hpp"
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <ctime>
#include <fmt/core.h>

uint64_t FCHelpers::getProcessId()
{
    return getpid();
}

std::string FCHelpers::makeUniqueMountPointName(const std::string &deviceName)
{
    return fmt::format("/tmp/{}_{}", FCHelpers::getProcessId(), deviceName);
}