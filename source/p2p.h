#ifndef P2P_H
#define P2P_H

#include <vector>
#include <map>
#include <mutex>
#include <atomic>
#include <thread>
#include <netdb.h>

namespace DBC {

struct ClientData{
    sockaddr_in addr;
    std::vector<uint8_t> rxBuf;
    bool mHandshake = false;
};

class P2P {
    static constexpr char * mPort = "11250";
    static constexpr int mListenQueue = 10;

    int mMainSocket = -1;
    int mEventFd = -1;
    std::map<int, ClientData> mClientSocket;

    std::atomic<bool> mRunning;
    std::thread mThread;
    std::mutex mThreadMutex, mDataMutex;
public:
    P2P();
    ~P2P();

    bool start();
    bool stop();

    int thread_loop(void);

    int getNumClients();
};

} //namesppace DBC

#endif // P2P_H
