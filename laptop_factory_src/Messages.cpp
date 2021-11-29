#include <cstring>
#include <iostream>

#include <arpa/inet.h>
#include "Messages.h"

bool NodeConfig::isValid() const {
    return port != -1;
}

void NodeConfig::marshal(char *buffer) {
    int net_port = htonl(port);
    unsigned int offset = 0;

    memcpy(buffer + offset, ip.c_str(), MAX_IP_SIZE);
    offset += MAX_IP_SIZE;
    memcpy(buffer + offset, &net_port, sizeof(net_port));
}

void NodeConfig::unmarshal(char *buffer) {
    char net_ip[MAX_IP_SIZE];
    int net_port;
    unsigned int offset = 0;

    memcpy(&net_ip, buffer + offset, MAX_IP_SIZE);
    offset += MAX_IP_SIZE;
    memcpy(&net_port, buffer + offset, sizeof(net_port));

    ip.assign(net_ip);
    port = ntohl(net_port);
}

void NodeConfig::print() {
    std::cout << "ip " << ip << " ";
    std::cout << "port " << port << std::endl;
}

int NodeConfig::size() {
    return MAX_IP_SIZE + sizeof(port);
}

