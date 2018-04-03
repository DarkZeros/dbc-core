#ifndef P2P_H
#define P2P_H

#include <map>
#include <mutex>
#include <atomic>
#include <thread>
#include <netdb.h>

namespace DBC {

class P2P {
    static constexpr char * mPort = "11250";
    static constexpr int mListenQueue = 10;

    int mMainSocket = 0;
    std::map<int, sockaddr_in> mClientSocket;
    std::atomic<bool> mRunning;
    std::thread mThread;
public:
    P2P();
    ~P2P();

    bool start();
    bool stop();

    int thread_loop(void);

};

} //namesppace DBC

#endif // P2P_H
