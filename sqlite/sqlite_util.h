#ifndef SQLITEW_H
#define SQLITEW_H

#include <string>

typedef struct sqlite3 sqlite3;
typedef struct sqlite3_stmt sqlite3_stmt;

class SQLiteDB {
    sqlite3 * mDB = nullptr;
public:
    SQLiteDB();
    SQLiteDB(const std::string& file);
    //No asigment, only movable
    SQLiteDB(const SQLiteDB&) = delete;
    SQLiteDB& operator= (const SQLiteDB& ) = delete;
    ~SQLiteDB();

    void swap(SQLiteDB&& o);
    operator sqlite3 *(){ return mDB;}
    int mStatus;
};

class SQLiteStmt {
    sqlite3_stmt * mStmt = nullptr;
public:
    SQLiteStmt();
    SQLiteStmt(sqlite3 * db, const std::string& stmt);
    //No asigment, only movable
    SQLiteStmt(const SQLiteStmt&) = delete;
    SQLiteStmt& operator= (const SQLiteStmt& ) = delete;
    ~SQLiteStmt();

    void swap(SQLiteStmt&& o);
    operator sqlite3_stmt *(){ return mStmt;}
    int mStatus;

    int step(); // SQLITE_BUSY, SQLITE_DONE, SQLITE_ROW, SQLITE_ERROR, or SQLITE_MISUSE

    const void *    getColumnBlob   (int col);
    double          getColumnDouble (int col);
    int             getColumnInt    (int col);
    int64_t         getColumnInt64  (int col);
    const uint8_t * getColumnText   (int col);
    const void *    getColumnText16 (int col);

    int getColumnBytes  (int col); //Total bytes of BLOB or string
    int getColumnBytes16(int col); //Total bytes of String16
    int getColumnType   (int col); //SQLITE_INTEGER, SQLITE_FLOAT, SQLITE_TEXT, SQLITE_BLOB, or SQLITE_NULL
};


#endif // SQLITEW_H
