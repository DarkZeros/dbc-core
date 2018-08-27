#ifndef SQLITEW_H
#define SQLITEW_H

#include <string>

namespace SQL{

typedef struct sqlite3 sqlite3;
typedef struct sqlite3_stmt sqlite3_stmt;

class DB {
    sqlite3 * mDB = nullptr;
    std::string mPath;
public:
    DB();
    DB(const std::string& file);
    //No asigment, only movable
    DB(const DB&) = delete;
    DB& operator= (const DB& ) = delete;
    ~DB();

    void swap(DB&& o);
    operator sqlite3 *(){ return mDB;}
    int mStatus;

    const std::string& getPath()const{return mPath;}
    std::string masterString(bool rootpage = true);
};

class Stmt {
    sqlite3_stmt * mStmt = nullptr;
public:
    Stmt();
    Stmt(DB& db, const std::string& stmt);
    Stmt(sqlite3 * db, const std::string& stmt);
    //No asigment, only movable
    Stmt(const Stmt&) = delete;
    Stmt& operator= (const Stmt& ) = delete;
    ~Stmt();

    void swap(Stmt&& o);
    operator sqlite3_stmt *(){ return mStmt;}
    int mStatus;

    int step(); // SQLITE_BUSY, SQLITE_DONE, SQLITE_ROW, SQLITE_ERROR, or SQLITE_MISUSE
    int columns();

    std::string getSingleString(const char col_sep='|', const char row_sep='\n');

    const void *    getColumnBlob   (int col);
    double          getColumnDouble (int col);
    int             getColumnInt    (int col);
    int64_t         getColumnInt64  (int col);
    const char *    getColumnText   (int col);
    const void *    getColumnText16 (int col);

    int getColumnBytes  (int col); //Total bytes of BLOB or string
    int getColumnBytes16(int col); //Total bytes of String16
    int getColumnType   (int col); //SQLITE_INTEGER, SQLITE_FLOAT, SQLITE_TEXT, SQLITE_BLOB, or SQLITE_NULL
};

}//namespace SQL

#endif // SQLITEW_H
