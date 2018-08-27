#ifndef MINER_H
#define MINER_H

#include <vector>

#include "sqlite/sqlite_util.h"

//#include "cryptopp/eccrypto.h"
//using namespace CryptoPP;

namespace DBC{

class Miner {
public:
    Miner();
    ~Miner();

    bool configure(const std::string& file);

    bool start();
    bool stop();

    bool addShard(const std::string& file, size_t start = 0, size_t end = 0);
    bool removeShard(const std::string& file);

    //bool setPubKey(const ECDSA<ECP, SHA1>::PublicKey& key, int64_t account);

private:
    struct Shard{
        SQL::DB db;
        size_t noonce_start;
        size_t noonce_end;
    };
    std::vector<Shard> mShards;

    //ECDSA<ECP, SHA1>::PublicKey mPubKey;
    int64_t mAccount;


};

} //namespace DBC

#endif // MINER_H
