#include <iostream>
#include "ServerStub.h"

ServerStub::ServerStub() {}

void ServerStub::Init(std::unique_ptr<ServerSocket> socket) {
    this->socket = std::move(socket);
}

Membership ServerStub::receiveMembership() {
    char size_buffer[sizeof(int)];
    Membership members;
    if (socket->recv(size_buffer, sizeof(int), 0)) {
        members.unmarshalNumNodes(size_buffer);
    }
    // if check to skip 0
    if (members.getNumNodes() <= 0) {
        return members;
    }
    char node_buffer[members.expectedNodeByteSize()];
    if (socket->recv(node_buffer, members.expectedNodeByteSize(), 0)) {
        members.unmarshalMembers(node_buffer);
    }
    return members;
}

int ServerStub::sendMembership(Membership members) {
    char buffer[members.byteSize()];
    members.marshal(buffer);
    return socket->send(buffer, members.byteSize(), 0);
}
