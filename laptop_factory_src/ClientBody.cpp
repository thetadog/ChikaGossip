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
    std::set<NodeConfig> member_set;

    NodeConfig n1 = NodeConfig(ip, 1);
    NodeConfig n2 = NodeConfig(ip, 2);
    NodeConfig n3 = NodeConfig(ip, 3);
    NodeConfig n4 = NodeConfig(ip, 3);
    NodeConfig n5 = NodeConfig("192.108.1.1", 3);

    member_set.insert(n1);
    member_set.insert(n2);
    member_set.insert(n3);
    member_set.insert(n4);
    member_set.insert(n5);
    std::cout << member_set.size() << std::endl;
    for (NodeConfig n: member_set) {
        n.print();
    }

//    Membership m;
//    m.addMember(n1);
//    m.addMember(n2);
//    m.addMember(n3);
//    stub.sendMembership(m);

    std::this_thread::sleep_for(std::chrono::microseconds(5000000));
}

ClientTimer ClientThreadClass::GetTimer() {
    return timer;
}

