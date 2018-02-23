#ifndef SHA3SUM_H
#define SHA3SUM_H

#include <stdint.h>
#include <vector>
#include <string>

#include "sqlite3.h"

enum SHA3_TYPE{
    sha3b224 = 224,
    sha3b256 = 256,
    sha3b384 = 384,
    sha3b512 = 512,
    def = sha3b256
};
static const std::string dummy_str = {};
std::vector<uint8_t> sqlite_SHA3(sqlite3 *db,
                                 SHA3_TYPE type = SHA3_TYPE::def,      /* Hash bytes                 */
                                 bool bSchema = true,                  /* Hash the schema as well    */
                                 bool bSeparate = false,               /* Hash each table separately */
                                 const std::string& zLike = dummy_str, /* Hash only matching tables  */
                                 std::string* debug_out = nullptr
                                 );

//Wrappers
std::vector<uint8_t> sqlite_SHA3(const std::string& path,
                                 SHA3_TYPE type = SHA3_TYPE::def,      /* Hash bytes                 */
                                 bool bSchema = true,                  /* Hash the schema as well    */
                                 bool bSeparate = false,               /* Hash each table separately */
                                 const std::string& zLike = dummy_str, /* Hash only matching tables  */
                                 std::string* debug_out = nullptr
                                 );

#endif // SHA3SUM_H
