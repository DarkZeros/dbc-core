#include "timer.h"

namespace DBC {

Timer::Timer(){

}
Timer::~Timer(){
    Stop();
}

void Timer::thread_loop(){
    while(bThreadRunning){
        {
            std::unique_lock<std::mutex> lock(mThreadMutex);
            mThreadCv.wait(lock);
        }
        //Update counters since last time
        //Fire them, or sleep
    }
}


//Creates the thread and starts firing
void Timer::Start(){
    std::unique_lock<std::mutex> lock(mThreadMutex);
    if(!bThreadRunning){
        bThreadRunning = true;
        //mThread = std::thread(thread_loop());
    }
}
void Timer::Stop(){
    std::unique_lock<std::mutex> lock(mThreadMutex);
    if(bThreadRunning){
        bThreadRunning = false;
        mThread.join();
    }
}

void Timer::AddEntry(std::function<void()>& func, uint32_t rate_ms, uint32_t first_delay){
    std::unique_lock<std::mutex> lock(mThreadMutex);
}


/*WorkerPool::WorkerPool() {
    WorkerPool();
    ~WorkerPool();

    void AddThread();
    void RemoveThread();

    /// Get the ID of this thread instance
    /// @return The worker thread ID
    std::thread::id GetThreadId();

    /// Get the ID of the currently executing thread
    /// @return The current thread ID
    static std::thread::id GetCurrentThreadId();

    /// Add a message to thread queue.
    /// @param[in] data - thread specific information created on the heap using operator new.
    void PostMsg(const UserData* data);

private:
    WorkerThread(const WorkerThread&);
    WorkerThread& operator=(const WorkerThread&);

    /// Entry point for the worker thread
    void Process();

    /// Entry point for timer thread
    void TimerThread();

    std::vector<std::thread*> mThreads;
    std::queue<ThreadMsg*> mQueue;
    std::mutex mMutex;
    std::condition_variable mCv;
    std::atomic<bool> mTimerExit;
}*/

} //namesppace DBC
