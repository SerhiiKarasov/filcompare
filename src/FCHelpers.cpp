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
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <ctime>
#include <sstream>

uint64_t FCHelpers::getProcessId()
{
    return getpid();
}

std::string FCHelpers::makeUniqueMountPointName(const std::string &deviceName)
{
    std::stringstream ss;
    ss << "/tmp/" << FCHelpers::getProcessId() << "_" << deviceName;
    return ss.str();
}