#include "ClientBody.h"
#include "Messages.h"

#include <iostream>
#include <thread>

ClientThreadClass::ClientThreadClass() {}

void ClientThreadClass::
ThreadBody(std::string ip, int port, int id, int orders, int type) {
    customer_id = id;
    if (!stub.Init(ip, port)) {
        std::cout << "Thread " << customer_id << " failed to connect" << std::endl;
        return;
    }
    NodeConfig nodeConfig = NodeConfig(std::move(ip), port);
    stub.sendNodeConfig(nodeConfig);
    std::this_thread::sleep_for(std::chrono::microseconds(5000000));
}

ClientTimer ClientThreadClass::GetTimer() {
    return timer;
}

