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
int SQLiteStmt::columns(){
    return sqlite3_column_count(mStmt);
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
const char * SQLiteStmt::getColumnText(int col){
    return (const char *)sqlite3_column_text(mStmt, col);
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

std::string SQLiteStmt::getSingleString(const char col_sep, const char row_sep){
    std::string str;
    const int cols = columns();
    while(step() == SQLITE_ROW){
        for(int c=0; c<cols; c++){
            auto res = getColumnText(c);
            if(res) //It might be null if field is empty
                str += res;
            if(c != cols-1)
                str += "|";
        }
        str += '\n';
    }
    //Remove last newline
    if(str.size()>0)
        str.resize(str.size()-1);
    return str;
}
