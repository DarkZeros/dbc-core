#ifndef TIMER_H
#define TIMER_H

/* This class can receive work in the shape of a std::function()
 * And it will call it periodically until destroyed
 */

#include <stdint.h>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>

namespace DBC {

class Timer {
    struct Entry{
        uint32_t rate, counter;
        std::function<void()> func;
    };
    std::vector<Entry> mEntries;

    std::thread mThread;
    std::mutex mThreadMutex;
    std::condition_variable mThreadCv;
    std::atomic<bool> bThreadRunning;

    void thread_loop();
public:
    Timer();
    ~Timer();

    //Creates the thread and starts firing
    void Start();
    void Stop();

    void AddEntry(std::function<void()>& func, uint32_t rate_ms, uint32_t first_delay = 0);

};

} //namesppace DBC

#endif // Timer_H
