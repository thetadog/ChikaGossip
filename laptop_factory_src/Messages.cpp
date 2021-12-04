#include <cstring>
#include <iostream>

#include <arpa/inet.h>
#include "Messages.h"

/**
 * NodeConfig Section
 */
bool NodeConfig::isValid() const {
    return port != -1;
}

void NodeConfig::marshal(char *buffer, unsigned int offset) {
    int net_port = htonl(port);

    memcpy(buffer + offset, ip.c_str(), MAX_IP_SIZE);
    offset += MAX_IP_SIZE;
    memcpy(buffer + offset, &net_port, sizeof(net_port));
}

void NodeConfig::unmarshal(char *buffer, unsigned int offset) {
    char net_ip[MAX_IP_SIZE];
    int net_port;

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

// this should always be a constant
int NodeConfig::byteSize() {
    return MAX_IP_SIZE + sizeof(port);
}

/**
 * Membership Section
 */
void Membership::addMember(const NodeConfig &newNode) {
    members.push_back(newNode);
    num_nodes++;
}

int Membership::getMemberSize() const {
    return this->members.size();
}

int Membership::getNumNodes() const {
    return this->num_nodes;
}

int Membership::expectedNodeByteSize() const {
    return (this->num_nodes * (new NodeConfig)->byteSize());
}

int Membership::byteSize() const {
    return (this->num_nodes * (new NodeConfig)->byteSize()) + sizeof(this->num_nodes);
}

void Membership::marshal(char *buffer) {
    int net_num_nodes = htonl(num_nodes);
    unsigned int offset = 0;
    memcpy(buffer + offset, &net_num_nodes, sizeof(net_num_nodes));
    offset += sizeof(net_num_nodes);
    for (auto &&node: members) {
        node.marshal(buffer, offset);
        offset += node.byteSize();
    }
}

void Membership::unmarshalNumNodes(char *buffer) {
    int net_num_nodes;
    memcpy(&net_num_nodes, buffer, sizeof(net_num_nodes));
    num_nodes = ntohl(net_num_nodes);
}

void Membership::unmarshalMembers(char *buffer) {
    unsigned int offset = 0;
    for (int i = 0; i < num_nodes; i++) {
        NodeConfig newNode;
        newNode.unmarshal(buffer, offset);
        offset += newNode.byteSize();
        members.push_back(newNode);
    }
}

bool Membership::isValid() const {
    return num_nodes > 0 && num_nodes == members.size();
}

void Membership::print() {
    for (auto &&node: members) {
        node.print();
    }
}