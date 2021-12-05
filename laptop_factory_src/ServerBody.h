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

class ServerNode {
private:
    NodeConfig self;
    Membership local_membership;
    // use vector so we can delete with ease
    std::vector<NodeConfig> hot_rumor;
    std::mutex hot_lock;
public:
    void init(std::string ip, int port);

    // todo: this thread can be started multiple time (i.e. on receive of new socket)
    void startPassiveThread(std::unique_ptr<ServerSocket> socket);

    // todo: this thread should only be started once
    [[noreturn]] void startActiveThread(const std::string& e_ip, int e_port);
};

#endif // end of #ifndef __SERVERTHREAD_H__

