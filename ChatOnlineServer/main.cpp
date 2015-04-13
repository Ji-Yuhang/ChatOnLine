#include <iostream>
#include "server.h"
#include "chatmessage.h"
using namespace std;
MsgManager* msgManager = 0;
void init()
{
    msgManager = new MsgManager;
}


void release()
{
    delete msgManager;
}


int main()
{
    cout << "This is Chat On Line Server!" << endl;
    cout << "bind 2020, wait new client..." << endl;

    init();

    muduo::net::EventLoop loop;
    muduo::net::InetAddress listenAddr(2020);

    Server server(&loop, listenAddr);
    server.start();
    loop.loop();

    release();
    return 0;
}

