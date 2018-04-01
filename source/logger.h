#ifndef LOGER_H
#define LOGER_H

#include <functional>
#include <vector>
#include <mutex>

namespace DBC {

enum LogLevel{
    DEBUG = 'D',
    INFO = 'I',
    WARNING = 'W',
    ERROR = 'E',
};

class Logger {
    static std::vector<std::function<void(const std::string &)>> mFordwards;
    static std::mutex mMutex;
    static void forward(const std::string& str);
public:
    static bool also_stdout;

    static void log(LogLevel level, const char * fmt, ...);
    static void add(std::function<void(const std::string &)>& callback);
    //static void remove();
};

} //namespace DBC

#endif // LOGER_H
