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
    NodeConfig n1 = NodeConfig(ip, 1);
    NodeConfig n2 = NodeConfig(ip, 2);
    NodeConfig n3 = NodeConfig(ip, 3);

    Membership m;
//    m.addMember(n1);
//    m.addMember(n2);
//    m.addMember(n3);
    stub.sendMembership(m);
//    stub.sendNodeConfig(nodeConfig);
    std::this_thread::sleep_for(std::chrono::microseconds(5000000));
}

ClientTimer ClientThreadClass::GetTimer() {
    return timer;
}

