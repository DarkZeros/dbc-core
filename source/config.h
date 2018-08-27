#ifndef DBC_CONFIG_H
#define DBC_CONFIG_H

#include <string>
#include <array>
#include <vector>

namespace DBC {

/* This class contains the default values of the DBC that have to be respected by all clients
 * Table configurations, table names, etc.
 * If the config does not match it will be considered invalid.
 * Config can be changed at certain points in time (blockchain length)
*/
class Config {
public:
    enum EDataBase{
        DBChain,
        DBMain,
        DBExtra,
        DBSize
    };
    enum ETable{
        TPowChain,
        TBlockChain,

        TAccounts,
        TCerts,
        TPrev_Certs,

        TCertTransactions,
        TMerkleHash_DB,
        TMerkleHash_TX,
        TSize
    };
    static std::vector<std::vector<ETable>> mDBTables;

    static constexpr int mNumVersions = 1;
    static constexpr std::array<int, mNumVersions> mVersion = {{0}};

    static const char * mDataBaseFilename[mNumVersions][EDataBase::DBSize];

    //Used to create the tables, and cross check
    static const char * mTableCreationSQL[mNumVersions][ETable::TSize];
    //Used to cross check agains the "sqlite_master"
    static const char * mDataBaseMaster[mNumVersions][EDataBase::DBSize];


    static int blockNum2Version(int blockNum);

    static const char * getTableCreationSQL(ETable tableType, int blockNum = -1);
    static const char * getDBFilenames(EDataBase databaseType, int blockNum = -1);
    static std::string getDBMaster(EDataBase databaseType, bool rootpage = true, int blockNum = -1);

};

} //namespace DBC

#endif // DBC_CONFIG_H
