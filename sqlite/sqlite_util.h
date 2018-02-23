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

    int step();
};


#endif // SQLITEW_H
