#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "ServerSocket.h"
#include "ServerBody.h"

int main(int argc, char *argv[]) {
    std::string ip;
    int port;

    std::string e_ip;
    int e_port = -1;

    ServerNode server;
    NodeConfig selfConfig;

    ServerSocket socket;
    std::unique_ptr<ServerSocket> new_socket;
    std::vector<std::thread> thread_vector;

    if (argc < 3) {
        std::cout << "not enough arguments" << std::endl;
        std::cout << argv[0] << "[ip #] [port #]" << std::endl;
        return 0;
    }
    ip = argv[1];
    port = atoi(argv[2]);

    if (argc == 5) {
        e_ip = argv[3];
        e_port = atoi(argv[4]);
    }

    if (!socket.Init(port)) {
        std::cout << "Socket initialization failed" << std::endl;
        return 0;
    }

    server.init(ip, port);

    std::thread active_thread(&ServerNode::startActiveThread, &server, std::move(e_ip), e_port);
    thread_vector.push_back(std::move(active_thread));

    while ((new_socket = socket.Accept())) {
        std::thread passive_thread(&ServerNode::startPassiveThread, &server, std::move(new_socket));
        thread_vector.push_back(std::move(passive_thread));
    }

    return 0;
}
