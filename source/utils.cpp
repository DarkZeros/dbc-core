
#include "utils.h"

namespace DBC {

static const char hex2str[] = "0123456789abcdef";

std::string convert_string(const std::vector<uint8_t>& in){
    std::string res(in.size()*2, 0);

    for(size_t i = 0; i<in.size(); i++){
        res[i*2  ] = hex2str[(in[i]>>4)&0xf];
        res[i*2+1] = hex2str[ in[i]    &0xf];
    }
    return res;
}

std::string buildPath(const std::string& s1,const std::string& s2){
    std::string res = s1;
    if(s1.size() > 0 && s2.size() > 0){
        if(s1.back() != '/' && s2.front() != '/'){
            res += '/';
        }else if(s1.back() == '/' && s2.front() == '/'){
            res.resize(res.size()-1);
        }
    }
    res += s2;
    return res;
}

std::string buildPath(const std::string& s1,const std::string& s2,const std::string& s3){
    auto res = buildPath(s2,s3);
    return buildPath(s1,res);
}
std::string buildPath(const std::string& s1,const std::string& s2, const std::string& s3, const std::string& s4){
    auto res1 = buildPath(s1,s2);
    auto res2 = buildPath(s3,s4);
    return buildPath(res1,res2);
}

} //namesppace DBC
