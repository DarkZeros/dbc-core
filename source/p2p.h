#ifndef P2P_H
#define P2P_H

namespace DBC {

class P2P {
    static constexpr char * mPort = "11250";
    static constexpr int mListenQueue = 10;

public:
    P2P();
    ~P2P();

    bool start();
    bool stop();

    int thread_loop(void);

};

} //namesppace DBC

#endif // P2P_H
