#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <string>
#include <utility>
#include <vector>

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

    bool operator==(const NodeConfig &node) {
        return (this->port == node.port) && (this->ip.compare(node.ip));
    }

    bool operator<(const NodeConfig &node) {
        if (this->ip.compare(node.ip) < 0) {
            return true;
        }
        if (this->port < node.port) {
            return true;
        }
        return false;
    }

    bool operator>(const NodeConfig &node) {
        if (this->ip.compare(node.ip) > 0) {
            return true;
        }
        if (this->port > node.port) {
            return true;
        }
        return false;
    }

    int size();

    void marshal(char *buffer, unsigned int offset);

    void unmarshal(char *buffer, unsigned int offset);

    bool isValid() const;

    void print();
};

class Membership {
private:
    int num_nodes = 0;
    std::vector<std::unique_ptr<NodeConfig>> members;
public:
    Membership() = default;

    void addMember(std::unique_ptr<NodeConfig> newNode);

    int size() const;

    void marshal(char *buffer);

    void unmarshal(char *buffer);

    bool isValid() const;

    void print();
};

class PullRequest {
private:
    char data[256];
public:
    PullRequest();

    void operator=(const PullRequest &req) {

    }

    void setRequest();

    int size();

    void marshal(char *buffer);

    void unmarshal(char *buffer);

    bool isValid();

    void print();
};

class PullResponse {
private:
public:
};

class PushRequest {
private:
public:
    PushRequest();

    void operator=(const PushRequest &req) {

    }

    void setRequest();

    int size();

    void marshal(char *buffer);

    void unmarshal(char *buffer);

    bool isValid();

    void print();
};

class PushResponse {
private:
public:
};

class PushPullRequest {
private:
public:
    PushPullRequest();

    void operator=(const PushPullRequest &req) {

    }

    void setRequest();

    int size();

    void marshal(char *buffer);

    void unmarshal(char *buffer);

    bool isValid();

    void print();
};

class PushPullResponse {
private:
public:
};

class LaptopOrder {
private:
    int customer_id;
    int order_number;
    int laptop_type;

public:
    LaptopOrder();

    void operator=(const LaptopOrder &order) {
        customer_id = order.customer_id;
        order_number = order.order_number;
        laptop_type = order.laptop_type;
    }

    void SetOrder(int cid, int order_num, int type);

    int GetCustomerId();

    int GetOrderNumber();

    int GetLaptopType();

    int Size();

    void Marshal(char *buffer);

    void Unmarshal(char *buffer);

    bool IsValid();

    void Print();
};

class LaptopInfo {
private:
    int customer_id;
    int order_number;
    int laptop_type;
    int engineer_id;
    int expert_id;

public:
    LaptopInfo();

    void operator=(const LaptopInfo &info) {
        customer_id = info.customer_id;
        order_number = info.order_number;
        laptop_type = info.laptop_type;
        engineer_id = info.engineer_id;
        expert_id = info.expert_id;
    }

    void SetInfo(int cid, int order_num, int type, int engid, int expid);

    void CopyOrder(LaptopOrder order);

    void SetEngineerId(int id);

    void SetExpertId(int id);

    int GetCustomerId();

    int GetOrderNumber();

    int GetLaptopType();

    int GetEngineerId();

    int GetExpertId();

    int Size();

    void Marshal(char *buffer);

    void Unmarshal(char *buffer);

    bool IsValid();

    void Print();
};

#endif // #ifndef __MESSAGES_H__
