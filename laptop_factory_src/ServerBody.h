#ifndef __SERVERTHREAD_H__
#define __SERVERTHREAD_H__

#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
#include <thread>

#include "Messages.h"
#include "ServerStub.h"
#include "ServerSocket.h"

struct ExpertRequest {
    LaptopInfo laptop;
    std::promise<LaptopInfo> prom;
};


class LaptopFactory {
private:
    // two vectors should have the same index
    std::vector<ServerStub> stubs;
    std::vector<NodeConfig> configs;

    std::queue<std::unique_ptr<ExpertRequest>> erq;
    std::mutex erq_lock;
    std::condition_variable erq_cv;
public:
    void init();

    void EngineerThread(std::unique_ptr<ServerSocket> socket, int id);

    void startActiveThread();

    [[noreturn]] void startPassiveThread();
};

#endif // end of #ifndef __SERVERTHREAD_H__

