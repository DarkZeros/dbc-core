#include "sqlite3.h"
#include "sqlite_util.h"

namespace SQL{

DB::DB() : mStatus(SQLITE_OK){
}
DB::DB(const std::string& file){
    mPath = file;
    mStatus = sqlite3_open_v2(file.c_str(), &mDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
}
DB::~DB(){
    if(mDB){
        sqlite3_close(mDB);
    }
}
void DB::swap(DB&& o){
    std::swap(o.mDB, mDB);
    std::swap(o.mStatus, mStatus);
}

std::string DB::masterString(bool rootpage){
    SQL::Stmt s(*this, rootpage ?
                    "SELECT * FROM sqlite_master;" :
                    "SELECT type,name,tbl_name,sql FROM sqlite_master;");
    return s.getSingleString();
}


Stmt::Stmt() : mStatus(SQLITE_OK){
}
Stmt::Stmt(DB& db, const std::string& stmt){
    mStatus = sqlite3_prepare_v2(db, stmt.c_str(), -1, &mStmt, NULL);
}
Stmt::Stmt(sqlite3 * db, const std::string& stmt){
    mStatus = sqlite3_prepare_v2(db, stmt.c_str(), -1, &mStmt, NULL);
}
Stmt::~Stmt(){
    if(mStmt){
        sqlite3_finalize(mStmt);
    }
}
void Stmt::swap(Stmt &&o){
    std::swap(o.mStmt, mStmt);
    std::swap(o.mStatus, mStatus);
}
int Stmt::step(){
    return sqlite3_step(mStmt);
}
int Stmt::columns(){
    return sqlite3_column_count(mStmt);
}

const void * Stmt::getColumnBlob(int col){
    return sqlite3_column_blob(mStmt, col);
}
double Stmt::getColumnDouble(int col){
    return sqlite3_column_double(mStmt, col);
}
int Stmt::getColumnInt(int col){
    return sqlite3_column_int(mStmt, col);
}
int64_t Stmt::getColumnInt64(int col){
    return sqlite3_column_int64(mStmt, col);
}
const char * Stmt::getColumnText(int col){
    return (const char *)sqlite3_column_text(mStmt, col);
}
const void * Stmt::getColumnText16(int col){
    return sqlite3_column_text16(mStmt, col);
}

int Stmt::getColumnBytes(int col){
    return sqlite3_column_bytes(mStmt, col);
}
int Stmt::getColumnBytes16(int col){
    return sqlite3_column_bytes16(mStmt, col);
}
int Stmt::getColumnType(int col){
    return sqlite3_column_type(mStmt, col);
}

std::string Stmt::getSingleString(const char col_sep, const char row_sep){
    std::string str;
    const int cols = columns();
    while(step() == SQLITE_ROW){
        for(int c=0; c<cols; c++){
            auto res = getColumnText(c);
            if(res) //It might be null if field is empty
                str += res;
            if(c != cols-1)
                str += col_sep;
        }
        str += row_sep;
    }
    //Remove last newline
    if(str.size()>0)
        str.resize(str.size()-1);
    return str;
}

}//namespace SQL
