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

    Message pullMessage;
    pullMessage.setType(PULL_MESSAGE);
    pullMessage.setPull(ip, 5);
    pullMessage.print();
    std::cout << "sending message..." << std::endl;

    Membership membership;
    membership = stub.pull(pullMessage);
    std::cout << "membership received:";
    membership.print();

//    Membership membership;
//    membership.addMember(NodeConfig(ip, 2)); // this won't be added
//    membership.addMember(NodeConfig(ip, 3)); // this won't be added
//    membership.addMember(NodeConfig(ip, 4));
//    membership.addMember(NodeConfig(ip, 5));
//
//    Message pushMessage;
//    pushMessage.setType(PUSH_MESSAGE);
//    pushMessage.setPush(membership);
//    pushMessage.print();
//    std::cout << "sending message..." << std::endl;
//    stub.push(pushMessage);

    std::this_thread::sleep_for(std::chrono::microseconds(5000000));
}

ClientTimer ClientThreadClass::GetTimer() {
    return timer;
}

