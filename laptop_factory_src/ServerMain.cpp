#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "ServerSocket.h"
#include "ServerBody.h"

int main(int argc, char *argv[]) {
	int port;
	int engineer_cnt = 0;
	int num_experts;
	ServerSocket socket;
    std::unique_ptr<ServerSocket> new_socket;
    LaptopFactory factory;
    std::vector<std::thread> thread_vector;

	if (argc < 3) {
		std::cout << "not enough arguments" << std::endl;
		std::cout << argv[0] << "[port #] [# experts]" << std::endl;
		return 0;
	}
	port = atoi(argv[1]);

	if (!socket.Init(port)) {
		std::cout << "Socket initialization failed" << std::endl;
		return 0;
	}

	while ((new_socket = socket.Accept())) {
        std::thread engineer_thread(&LaptopFactory::EngineerThread,
				&factory, std::move(new_socket), 
				engineer_cnt++);
		thread_vector.push_back(std::move(engineer_thread));
	}
	return 0;
}
