#include <iostream>
#include <memory>

#include "ServerBody.h"
#include "ServerStub.h"

void LaptopFactory::
EngineerThread(std::unique_ptr<ServerSocket> socket, int id) {
    std::cout << "new thread starts" << std::endl;
    int msg_type;
    NodeConfig nodeConfig;
    ServerStub stub;

    stub.Init(std::move(socket));

    while (true) {
        // todo: request switch here
        nodeConfig = stub.receiveNodeConfig();
        if (!nodeConfig.isValid()) {
            // todo: it can either be an invalid config or a closed connection
            break;
        }
        nodeConfig.print();
    }
}

void LaptopFactory::startActiveThread() {

}

[[noreturn]] void LaptopFactory::startPassiveThread() {
    while (true) {

    }
}



