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
    //Open the DB to files
    reOpenDBs();

    //Check them to be valid
    if(!checkStructure()){
        if(!reCreateDBs()){
            return false;
        }
    }

    //Start P2P system
    if(!mP2P.start()){
        return false;
    }

    //Start Mining system
    if(!mMiner.start()){
        return false;
    }


    //Start event handler
    //TODO

    return true;
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
    for(const auto& t : Config::mDBTables[Config::EDataBase::DBChain]){
        SQL::Stmt s(mDB.Chain, Config::getTableCreationSQL(t));
        int res = s.step();
        if(res != SQLITE_DONE){
            Logger::log(CORE, ERROR, "Cannot create tables in DBChain");
            return false;
        }
    }
    for(const auto& t : Config::mDBTables[Config::EDataBase::DBMain]){
        SQL::Stmt s(mDB.Main, Config::getTableCreationSQL(t));
        int res = s.step();
        if(res != SQLITE_DONE){
            Logger::log(CORE, ERROR, "Cannot create tables in DBMain");
            return false;
        }
    }

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

    return true;
}

// This method will check the DBs structure (sqlite_master)
bool Core::checkStructure(bool force_recheck){
    Logger::log(CORE, DEBUG, "Checking DB structure...");

    std::unique_lock<std::mutex> lock(mMutexDB);

    //Check the chain db has the proper table(s) inside
    {
        auto master = mDB.Chain.masterString(false);
        auto master_original = Config::getDBMaster(Config::DBChain, false);
        if(master_original != master){
            Logger::log(CORE, WARNING, "SQLiteMaster missmatch in Chain table");
            Logger::log(CORE, DEBUG, master + " != " + master_original);
            return false;
        }
    }

    //Check the main db has the proper table(s) inside
    {
        auto master = mDB.Main.masterString(false);
        auto master_original = Config::getDBMaster(Config::DBMain, false);
        if(master_original != master){
            Logger::log(CORE, WARNING, "SQLiteMaster missmatch in Main table");
            Logger::log(CORE, DEBUG, master + " != " + master_original);
            return false;
        }
    }

    Logger::log(CORE, INFO, "DB structure correct");
    return true;
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
