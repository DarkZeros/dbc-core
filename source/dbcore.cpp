#include "../include/dbcore.h"

#include "../sqlite/sqlite3.h"
#include "../sqlite/sha3sum.h"

#include <stdio.h>

#include "utils.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    for(int i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

DBC::DBC(const std::string path) : mPath(path) {
    //Lock all DBs
    std::unique_lock<std::mutex> lock(mMutexDB);

    //Open the DB to files
    mDB.Chain.swap(SQLiteDB(mPath+'/'+DEF_DB_FILE_CHAIN));
    mDB.Main.swap(SQLiteDB(mPath+'/'+DEF_DB_FILE_MAIN));
}

/* This method will check the DBs are in a proper state
 * If they are not, it will delete them all and wait for a new one to
 * come from the network.
 */
bool DBC::check(bool force_recheck){
    std::unique_lock<std::mutex> lock(mMutexDB);

    //Check the chain db has a "BlockHeaders" table
    SQLiteStmt s1(mDB.Chain, "SELECT name FROM sqlite_master WHERE type='table' AND name='" DEF_DB_TABLE_NAME_BLOCKHEADERS"';");
    if(s1.step() != SQLITE_ROW){
        return false;
    }

    //Check the columns are correct
    SQLiteStmt s2(mDB.Chain, "PRAGMA table_info('" DEF_DB_TABLE_NAME_BLOCKHEADERS "');");
    if(s2.step() != SQLITE_ROW){
        return false;
    }


    return true;
}

/*DBC::DBC(const std::string& path){
    int rc = sqlite3_open(path.c_str(), &mMainDB);
    if( rc ){
        printf("Can't open database: %s\n", sqlite3_errmsg(mMainDB));
    }else{
        printf("yeeeeeahhhhh\n");
        sqlite3_exec(mMainDB, "CREATE TABLE asd (num int(1), name VARCHAR(50))", callback, 0, 0);
    }
    //printf("test11 %s\n", hash_to_text(dbhash_SHA1("test11.db")).c_str());
    //printf("test2 %s\n", hash_to_text(dbhash_SHA1("test2.db")).c_str());

    printf("main %s\n", convert_string(sqlite_SHA3(mMainDB)).c_str());
    sqlite3 *db1, *db2;
    sqlite3_open("test1.db", &db1);
    sqlite3_open("test2.db", &db2);
    printf("test1 %s\n", convert_string(sqlite_SHA3(db1)).c_str());
    printf("test1 %s\n", convert_string(sqlite_SHA3(db2)).c_str());
    sqlite3_close(db1);
    sqlite3_close(db2);

    sqlite3_close(mMainDB);
}*/
