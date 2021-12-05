#ifndef __SERVER_STUB_H__
#define __SERVER_STUB_H__

#include <memory>

#include "ServerSocket.h"
#include "Messages.h"

class ServerStub {
private:
    std::unique_ptr<ServerSocket> socket = std::unique_ptr<ServerSocket>(new ServerSocket);
public:
    ServerStub();

    void init(std::unique_ptr<ServerSocket> socket);

    int init(NodeConfig config);

    bool isInitialized();

    void close();

    Message receiveMessage();

    Membership pull(Message message);

    int replyPull(Membership membership);

    int push(Message message);
};

#endif // end of #ifndef __SERVER_STUB_H__
