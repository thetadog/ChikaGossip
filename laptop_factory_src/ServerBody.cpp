#include <iostream>
#include <memory>

#include "ServerBody.h"
#include "ServerStub.h"

void ServerNode::startPassiveThread(std::unique_ptr<ServerSocket> socket) {
    std::cout << "new thread starts" << std::endl;
    NodeConfig nodeConfig;
    Membership m;
    ServerStub stub;

    stub.Init(std::move(socket));

    while (true) {
        // todo: request switch here

//        nodeConfig = stub.receiveNodeConfig();
        m = stub.receiveMembership();

        if (!m.isValid()) {
            std::cout << "invalid membership received" << std::endl;
            std::cout << "getNumNodes=" << m.getNumNodes() << " getMemberSize=" << m.getMemberSize() << std::endl;
            break;
        }
//        if (!nodeConfig.isValid()) {
//            // todo: it can either be an invalid config or a closed connection
//            break;
//        }
//        nodeConfig.print();
        m.print();
    }
}
