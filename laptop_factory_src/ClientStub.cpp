#include "ClientStub.h"
#include <iostream>

ClientStub::ClientStub() {}

int ClientStub::Init(std::string ip, int port) {
    return socket.Init(ip, port);
}

int ClientStub::sendMembership(Membership members) {
    char buffer[members.byteSize()];
    members.marshal(buffer);
    members.print();
    std::cout << members.byteSize() << std::endl;
    return socket.send(buffer, members.byteSize(), 0);
}

