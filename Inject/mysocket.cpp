#include "mysocket.h"
#include <iostream>

using namespace std;

int Socket::nofSockets_ = 0;

void Socket::Start() 
{
    if (!nofSockets_) 
    {
        WSADATA info;
        if (WSAStartup(MAKEWORD(2, 0), &info))
        {
            throw "Could not start WSA";
        }
    }
    ++nofSockets_;
}

void Socket::End() {
    WSACleanup();
}

Socket::Socket() : s_(0) 
{
    Start();
    // UDP: use SOCK_DGRAM instead of SOCK_STREAM
    s_ = socket(AF_INET, SOCK_STREAM, 0);

    if (s_ == INVALID_SOCKET)
    {
        throw "INVALID_SOCKET";
    }

    refCounter_ = new int(1);
}

Socket::Socket(SOCKET s) : s_(s) 
{
    Start();
    refCounter_ = new int(1);
};

Socket::~Socket()
{
    if (!--(*refCounter_)) 
    {
        Close();
        delete refCounter_;
    }

    --nofSockets_;
    if (!nofSockets_) End();
}

Socket::Socket(const Socket& o) 
{
    refCounter_ = o.refCounter_;
    (*refCounter_)++;
    s_ = o.s_;

    nofSockets_++;
}

Socket& Socket::operator=(Socket& o)
{
    (*o.refCounter_)++;

    refCounter_ = o.refCounter_;
    s_ = o.s_;

    nofSockets_++;

    return *this;
}

void Socket::Close() 
{
    closesocket(s_);
}


std::string Socket::ReceiveLine() 
{
    std::string ret;
    while (1) 
    {
        char r;

        switch (recv(s_, &r, 1, 0)) 
        {
        case 0: 
            return ret;
        case -1:
            return "-1";
        }

        ret += r;
        if (r == '\n')  return ret;
    }
}

void Socket::SendLine(std::string s) 
{
    s += '\n';
    send(s_, s.c_str(), static_cast<int>(s.length()), 0);
}



SocketServer::SocketServer(int port, int connections, TypeSocket type) 
{
    sockaddr_in sa;

    memset(&sa, 0, sizeof(sa));

    sa.sin_family = PF_INET;
    sa.sin_port = htons(port);
    s_ = socket(AF_INET, SOCK_STREAM, 0);
    if (s_ == INVALID_SOCKET) 
    {
        throw "INVALID_SOCKET";
    }

    if (type == NonBlockingSocket) 
    {
        u_long arg = 1;
        ioctlsocket(s_, FIONBIO, &arg);
    }

    /* bind the socket to the internet address */
    if (bind(s_, (sockaddr*)&sa, sizeof(sockaddr_in)) == SOCKET_ERROR) 
    {
        closesocket(s_);
        throw "INVALID_SOCKET";
    }

    listen(s_, connections);
}

Socket* SocketServer::Accept() 
{
    SOCKET new_sock = accept(s_, 0, 0);
    if (new_sock == INVALID_SOCKET) 
    {
        int rc = WSAGetLastError();
        if (rc == WSAEWOULDBLOCK) 
        {
            return 0; // non-blocking call, no request pending
        }
        else 
        {
            throw "Invalid Socket";
        }
    }

    Socket* r = new Socket(new_sock);
    return r;
}

