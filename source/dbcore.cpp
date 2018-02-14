#include "../include/dbcore.h"

#include "../sqlite/sqlite3.h"

#include <stdio.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    for(int i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

DBC::DBC(const std::string& path){
    int rc = sqlite3_open(path.c_str(), &mMainDB);
    if( rc ){
        printf("Can't open database: %s\n", sqlite3_errmsg(mMainDB));
    }else{
        printf("yeeeeeahhhhh");
        sqlite3_exec(mMainDB, "CREATE TABLE asd (num int(1), name VARCHAR(50))", callback, 0, 0);
    }
    sqlite3_close(mMainDB);
}
