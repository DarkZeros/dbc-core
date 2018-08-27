#ifndef DBC_CORE_H
#define DBC_CORE_H

#include <string>
#include <mutex>
#include <thread>
#include <map>

#include "config.h"
#include "p2p.h"
#include "miner.h"
#include "timer.h"
#include "../sqlite/sqlite_util.h"

namespace DBC {

class Core {
    std::mutex mMutexDB; //This lock protects access to the main DBs
    struct {
        SQL::DB Chain, Main, Extra;
    } mDB;

    //Main modules
    Timer mTimer;
    P2P mP2P;
    Miner mMiner;

    //Variables
    std::string mPath;
public:
    Core(const std::string path = "data");
    ~Core(){}

    bool initialize();

    bool reCreateDBs();
    bool reOpenDBs();

    bool checkStructure(bool force_recheck = true);
    bool checkPoW(bool force_recheck = true){return false;}
    bool checkData(bool force_recheck = true);
};

} //namespace DBC

#endif // DBCORE_H
