#include "ClientStub.h"
#include <iostream>

ClientStub::ClientStub() {}

int ClientStub::Init(std::string ip, int port) {
    return socket.Init(ip, port);
}

Membership ClientStub::pull(Message message) {
    Membership membership;
    char buffer[message.byteSize()];
    message.marshal(buffer);
    if (socket.send(buffer, message.byteSize(), 0)) {
        char size_buffer[sizeof(int)];
        if (socket.recv(size_buffer, sizeof(int), 0)) {
            membership.unmarshalNumNodes(size_buffer);
        }
        // if check to skip 0
        if (membership.getNumNodes() <= 0) {
            return membership;
        }
        char node_buffer[membership.membersByteSize()];
        if (socket.recv(node_buffer, membership.membersByteSize(), 0)) {
            membership.unmarshalMembers(node_buffer);
        }
    }
    return membership;
}

int ClientStub::push(Message message) {
    char buffer[message.byteSize()];
    message.marshal(buffer);
    return socket.send(buffer, message.byteSize(), 0);
}
