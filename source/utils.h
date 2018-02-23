#ifndef UTILS_H
#define UTILS_H

#include <inttypes.h>
#include <string>
#include <vector>

std::string convert_string(const std::vector<uint8_t>& in);

template<typename... Args>
std::string build_path(const std::string& st, const std::string& s2, Args... ){

}

#endif // UTILS_H
