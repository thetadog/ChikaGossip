#ifndef __SERVERTHREAD_H__
#define __SERVERTHREAD_H__

#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <map>

#include "Messages.h"
#include "ServerStub.h"
#include "ServerSocket.h"

struct ExpertRequest {
    LaptopInfo laptop;
    std::promise<LaptopInfo> prom;
};

class ServerNode {
private:
    // two vectors should have the same index
    std::vector<ServerStub> stubs;
    std::vector<NodeConfig> configs;

    // local storage
    NodeConfig self; // this is needed to send to other nodes
    Membership local_membership;

    std::queue<std::unique_ptr<ExpertRequest>> erq;
    std::mutex erq_lock;
    std::condition_variable erq_cv;
public:
    void init();

    void startActiveThread(std::unique_ptr<ServerSocket> socket);

    [[noreturn]] void startPassiveThread();
};

#endif // end of #ifndef __SERVERTHREAD_H__

