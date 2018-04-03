#include "logger.h"

#include <stdarg.h>

namespace DBC {

std::vector<std::pair<std::string, callback_log_function>> Logger::mFordwards;
std::mutex Logger::mMutex;
constexpr const char * Logger::type2str[];
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
    log(NOTYPE, NOLEVEL, fmt, args);
    va_end(args);
}

void Logger::log(LogLevel level, const char * fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log(NOTYPE, level, fmt, args);
    va_end(args);
}

void Logger::log(LogType type, LogLevel level, const std::string & fmt, ...){
    va_list args;
    va_start(args, fmt);
    log(type, level, fmt.c_str(), args);
    va_end(args);
}

void Logger::log(LogType type, LogLevel level, const char * fmt, ...){
    if(level < min_loglevel) return;
    if(type >= LogType::LogType_SIZE) return;

    std::string str(4096, 0);
    va_list args;
    va_start(args, fmt);
    vsnprintf(&str[0], 4090, fmt, args);
    va_end(args);

    //Send it to the log
    forward(type, level, str);

    if(also_stdout){
        fprintf(stdout, "%s-%c: %s\n", type2str[type], (char)level, str.c_str());
    }
}


void Logger::forward(LogType type, LogLevel level, const std::string& str) {
    std::unique_lock<std::mutex> lock(mMutex);
    for(auto& i : mFordwards){
        i.second(type, level, str);
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
