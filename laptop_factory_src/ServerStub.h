#ifndef __SERVER_STUB_H__
#define __SERVER_STUB_H__

#include <memory>

#include "ServerSocket.h"
#include "Messages.h"

class ServerStub {
private:
//    std::unique_ptr<ServerSocket> socket = std::unique_ptr<ServerSocket>(new ServerSocket);
    std::unique_ptr<ServerSocket> socket;
public:
    ServerStub();

    void Init(std::unique_ptr<ServerSocket> socket);

    Membership receiveMembership();

    int sendMembership(Membership members);
};

#endif // end of #ifndef __SERVER_STUB_H__
