#include "miner.h"

#include "logger.h"
#include "utils.h"

/*#include "cryptopp/cryptlib.h"
#include "cryptopp/secblock.h"
#include "cryptopp/eccrypto.h"
#include "cryptopp/osrng.h"
#include "cryptopp/oids.h"
#include "cryptopp/hex.h"*/
#include "cryptopp/blake2.h"

using namespace CryptoPP;

namespace DBC{

Miner::Miner(){
}
Miner::~Miner(){
}

bool Miner::configure(const std::string& file){
}

bool Miner::start(){
    /*AutoSeededRandomPool prng;
    ECDSA<ECP, SHA1>::PrivateKey privateKey;
    ECDSA<ECP, SHA1>::PublicKey publicKey;
    privateKey.Initialize( prng, CryptoPP::ASN1::secp256r1());*/

    BLAKE2b blake;
    std::vector<uint8_t> d;d.resize(32);
    blake.CalculateDigest(d.data(), (const byte *)"Hello world", 11);
    Logger::log(LogType::P2P, INFO,  convert_string(d));
    int r = blake.BlockSize();
    return true;
}
bool Miner::stop(){

}

bool Miner::addShard(const std::string& file, size_t start, size_t end){

}
bool Miner::removeShard(const std::string& file){

}

} //namespace DBC
