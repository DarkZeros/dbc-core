#ifndef LOGER_H
#define LOGER_H

#include <functional>
#include <vector>
#include <mutex>

namespace DBC {

#define DEFAULT_MIN_LOGLEVEL DEBUG

enum LogLevel{
    DEBUG = 'D',
    INFO = 'I',
    WARNING = 'W',
    ERROR = 'E',
    NOLEVEL = '?'
};

typedef std::function<void(LogLevel, const std::string &)> callback_log_function;

class Logger {
    static std::vector<std::pair<std::string, callback_log_function>> mFordwards;
    static std::mutex mMutex;
    static void forward(LogLevel level, const std::string& str);

    std::string mID;
    bool mAttached = false;
public:
    static bool also_stdout;
    static LogLevel min_loglevel;

    static void log(LogLevel level, const char * fmt, ...);
    static void log(const char * fmt, ...);
    static void add(const std::string& id, const callback_log_function& callback);
    static void remove(const std::string& id);

    Logger();
    Logger(const std::string& id);
    Logger(const std::string& id, const callback_log_function& callback);
    ~Logger();
    void attach(const callback_log_function& callback);
};

} //namespace DBC

#endif // LOGER_H
