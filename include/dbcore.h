#ifndef DBCORE_H
#define DBCORE_H

#include <string>
#include <mutex>
#include <thread>
#include <map>

#include "../sqlite/sqlite_util.h"

#define DEF_PATH "data"
#define DEF_DB_FILE_CHAIN "chain.db"
#define DEF_DB_FILE_MAIN  "main.db"

#define DEF_DB_TABLE_NAME_BLOCKHEADERS  "block_headers"


class DBC {
    std::mutex mMutexDB; //This lock protects access to the main DBs
    struct {
        SQLiteDB Chain;
        SQLiteDB Main;
        std::map<std::string, SQLiteDB> Extras;
    } mDB;

    //Variables related to the main event loop


    //Variables
    std::string mPath;
public:
    DBC(const std::string path = "data");
    ~DBC(){}

    bool check(bool force_recheck = true);

};

#endif // DBCORE_H
