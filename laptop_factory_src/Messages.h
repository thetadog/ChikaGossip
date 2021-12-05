#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <string>
#include <utility>
#include <vector>
#include <set>

#define MAX_IP_SIZE 16
#define PULL_MESSAGE 1
#define PUSH_MESSAGE 2
#define FAN_OUT 2
#define DICE 5

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

    std::string getIp() {
        return ip;
    }

    int getPort() const {
        return port;
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

    int byteSize() const;

    void marshal(char *buffer, unsigned int offset);

    void unmarshal(char *buffer, unsigned int offset);

    bool isValid() const;

    void print();
};

class Membership {
private:
    // todo: num_nodes affect the size of marshalPullMessage/unmarshalPullMessage
    //  therefore this only should be changed by addMember() and no deletion is allowed
    int num_node = 0;
public:
    std::set<NodeConfig> members;

    Membership() = default;

    int addMember(const NodeConfig &newNode);

    int merge(const Membership& another);

    int getMemberSize() const;

    int getNumNodes() const;

    int membersByteSize() const;

    int byteSize() const;

    void marshal(char *buffer, int offset);

    void unmarshalNumNodes(char *buffer);

    void unmarshalMembers(char *buffer);

    bool isValid() const;

    void print();
};

class Message {
private:
    int message_type = 0;
public:
    Membership membership;

    Message() = default;

    int getType() const;

    void setType(int type);

    void setPull(std::string ip, int port);

    void setPull(const NodeConfig &self);

    void setPush(Membership hot_rumor);

    int byteSize() const;

    int contentByteSize() const;

    void marshal(char *buffer);

    void unmarshalMessageType(char *buffer);

    bool isValid() const;

    void print();
};

class PushMessage {
private:
    int TYPE = PUSH_MESSAGE;
    Membership hot_rumors;
public:
    PushMessage() = default;

    bool isValid() const;

    void print();
};

#endif // #ifndef __MESSAGES_H__
