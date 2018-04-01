#include "logger.h"

#include <stdarg.h>

namespace DBC {

std::vector<std::pair<std::string, callback_log_function>> Logger::mFordwards;
std::mutex Logger::mMutex;
bool Logger::also_stdout = true;
LogLevel Logger::min_loglevel = DEFAULT_MIN_LOGLEVEL;

Logger::Logger() {
}
Logger::Logger(const std::string& id) {
    mID = id;
}
Logger::Logger(const std::string& id, const callback_log_function& callback) {
    mID = id;
    attach(callback);
}
void Logger::attach(const callback_log_function& callback){
    mAttached = true;
    add(mID, callback);
}
Logger::~Logger(){
    if(mAttached)
        remove(mID);
}

void Logger::log(const char * fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log(NOLEVEL, fmt, args);
    va_end(args);
}

void Logger::log(LogLevel level, const char * fmt, ...) {
    if(level < min_loglevel) return;

    std::string str(4096, 0);
    va_list args;
    va_start(args, fmt);
    int chars = vsnprintf(&str[0], 4090, fmt, args);
    va_end(args);

    //Send it to the log
    forward(level, str);

    if(also_stdout){
        fprintf(stdout, "%c %s\n", (char)level, str.c_str());
    }
}

void Logger::forward(LogLevel level, const std::string& str) {
    std::unique_lock<std::mutex> lock(mMutex);
    for(auto& i : mFordwards){
        i.second(level, str);
    }
}

void Logger::add(const std::string& id, const callback_log_function& callback) {
    std::unique_lock<std::mutex> lock(mMutex);
    mFordwards.push_back(std::make_pair(id, callback));
}

void Logger::remove(const std::string& id) {
    std::unique_lock<std::mutex> lock(mMutex);
    for(auto it = mFordwards.begin(); it != mFordwards.end();){
        if(it->first == id){
            it = mFordwards.erase(it);
        }else{
            it ++;
        }
    }
}


} //namesppace DBC
