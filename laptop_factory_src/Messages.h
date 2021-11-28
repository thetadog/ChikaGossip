#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <string>

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
