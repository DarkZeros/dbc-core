#include "config.h"

#include "sqlite/sqlite_util.h"

namespace DBC {

//TODO: Possibly we will need to ensure that the blob sizes/text length/integer lengths are ok
//A malicius user may set the sizes to be huge.
//We should therefore checking it thoroughly

//TODO: SQL transactions by users may also be malicius, we can either:
//*Check each of them to be valid by some check code, and only use a subset of parameters of SQL
// (We can extend it progressively each revision to support more commands)
//*Add trigers to tables that will fire RAISE(ROLLBACK) if the unallowed operation is performed
// Problem with that is that we will overload the SQL operation and a very clever user may use
// a transaction that bypasses the triggers.

constexpr std::array<int, Config::mNumVersions> Config::mVersion;

std::vector<std::vector<Config::ETable>> Config::mDBTables = {
    {Config::ETable::TPowChain, Config::ETable::TBlockChain},

    {Config::ETable::TAccounts, Config::ETable::TCerts, Config::ETable::TPrev_Certs},

    {Config::ETable::TCertTransactions, Config::ETable::TMerkleHash_DB,
     Config::ETable::TMerkleHash_TX},
};

const char * Config::mDataBaseFilename[Config::mNumVersions][Config::EDataBase::DBSize] = {
/// Version 0
{
    "blockchain.db",
    "main.db",
    "extra.db"
}};

const char * Config::mTableCreationSQL[Config::mNumVersions][Config::ETable::TSize] = {
/// Version 0
{
    "CREATE TABLE powchain ("
        "id INTEGER PRIMARY KEY,"
        "hash BLOB NOT NULL,"
        "acc_pubkey BLOB NOT NULL,"
        "nonce BLOB NOT NULL,"
        "timestamp INTEGER NOT NULL"
    ");",
    "CREATE TABLE blockchain ("
        "id INTEGER PRIMARY KEY,"
        "prev_hash BLOB NOT NULL,"
        "tx_root BLOB NOT NULL,"
        "db_root BLOB NOT NULL,"
        "nonces TEXT NOT NULL,"
        "timestamp INTEGER NOT NULL"
    ");",
    "CREATE TABLE accounts ("
        "id INTEGER PRIMARY KEY,"
        "pub_key BLOB NOT NULL,"
        "balance INTEGER NOT NULL,"
        "tx_allow INTEGER NOT NULL,"
        "tx_count INTEGER NOT NULL,"
        "extra TEXT"
    ");",
    "CREATE TABLE certs ("
        "id INTEGER PRIMARY KEY,"
        "account_id INTEGER UNIQUE NOT NULL"
            " REFERENCES accounts(id) ON DELETE CASCADE,"
        "contract TEXT NOT NULL"
    ");",
    "CREATE TABLE prev_certs ("
        "id INTEGER PRIMARY KEY,"
        "block_num INTEGER NOT NULL,"
        "cert_num INTEGER NOT NULL," //Cert number at that time
        "pub_key INTEGER NOT NULL," //Pub key of certifier
            //Just for reference, not really needed
        "account_id INTEGER NOT NULL,"  //Account id of the certifier, to cross check it is valid
        "cert_power INTEGER NOT NULL," //Cert power at that time, to cross check, again
        "contract TEXT NOT NULL"       //Contract at that time, anyway transactions don't care about the contract
    ");",
    "CREATE TABLE cert_transactions ("
        "id INTEGER PRIMARY KEY,"
    ");",
    "CREATE TABLE merkle_tx ("
        "depth INTEGER NOT NULL," //0 is the root, then it goes up as needed
        "index INTEGER NOT NULL," //0 for the root, 0-1 for the depth 1, and so on
        "hash BLOB NOT NULL"
    ");",
    "CREATE TABLE merkle_db ("
        "depth INTEGER NOT NULL," //0 is the root, then it goes up as needed
        "index INTEGER NOT NULL," //0 for the root, 0-1 for the depth 1, and so on
        "hash BLOB NOT NULL"
    ");"
}};

const char * Config::mDataBaseMaster[Config::mNumVersions][Config::EDataBase::DBSize] = {
/// Version 0
{
    "table|powchain|powchain|3|CREATE TABLE powchain (id INTEGER PRIMARY KEY,hash BLOB NOT NULL,acc_pubkey BLOB NOT NULL,nonce BLOB NOT NULL,timestamp INTEGER NOT NULL)\n"
        "table|blockchain|blockchain|4|CREATE TABLE blockchain (id INTEGER PRIMARY KEY,prev_hash BLOB NOT NULL,tx_root BLOB NOT NULL,db_root BLOB NOT NULL,nonces TEXT NOT NULL,timestamp INTEGER NOT NULL)",
    "table|accounts|accounts|3|CREATE TABLE accounts (id INTEGER PRIMARY KEY,pub_key BLOB NOT NULL,balance INTEGER NOT NULL,tx_allow INTEGER NOT NULL,tx_count INTEGER NOT NULL,extra TEXT)\n"
        "table|certs|certs|4|CREATE TABLE certs (id INTEGER PRIMARY KEY,account_id INTEGER UNIQUE NOT NULL REFERENCES accounts(id) ON DELETE CASCADE,contract TEXT NOT NULL)\n"
        "index|sqlite_autoindex_certs_1|certs|5|\n"
        "table|prev_certs|prev_certs|6|CREATE TABLE prev_certs (id INTEGER PRIMARY KEY,block_num INTEGER NOT NULL,cert_num INTEGER NOT NULL,pub_key INTEGER NOT NULL,account_id INTEGER NOT NULL,cert_power INTEGER NOT NULL,contract TEXT NOT NULL)\n",
    ""
}};

int Config::blockNum2Version(int blockNum){
    for(size_t v = 0; v < mVersion.size(); v++){
        if(blockNum >= mVersion[v]) return v;
    }
    return mVersion.size()-1;
}

const char * Config::getTableCreationSQL(ETable tableType, int blockNum){
    return mTableCreationSQL[blockNum2Version(blockNum)][tableType];
}
const char * Config::getDBFilenames(EDataBase databaseType, int blockNum){
    return mDataBaseFilename[blockNum2Version(blockNum)][databaseType];
}
std::string Config::getDBMaster(EDataBase databaseType, bool rootpage, int blockNum){
    SQL::DB mem(":memory:");
    for(const auto& i : mDBTables[databaseType]){
        SQL::Stmt st(mem, mTableCreationSQL[blockNum2Version(blockNum)][i]);
        st.step();
    }
    return mem.masterString(rootpage);
}

} //namespace DBC
