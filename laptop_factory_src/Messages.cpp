#include <cstring>
#include <iostream>
#include <utility>

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
int NodeConfig::byteSize() const {
    return MAX_IP_SIZE + sizeof(port);
}

/**
 * Membership Section
 */
int Membership::addMember(const NodeConfig &newNode) {
    auto res = members.insert(newNode);
    if (res.second) {
        // only increment when successfully inserted
        num_node++;
        return 1;
    }
    return 0;
}

int Membership::merge(const Membership& another) {
    int c = 0;
    for (const NodeConfig &n: another.members) {
        c += this->addMember(n);
    }
    return c;
}

int Membership::getMemberSize() const {
    return this->members.size();
}

int Membership::getNumNodes() const {
    return this->num_node;
}

int Membership::membersByteSize() const {
    return (this->num_node * (new NodeConfig)->byteSize());
}

int Membership::byteSize() const {
    return (this->num_node * (new NodeConfig)->byteSize()) + sizeof(this->num_node);
}

void Membership::marshal(char *buffer, int offset) {
    int net_num_nodes = htonl(num_node);
    memcpy(buffer + offset, &net_num_nodes, sizeof(net_num_nodes));
    offset += sizeof(net_num_nodes);
    for (NodeConfig node: members) {
        node.marshal(buffer, offset);
        offset += node.byteSize();
    }
}

void Membership::unmarshalNumNodes(char *buffer) {
    int net_num_nodes;
    memcpy(&net_num_nodes, buffer, sizeof(net_num_nodes));
    num_node = ntohl(net_num_nodes);
}

void Membership::unmarshalMembers(char *buffer) {
    unsigned int offset = 0;
    for (int i = 0; i < num_node; i++) {
        NodeConfig newNode;
        newNode.unmarshal(buffer, offset);
        offset += newNode.byteSize();
        members.insert(newNode);
    }
}

bool Membership::isValid() const {
    return num_node > 0 && num_node == members.size();
}

void Membership::print() {
    for (NodeConfig node: members) {
        node.print();
    }
}

/**
* Message Section
*/
int Message::getType() const {
    return message_type;
}

void Message::setType(int type) {
    message_type = type;
}

void Message::setPull(std::string ip, int port) {
    membership.addMember(NodeConfig(std::move(ip), port));
}

void Message::setPull(const NodeConfig &self) {
    membership.addMember(self);
}

void Message::setPush(Membership hot_rumor) {
    membership = std::move(hot_rumor);
}

int Message::byteSize() const {
    return sizeof(message_type) + membership.byteSize();
}

int Message::contentByteSize() const {
    return membership.byteSize();
}

void Message::marshal(char *buffer) {
    int net_type = htonl(message_type);
    int offset = 0;
    memcpy(buffer + offset, &net_type, sizeof(net_type));
    offset += sizeof(net_type);
    membership.marshal(buffer, offset);
}

void Message::unmarshalMessageType(char *buffer) {
    int net_type;
    memcpy(&net_type, buffer, sizeof(net_type));
    message_type = ntohl(net_type);
}

bool Message::isValid() const {
    return message_type > 0;
}

void Message::print() {
    std::cout << "Message: Type " << message_type << std::endl;
    membership.print();
}

/**
* PushMessage Section
*/
