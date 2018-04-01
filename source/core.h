#ifndef DBC_CORE_H
#define DBC_CORE_H

#include <string>
#include <mutex>
#include <thread>
#include <map>

#include "../sqlite/sqlite_util.h"
#include "config.h"

namespace DBC {

class Core {
    std::mutex mMutexDB; //This lock protects access to the main DBs
    struct {
        SQLiteDB Chain, Main, Extra;
    } mDB;

    //Variables related to the main event loop


    //Variables
    std::string mPath;
public:
    Core(const std::string path = "data");
    ~Core(){}

    void initialize();

    bool reCreateDBs();
    bool reOpenDBs();

    bool checkStructure(bool force_recheck = true);
    bool checkPoW(bool force_recheck = true){}
    bool checkData(bool force_recheck = true);
};

} //namespace DBC

#endif // DBCORE_H
