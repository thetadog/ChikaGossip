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
    // two vectors should have the same index
    std::vector<ServerStub> stubs;
    std::vector<NodeConfig> configs;

    // local storage
    NodeConfig self; // this is needed to send to other nodes
    Membership local_membership;

public:
    void init();

    void startPassiveThread(std::unique_ptr<ServerSocket> socket);
};

#endif // end of #ifndef __SERVERTHREAD_H__

