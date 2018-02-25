#ifndef UTILS_H
#define UTILS_H

#include <inttypes.h>
#include <string>
#include <vector>

std::string convert_string(const std::vector<uint8_t>& in);

std::string buildPath(const std::string& s1,const std::string& s2);
std::string buildPath(const std::string& s1,const std::string& s2,const std::string& s3);
std::string buildPath(const std::string& s1,const std::string& s2, const std::string& s3, const std::string& s4);


#endif // UTILS_H
