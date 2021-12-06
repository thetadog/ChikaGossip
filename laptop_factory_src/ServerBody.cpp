#include <iostream>
#include <memory>
#include <random>
#include <algorithm>

#include "ServerBody.h"
#include "ServerStub.h"


void ServerNode::init(std::string ip, int port) {
    self = NodeConfig(std::move(ip), port);
    // todo: push's membership should avoid sending to self
    local_membership.addMember(self);
//    local_membership.addMember(NodeConfig("127.0.0.1", 1));
//    local_membership.addMember(NodeConfig("127.0.0.1", 2));
//    local_membership.addMember(NodeConfig("127.0.0.1", 3));

    hot_rumor.emplace_back(self);
//    hot_rumor.emplace_back("127.0.0.1", 1);
//    hot_rumor.emplace_back("127.0.0.1", 2);
//    hot_rumor.emplace_back("127.0.0.1", 3);
}

void ServerNode::startPassiveThread(std::unique_ptr<ServerSocket> socket) {
    ServerStub stub;
    Message message;

    stub.init(std::move(socket));

    while (true) {
        message = stub.receiveMessage();

        if (!message.isValid()) {
            // std::cout << "invalid message received" << std::endl;
            break;
        }

        // todo: this step can be optimized with merge
        switch (message.getType()) {
            case PULL_MESSAGE:
                // todo: add msg.membership to hot and local
                hot_lock.lock();
                for (const NodeConfig &node: message.membership.members) {
                    // todo: multi pull from the same node will cause hot_rumor to repeat
                    //  but this is acceptable, it will be removed when send
                    local_membership.addMember(node);
                    hot_rumor.push_back(node);
                }
                hot_lock.unlock();
                // todo: send local_membership to new node
                stub.replyPull(local_membership);
                break;
            case PUSH_MESSAGE:
                // todo: merge PUSH with local_membership
                // todo: only add node to hot_rumor if node is not in local_membership
                for (const NodeConfig &node: message.membership.members) {
                    if (local_membership.members.count(node) == 0) {
                        hot_lock.lock();
                        local_membership.addMember(node);
                        hot_rumor.push_back(node);
                        hot_lock.unlock();
                    }
                }
                break;
            default:
                break;
        }

        std::cout << "........................Membership State........................" << std::endl;
        local_membership.print();
    }
}

[[noreturn]] void ServerNode::startActiveThread(const std::string &e_ip, int e_port) {
    // todo: send pull message once before push messages if there's an existing node
    if (e_port != -1) {
        ServerStub stub;
        stub.init(NodeConfig(e_ip, e_port));

        Message pullMessage;
        pullMessage.setType(PULL_MESSAGE);
        pullMessage.setPull(self);

        Membership received_membership = stub.pull(pullMessage);
        local_membership.merge(received_membership);
        stub.close();
    }

    while (true) {
        // send hot rumors to random nodes every 10s
//        std::cout << "........................active thread sleeping........................" << std::endl;
        std::this_thread::sleep_for(std::chrono::microseconds(20000000));
        if (!hot_rumor.empty()) {
            Message pushMessage;
            pushMessage.setType(PUSH_MESSAGE);
            Membership hot_rumors;
            // todo: move rumor dice to top so it can be unlocked without locking the entire message sent process
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            srand(seed);
            hot_lock.lock();
            for (auto &i: hot_rumor) {
                hot_rumors.addMember(i);
            }
            for (int i = 0; i < hot_rumor.size(); i++) {
                if (rand() % DICE == 0) {
                    //  std::cout << "DICE! Kicking ";
                    //  hot_rumor[i].print();
                    std::swap(hot_rumor[i], hot_rumor.back());
                    hot_rumor.pop_back();
                }
            }
            hot_lock.unlock();
            pushMessage.setPush(hot_rumors);
            // std::cout << "push message created:" << std::endl;
            // pushMessage.print();

            // todo: this copy & paste to vector can be optimized
            std::vector<NodeConfig> shuffledNodes(local_membership.members.begin(),
                                                  local_membership.members.end());
            // randomly select f shuffledNodes, where f is the min of size of shuffledNodes - 1 or FAN_OUT
            std::shuffle(shuffledNodes.begin(), shuffledNodes.end(), std::default_random_engine(seed));
            for (int i = 0, index = 0; i < std::min(int(shuffledNodes.size() - 1), FAN_OUT); i++, index++) {
                ServerStub stub;
                if (shuffledNodes[index] == self) {
                    // if itself is rolled into the list, simply ignore and seek another
                    i--;
                    continue;
                }
                // todo: create connection with ith node
                // std::cout << "connecting with ";
                // shuffledNodes[index].print();
                if (!stub.init(shuffledNodes[index])) {
                    // if current node is unreachable, try next node
                    continue;
                }
                // todo: send message to ith node
                stub.push(pushMessage);
                stub.close();
            }

        }

    }
}