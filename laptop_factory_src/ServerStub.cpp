#include <iostream>
#include "ServerStub.h"

ServerStub::ServerStub() {}

void ServerStub::Init(std::unique_ptr<ServerSocket> socket) {
    this->socket = std::move(socket);
}

Membership ServerStub::receiveMembership() {
    char size_buffer[sizeof(int)];
    Membership membership;
    if (socket->recv(size_buffer, sizeof(int), 0)) {
        membership.unmarshalNumNodes(size_buffer);
    }
    // if check to skip 0
    if (membership.getNumNodes() <= 0) {
        return membership;
    }
    char node_buffer[membership.membersByteSize()];
    if (socket->recv(node_buffer, membership.membersByteSize(), 0)) {
        membership.unmarshalMembers(node_buffer);
    }
    return membership;
}

int ServerStub::sendMembership(Membership membership) {
    char buffer[membership.byteSize()];
    membership.marshal(buffer, 0);
    return socket->send(buffer, membership.byteSize(), 0);
}

Message ServerStub::receiveMessage() {
    Message message;
    char type_buffer[sizeof(int)];
    if (socket->recv(type_buffer, sizeof(int), 0)) {
        message.unmarshalMessageType(type_buffer);
    }
    if (!message.isValid()) {
        return message;
    }
    char size_buffer[sizeof(int)];
    if (socket->recv(size_buffer, sizeof(int), 0)) {
        message.membership.unmarshalNumNodes(size_buffer);
    }

    // todo: this should never happen under current design
    if (message.membership.getNumNodes() <= 0) {
        return message;
    }

    char node_buffer[message.membership.membersByteSize()];
    if (socket->recv(node_buffer, message.membership.membersByteSize(), 0)) {
        message.membership.unmarshalMembers(node_buffer);
    }

    return message;
}

int ServerStub::sendMessage(Message message) {
    char buffer[message.byteSize()];
    message.marshal(buffer);
    return socket->send(buffer, message.byteSize(), 0);
}
