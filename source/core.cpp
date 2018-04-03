#include "core.h"

#include "../sqlite/sqlite3.h"
#include "../sqlite/sha3sum.h"

#include <stdio.h>

#include "utils.h"
#include "protobuf/client.pb.h"

namespace DBC {

Core::Core(const std::string path) : mPath(path) {
}

bool Core::initialize() {
    //Start P2P system
    if(mP2P.start()){
        return false;
    }

    //Start event handler
    //TODO

    //Open the DB to files
    reOpenDBs();

    //Check them to be valid
    if(checkStructure()){
        reCreateDBs();
    }
    //TODO

    return false;
}

bool Core::reCreateDBs(){
    Logger::log(CORE, DEBUG, "Re-creating DBs");

    //Drop DBs (now they are read only if they were open)
    system(("rm "+
           buildPath(mPath,Config::getDBFilenames(Config::DBChain))+" "+
           buildPath(mPath,Config::getDBFilenames(Config::DBMain))
           ).c_str());

    //Reopen them
    reOpenDBs();

    //Lock all DBs
    std::unique_lock<std::mutex> lock(mMutexDB);

    //Build Structure of the DBs
    SQL::Stmt s1(mDB.Chain, Config::getTableCreationSQL(Config::TChain));
    int res = s1.step();
    SQL::Stmt s2(mDB.Main, Config::getTableCreationSQL(Config::TAccounts));
    res = s2.step();
    SQL::Stmt s3(mDB.Main, Config::getTableCreationSQL(Config::TCerts));
    res = s3.step();
    SQL::Stmt s4(mDB.Main, Config::getTableCreationSQL(Config::TPrev_Certs));
    res = s4.step();

    return true; //OK
}

bool Core::reOpenDBs(){
    Logger::log(CORE, DEBUG, "Re-opening DBs");

    //Lock all DBs
    std::unique_lock<std::mutex> lock(mMutexDB);

    //Ensure paths are created
    system(("mkdir -p "+mPath).c_str());

    //Open them (will close them if they are already open)
    mDB.Chain.swap(SQL::DB(buildPath(mPath,Config::getDBFilenames(Config::DBChain))));
    mDB.Main.swap(SQL::DB(buildPath(mPath,Config::getDBFilenames(Config::DBMain))));
}

// This method will check the DBs structure (sqlite_master)
bool Core::checkStructure(bool force_recheck){
    Logger::log(CORE, DEBUG, "Checking DB structure...");

    std::unique_lock<std::mutex> lock(mMutexDB);

    //Check the chain db has the proper table(s) inside
    {
        auto master = SQL::Stmt(mDB.Chain, "SELECT * FROM sqlite_master;").getSingleString();
        if(Config::getDBMaster(Config::DBChain) != master){
            Logger::log(CORE, WARNING, "SQLiteMaster missmatch in Chain table");
            Logger::log(CORE, DEBUG, master + " != " + Config::getDBMaster(Config::DBChain));
            return true;
        }
    }

    //Check the main db has the proper table(s) inside
    {
        auto master = SQL::Stmt(mDB.Main, "SELECT * FROM sqlite_master;").getSingleString();
        if(Config::getDBMaster(Config::DBMain) != master){
            Logger::log(CORE, WARNING, "SQLiteMaster missmatch in Main table");
            Logger::log(CORE, DEBUG, master + " != " + Config::getDBMaster(Config::DBMain));
            return true;
        }
    }

    Logger::log(CORE, INFO, "DB structure correct");

    return false;
}

/*
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

} //namespace DBC
