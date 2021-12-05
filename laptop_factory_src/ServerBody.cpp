#include <iostream>
#include <memory>
#include <random>

#include "ServerBody.h"
#include "ServerStub.h"


void ServerNode::init(std::string ip, int port) {
    self = NodeConfig(std::move(ip), port);
    // todo: push's membership should avoid sending to self
    local_membership.addMember(self);
    local_membership.addMember(NodeConfig("127.0.0.1", 1));
    local_membership.addMember(NodeConfig("127.0.0.1", 2));
    local_membership.addMember(NodeConfig("127.0.0.1", 3));

    hot_rumor.emplace_back(self);
    hot_rumor.emplace_back("127.0.0.1", 1);
    hot_rumor.emplace_back("127.0.0.1", 2);
    hot_rumor.emplace_back("127.0.0.1", 3);
}


void ServerNode::startPassiveThread(std::unique_ptr<ServerSocket> socket) {
    ServerStub stub;
    Message message;

    stub.Init(std::move(socket));

    while (true) {
        message = stub.receiveMessage();

        if (!message.isValid()) {
            std::cout << "invalid message received" << std::endl;
            break;
        }

        message.print();

        // todo: type switch here
        switch (message.getType()) {
            case PULL_MESSAGE:
                // todo: add msg.membership to hot and local
                for (const NodeConfig &node: message.membership.members) {
                    // todo: multi pull from the same node will cause hot_rumor to repeat
                    //  but this is acceptable
                    local_membership.addMember(node);
                    hot_rumor.push_back(node);
                }
                // todo: send local_membership to new node
                stub.sendMembership(local_membership);
                break;
            case PUSH_MESSAGE:
                // todo: merge PUSH with local_membership
                // todo: only add node to hot_rumor if node is not in local_membership
                for (const NodeConfig &node: message.membership.members) {
                    if (local_membership.members.count(node) == 0) {
                        local_membership.addMember(node);
                        hot_rumor.push_back(node);
                    }
                }
                break;
            default:
                break;
        }

        std::cout << "post-processing local_membership" << std::endl;
        local_membership.print();

        std::cout << "post-processing hot_rumor" << std::endl;
        for (NodeConfig node: hot_rumor) {
            node.print();
        }
    }
}

[[noreturn]] void ServerNode::startActiveThread() {
    ServerStub stub;

    // sleep for 1 min before sending any messages
//    std::this_thread::sleep_for(std::chrono::microseconds(60000000));

    while (true) {
        // send hot rumors to random nodes every 10s
        std::this_thread::sleep_for(std::chrono::microseconds(10000000));
        if (!hot_rumor.empty()) {
            // todo: this copy & paste to vector can be optimized
            std::vector<NodeConfig> nodes(local_membership.members.begin(),
                                          local_membership.members.end());
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::shuffle(nodes.begin(), nodes.end(), std::default_random_engine(seed));
            for (int i = 0, index = 0; i < std::min(int(nodes.size() - 1), FAN_OUT); i++, index++) {
                if (nodes[index] == self) {
                    // if itself is rolled into the list, simply ignore and seek another
                    i--;
                    continue;
                }
                // todo: create connection with ith node
                std::cout << "connecting with ";
                nodes[index].print();

                // todo: send message to ith node
            }
            // todo: after sending the message, roll dice to remove hot_rumor
            srand(seed);
            for (int i = 0; i < hot_rumor.size(); i++) {
                if (rand() % DICE == 0) {
                    std::cout << "DICE! Kicking ";
                    hot_rumor[i].print();
                    std::swap(hot_rumor[i], hot_rumor.back());
                    hot_rumor.pop_back();
                }
            }
            std::cout << "hot rumors left..." << std::endl;
            for (NodeConfig node : hot_rumor) {
                node.print();
            }
        }

    }
}