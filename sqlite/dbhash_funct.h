#ifndef DBHASH_FUNCT_H
#define DBHASH_FUNCT_H

#include <string>
#include <vector>

std::vector<unsigned char> dbhash_SHA1(const std::string& dbfile,
                                       const std::string zLike = std::string("%"),
                                       bool omitContent = false,
                                       bool omitSchema = false);

std::string hash_to_text(const std::vector<unsigned char>& in);


#endif // DBHASH_FUNCT_H
