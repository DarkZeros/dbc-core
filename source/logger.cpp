#include "logger.h"

#include <stdarg.h>

namespace DBC {

std::vector<std::function<void(const std::string &)>> Logger::mFordwards;
std::mutex Logger::mMutex;
bool Logger::also_stdout = true;

void Logger::log(LogLevel level, const char * fmt, ...) {
    std::string str(4096, 0);
    str[0] = (char)level;
    str[1] = ' ';
    va_list args;
    va_start(args, fmt);
    int chars = vsnprintf(&str[2], 4090, fmt, args);
    va_end(args);

    //Send it to the log
    forward(str);

    if(also_stdout){
        fprintf(stdout, "%s\n", str.c_str());
    }
}

void Logger::forward(const std::string& str) {
    std::unique_lock<std::mutex> lock(mMutex);
    for(auto& i : mFordwards){
        i(str);
    }
}

void Logger::add(std::function<void(const std::string &)>& callback) {
    std::unique_lock<std::mutex> lock(mMutex);
    mFordwards.push_back(callback);
}


} //namesppace DBC
