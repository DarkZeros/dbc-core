
#include "utils.h"

static const char hex2str[] = "0123456789abcdef";

std::string convert_string(const std::vector<uint8_t>& in){
    std::string res(in.size()*2, 0);

    for(size_t i = 0; i<in.size(); i++){
        res[i*2  ] = hex2str[(in[i]>>4)&0xf];
        res[i*2+1] = hex2str[ in[i]    &0xf];
    }
    return res;
}
