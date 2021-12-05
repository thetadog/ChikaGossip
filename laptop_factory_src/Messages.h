#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <string>
#include <utility>
#include <vector>
#include <set>

#define MAX_IP_SIZE 16

class NodeConfig {
private:
    std::string ip;
    int port = -1;
public:
    NodeConfig() = default;

    NodeConfig(std::string ip, int port) {
        this->ip = std::move(ip);
        this->port = port;
    }

    bool operator==(const NodeConfig &node) const {
        return (this->port == node.port) && (this->ip.compare(node.ip));
    }

    bool operator<(const NodeConfig &node) const {
        if (this->ip.compare(node.ip) < 0) {
            return true;
        }
        if (this->port < node.port) {
            return true;
        }
        return false;
    }

    bool operator>(const NodeConfig &node) const {
        if (this->ip.compare(node.ip) > 0) {
            return true;
        }
        if (this->port > node.port) {
            return true;
        }
        return false;
    }


    int byteSize();

    void marshal(char *buffer, unsigned int offset);

    void unmarshal(char *buffer, unsigned int offset);

    bool isValid() const;

    void print();
};

class Membership {
private:
    // todo: num_nodes affect the size of marshal/unmarshal
    //  therefore this only should be changed by addMember() and no deletion is allowed
    int num_node = 0;
    std::set<NodeConfig> members;
public:
    Membership() = default;

    void addMember(const NodeConfig &newNode);

    int getMemberSize() const;

    int getNumNodes() const;

    int expectedNodeByteSize() const;

    int byteSize() const;

    void marshal(char *buffer);

    void unmarshalNumNodes(char *buffer);

    void unmarshalMembers(char *buffer);

    bool isValid() const;

    void print();
};

#endif // #ifndef __MESSAGES_H__
