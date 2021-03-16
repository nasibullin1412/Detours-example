
#ifndef SOCKET_H
#define SOCKET_H

#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable  : 4996)


#include <string>

enum TypeSocket { BlockingSocket, NonBlockingSocket };

class Socket {
public:

    virtual ~Socket();
    Socket(const Socket&);
    Socket& operator=(Socket&);

    std::string ReceiveLine();

    void   Close();

    // The parameter of SendLine is not a const reference
    // because SendLine modifes the std::string passed.
    void   SendLine(std::string);


protected:
    friend class SocketServer;
    friend class SocketSelect;

    Socket(SOCKET s);
    Socket();

    SOCKET s_;

    int* refCounter_;

private:
    static void Start();
    static void End();
    static int  nofSockets_;
};


class SocketServer : public Socket {
public:
    SocketServer(int port, int connections, TypeSocket type = BlockingSocket);

    Socket* Accept();
};




#endif