#include "sqlite3.h"
#include "sqlite_util.h"

SQLiteDB::SQLiteDB() : mStatus(SQLITE_OK){
}
SQLiteDB::SQLiteDB(const std::string& file){
    mStatus = sqlite3_open_v2(file.c_str(), &mDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
}
SQLiteDB::~SQLiteDB(){
    if(mDB){
        sqlite3_close(mDB);
    }
}
void SQLiteDB::swap(SQLiteDB&& o){
    std::swap(o.mDB, mDB);
    std::swap(o.mStatus, mStatus);
}




SQLiteStmt::SQLiteStmt() : mStatus(SQLITE_OK){
}
SQLiteStmt::SQLiteStmt(sqlite3 * db, const std::string& stmt){
    mStatus = sqlite3_prepare_v2(db, stmt.c_str(), -1, &mStmt, NULL);
}
SQLiteStmt::~SQLiteStmt(){
    if(mStmt){
        sqlite3_finalize(mStmt);
    }
}
void SQLiteStmt::swap(SQLiteStmt &&o){
    std::swap(o.mStmt, mStmt);
    std::swap(o.mStatus, mStatus);
}
int SQLiteStmt::step(){
    return sqlite3_step(mStmt);
}

const void * SQLiteStmt::getColumnBlob(int col){
    return sqlite3_column_blob(mStmt, col);
}
double SQLiteStmt::getColumnDouble(int col){
    return sqlite3_column_double(mStmt, col);
}
int SQLiteStmt::getColumnInt(int col){
    return sqlite3_column_int(mStmt, col);
}
int64_t SQLiteStmt::getColumnInt64(int col){
    return sqlite3_column_int64(mStmt, col);
}
const uint8_t * SQLiteStmt::getColumnText(int col){
    return sqlite3_column_text(mStmt, col);
}
const void * SQLiteStmt::getColumnText16(int col){
    return sqlite3_column_text16(mStmt, col);
}

int SQLiteStmt::getColumnBytes(int col){
    return sqlite3_column_bytes(mStmt, col);
}
int SQLiteStmt::getColumnBytes16(int col){
    return sqlite3_column_bytes16(mStmt, col);
}
int SQLiteStmt::getColumnType(int col){
    return sqlite3_column_type(mStmt, col);
}
