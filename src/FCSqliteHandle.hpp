#ifndef SRC_FCSQLITEHANDLE_HPP_
#define SRC_FCSQLITEHANDLE_HPP_

/**
 * @file FCSqliteHandle.cpp
 *
 * @brief a handle class for sqlite resources
 *
 * @ingroup filcomp
  *
 * @author Serhii Karasov
 * Contact: sergeyvkarasyov@gmail.com
 *
 */

#include <sqlite3.h>
#include "Handle.hpp"


class FCSqliteHandleTraits : HandleTraits<sqlite3 *>
{
  public:
    static void Close(Type value) noexcept
    {
        assert(SQLITE_OK == sqlite3_close(value));
    }
};

using FCSqliteHandle = Handle<FCSqliteHandleTraits>;

#endif// SRC_FCSQLITEHANDLE_HPP_