#ifndef DBCORE_H
#define DBCORE_H

#include <string>

typedef struct sqlite3 sqlite3;

class DBC {
    sqlite3 * mMainDB = nullptr;
    sqlite3 * mLastBlockDB = nullptr;

public:
    DBC(){}
    DBC(const std::string& path);

    //void intialize
};

#endif // DBCORE_H
