#include <iostream>
#include "ServerStub.h"

ServerStub::ServerStub() {}

void ServerStub::Init(std::unique_ptr<ServerSocket> socket) {
    this->socket = std::move(socket);
}

NodeConfig ServerStub::receiveNodeConfig() {
    char buffer[32];
    NodeConfig node;
    if (socket->recv(buffer, node.size(), 0)) {
        node.unmarshal(buffer, 0);
    }
    return node;
}

int ServerStub::sendNodeConfig(NodeConfig nodeConfig) {
    char buffer[32];
    nodeConfig.marshal(buffer);
    return socket->send(buffer, nodeConfig.size(), 0);
}

