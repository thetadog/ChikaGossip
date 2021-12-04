#include "ClientStub.h"
#include <iostream>

ClientStub::ClientStub() {}

int ClientStub::Init(std::string ip, int port) {
    return socket.Init(ip, port);
}

int ClientStub::sendNodeConfig(NodeConfig nodeConfig) {
    char buffer[32];
    nodeConfig.marshal(buffer, 0);
    return socket.send(buffer, nodeConfig.size(), 0);
}

