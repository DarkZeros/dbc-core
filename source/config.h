#ifndef DBC_CONFIG_H
#define DBC_CONFIG_H

#include <string>
#include <array>

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
        TChain,
        TAccounts,
        TCerts,
        TPrev_Certs,

        TCertTransactions,
        TMerkleHash_DB,
        TMerkleHash_TX,
        TSize
    };

    static constexpr int mNumVersions = 1;
    static constexpr std::array<int, mNumVersions> mVersion = {{0}};

    static const std::string mDataBaseFilename[mNumVersions][EDataBase::DBSize];

    //Used to create the tables, and cross check
    static const std::string mTableCreationSQL[mNumVersions][ETable::TSize];
    //Used to cross check agains the "sqlite_master"
    static const std::string mDataBaseMaster[mNumVersions][EDataBase::DBSize];


    static int blockNum2Version(int blockNum);

    static const std::string& getTableCreationSQL(ETable tableType, int blockNum = -1);
    static const std::string& getDBMaster(EDataBase databaseType, int blockNum = -1);
    static const std::string& getDBFilenames(EDataBase databaseType, int blockNum = -1);

};

} //namespace DBC

#endif // DBC_CONFIG_H
