#include <array>
#include <iostream>
#include <iomanip>
#include <thread>
#include <vector>

#include "ClientSocket.h"
#include "ClientBody.h"
#include "ClientTimer.h"

int main(int argc, char *argv[]) {
    std::string ip;
    int port;
    int num_customers;
    int num_orders;
    int laptop_type;
    ClientTimer timer;

    std::vector<std::shared_ptr<ClientThreadClass>> client_vector;
    std::vector<std::thread> thread_vector;

    if (argc < 3) {
        std::cout << "not enough arguments" << std::endl;
        std::cout << argv[0] << "[ip] [port #]" << std::endl;
        return 0;
    }

    ip = argv[1];
    port = atoi(argv[2]);


    timer.Start();
    for (int i = 0; i < 1; i++) {
        auto client_cls = std::shared_ptr<ClientThreadClass>(new ClientThreadClass());
        std::thread client_thread(&ClientThreadClass::ThreadBody, client_cls,
                                  ip, port, i, 1, 1);

        client_vector.push_back(std::move(client_cls));
        thread_vector.push_back(std::move(client_thread));
    }
    for (auto &th: thread_vector) {
        th.join();
    }
    timer.End();

    for (auto &cls: client_vector) {
        timer.Merge(cls->GetTimer());
    }
    timer.PrintStats();

    return 1;
}
