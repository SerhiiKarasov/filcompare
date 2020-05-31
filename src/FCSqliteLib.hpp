#ifndef SRC_FCSQLITELIB_HPP_
#define SRC_FCSQLITELIB_HPP_

/**
 * @file FCSqliteConnectionHandle.cpp
 *
 * @brief a handle class for sqlite resources
 *
 * @ingroup filcomp
  *
 * @author Serhii Karasov
 * Contact: sergeyvkarasyov@gmail.com
 *
 */

#include "Handle.hpp"
#include <sqlite3.h>
#include <string>
#include <utility>  //for swap


enum class FCSQLITE_TYPE {
    Integer = SQLITE_INTEGER,
    Float = SQLITE_FLOAT,
    Blob = SQLITE_BLOB,
    Text = SQLITE_TEXT,
    Null = SQLITE_NULL,
};


class FCSqliteException
{
  public:
    int Result = 0;
    std::string Message;
    explicit FCSqliteException(sqlite3 *const connection) : Result(sqlite3_extended_errcode(connection)),
                                                            Message(sqlite3_errmsg(connection))
    {}
};


class FCSqliteConnection
{
  private:
    class FCSqliteConnectionHandleTraits : public HandleTraits<sqlite3 *>
    {
      public:
        static void Close(Type value) noexcept
        {
            assert(SQLITE_OK == sqlite3_close(value));
        }
    };
    using FCSqliteConnectionHandle = Handle<FCSqliteConnectionHandleTraits>;
    FCSqliteConnectionHandle m_handle;

    template<typename F, typename C>
    void InternalOpen(F open, C const *const filename)
    {
        FCSqliteConnection temp;
        if (SQLITE_OK != open(filename, temp.m_handle.Set())) {
            temp.ThrowLastError();
        }
        swap(m_handle, temp.m_handle);
    }

  public:
    FCSqliteConnection() noexcept = default;

    template<typename C>
    explicit FCSqliteConnection(C const *const filename)
    {
        Open(filename);
    }

    static FCSqliteConnection Memory()
    {
        return FCSqliteConnection(":memory:");
    }

    static FCSqliteConnection WideMemory()
    {
        return FCSqliteConnection(L":memory:");
    }

    //check if the connection has valid state
    explicit operator bool() const noexcept
    {
        return static_cast<bool>(m_handle);
    }

    sqlite3 *GetAbi() const noexcept
    {
        return m_handle.Get();
    }

    [[noreturn]] void ThrowLastError() const {
        throw FCSqliteException(GetAbi());
    }

    void Open(char const *const filename)
    {
        InternalOpen(sqlite3_open, filename);
    }

    void Open(wchar_t const *const filename)
    {
        InternalOpen(sqlite3_open16, filename);
    }

    int64_t RowId() const noexcept
    {
        return sqlite3_last_insert_rowid(GetAbi());
    }

    template<typename F>
    void Profile(F callback, void *const context = nullptr)
    {
        sqlite3_profile(GetAbi(), callback, context);
    }
};

class Backup
{
  private:
    class FCSqliteBackupHandleTraits : public HandleTraits<sqlite3_backup *>
    {
      public:
        static void Close(Type value) noexcept
        {
            assert(SQLITE_OK == sqlite3_backup_finish(value));
        }
    };
    using FCSqliteBackupHandle = Handle<FCSqliteBackupHandleTraits>;
    FCSqliteBackupHandle m_handle;
    FCSqliteConnection const *m_destination = nullptr;

  public:
    Backup(FCSqliteConnection const &destination,
        FCSqliteConnection const &source,
        char const *const destinationName = "main",
        char const *const sourceName = "main")
        : m_handle(sqlite3_backup_init(destination.GetAbi(), destinationName, source.GetAbi(), sourceName)),
          m_destination(&destination)
    {
        if (!m_handle) {
            destination.ThrowLastError();
        }
    }

    sqlite3_backup *GetAbi() const noexcept
    {
        return m_handle.Get();
    }

    bool Step(int const pages = -1)
    {
        const auto result = sqlite3_backup_step(GetAbi(), pages);
        if (result == SQLITE_OK) return true;
        if (result == SQLITE_DONE) return false;

        m_handle.Reset();
        m_destination->ThrowLastError();
    }
};

//CRTP
template<typename T>
class Reader
{
  public:
    int GetInt(int const column = 0) const noexcept
    {
        return sqlite3_column_int(static_cast<T const *>(this)->GetAbi(), column);
    }
    const char *GetString(int const column = 0) const noexcept
    {
        return reinterpret_cast<const char *>(sqlite3_column_text(static_cast<T const *>(this)->GetAbi(), column));
    }
    const wchar_t *GetWideString(int const column = 0) const noexcept
    {
        return static_cast<const wchar_t *>(sqlite3_column_text16(static_cast<T const *>(this)->GetAbi(), column));
    }
    int GetStringLength(int const column = 0) const noexcept
    {
        return sqlite3_column_bytes(static_cast<T const *>(this)->GetAbi(), column);
    }
    int GetWideStringLength(int const column = 0) const noexcept
    {
        return sqlite3_column_bytes16(static_cast<T const *>(this)->GetAbi(), column) * sizeof(wchar_t);
    }
    FCSQLITE_TYPE GetType(int const column = 0) const noexcept
    {
        return static_cast<FCSQLITE_TYPE>(sqlite3_column_type(static_cast<T const *>(this)->GetAbi(), column));
    }
};

class Row : public Reader<Row>
{
  private:
    sqlite3_stmt *m_statement = nullptr;

  public:
    sqlite3_stmt *GetAbi() const noexcept
    {
        return m_statement;
    }
    explicit Row(sqlite3_stmt *const statement) noexcept : m_statement(statement)
    {}
};


class FCSqliteStatement : public Reader<FCSqliteStatement>
{
  private:
    class FCSqliteStatementHandleTraits : public HandleTraits<sqlite3_stmt *>
    {
      public:
        static void Close(Type value) noexcept
        {
            assert(SQLITE_OK == sqlite3_finalize(value));
        }
    };
    using FCSqliteStatementHandle = Handle<FCSqliteStatementHandleTraits>;
    FCSqliteStatementHandle m_handle;

    template<typename F, typename C, typename... Values>
    void InternalPrepare(FCSqliteConnection const &connection, F prepare, C const *const text, Values &&... values)
    {
        assert(connection);
        if (SQLITE_OK != prepare(connection.GetAbi(), text, -1, m_handle.Set(), nullptr)) {
            connection.ThrowLastError();
        }
        BindAll(std::forward<Values>(values)...);
    }

    void InternalBind(int) const noexcept
    {
    }

    template<typename First, typename... Rest>
    void InternalBind(int const index, First &&first, Rest &&... rest) const
    {
        Bind(index, std::forward<First>(first));
        InternalBind(index + 1, std::forward<Rest>(rest)...);
    }

  public:
    FCSqliteStatement() noexcept = default;

    template<typename C, typename... Values>
    FCSqliteStatement(FCSqliteConnection const &connection, C const *const text, Values &&... values)
    {
        Prepare(connection, text, std::forward<Values>(values)...);
    }

    explicit operator bool() const noexcept
    {
        return static_cast<bool>(m_handle);
    }

    sqlite3_stmt *GetAbi() const noexcept
    {
        return m_handle.Get();
    }

    [[noreturn]] void ThrowLastError() const {
        throw FCSqliteException(sqlite3_db_handle(GetAbi()));
    }

    template<typename... Values>
    void Prepare(FCSqliteConnection const &connection, char const *const text, Values &&... values)
    {
        InternalPrepare(connection, sqlite3_prepare_v2, text, std::forward<Values>(values)...);
    }

    template<typename... Values>
    void Prepare(FCSqliteConnection const &connection, wchar_t const *const text, Values &&... values)
    {
        InternalPrepare(connection, sqlite3_prepare16_v2, text, std::forward<Values>(values)...);
    }

    bool Step() const
    {
        int const result = sqlite3_step(GetAbi());
        if (result == SQLITE_ROW) return true;
        if (result == SQLITE_DONE) return false;
        ThrowLastError();
    }

    void Execute() const
    {
        assert(!Step());
    }

    void Bind(const int index, const int value) const
    {
        if (SQLITE_OK != sqlite3_bind_int(GetAbi(), index, value)) {
            ThrowLastError();
        }
    }
    void Bind(const int index, const char *const value, int const size = -1) const
    {
        if (SQLITE_OK != sqlite3_bind_text(GetAbi(), index, value, size, SQLITE_STATIC)) {
            ThrowLastError();
        }
    }

    void Bind(const int index, const wchar_t *const value, int const size = -1) const
    {
        if (SQLITE_OK != sqlite3_bind_text16(GetAbi(), index, value, size, SQLITE_STATIC)) {
            ThrowLastError();
        }
    }

    void Bind(const int index, std::string const &value) const
    {
        Bind(index, value.c_str(), value.size());
    }

    void Bind(const int index, std::wstring const &value) const
    {
        Bind(index, value.c_str(), value.size() * sizeof(wchar_t));
    }

    //rvalue
    void Bind(const int index, std::string const &&value) const
    {
        if (SQLITE_OK != sqlite3_bind_text(GetAbi(), index, value.c_str(), value.size(), SQLITE_TRANSIENT)) {
            ThrowLastError();
        }
    }

    void Bind(const int index, std::wstring const &&value) const
    {
        if (SQLITE_OK != sqlite3_bind_text16(GetAbi(), index, value.c_str(), value.size() * sizeof(wchar_t), SQLITE_TRANSIENT)) {
            ThrowLastError();
        }
    }

    template<typename... Values>
    void BindAll(Values &&... values) const
    {
        InternalBind(1, std::forward<Values>(values)...);
    }

    template<typename... Values>
    void Reset(Values &&... values) const
    {
        if (SQLITE_OK != sqlite3_reset(GetAbi())) {
            ThrowLastError();
        }
        BindAll(values...);
    }
};


class RowIterator
{
  private:
    FCSqliteStatement const *m_statement = nullptr;

  public:
    RowIterator() noexcept = default;
    explicit RowIterator(FCSqliteStatement const &statement) noexcept
    {
        if (statement.Step()) {
            m_statement = &statement;
        }
    }
    RowIterator &operator++() noexcept
    {
        if (!m_statement->Step()) {
            m_statement = nullptr;
        }
        return *this;
    }

    bool operator!=(RowIterator const &other) const noexcept
    {
        return m_statement != other.m_statement;
    }

    Row operator*() const noexcept
    {
        return Row(m_statement->GetAbi());
    }
};

inline RowIterator begin(FCSqliteStatement const &statement) noexcept
{
    return RowIterator(statement);
}

inline RowIterator end(FCSqliteStatement const &) noexcept
{
    return RowIterator();
}

template<typename C, typename... Values>
void Execute(FCSqliteConnection const &connection, C const *const text, Values &&... values)
{
    FCSqliteStatement(connection, text, std::forward<Values>(values)...).Execute();
}

#endif  //  SRC_FCSQLITELIB_HPP_