#ifndef SERVER_H
#define SERVER_H
#include "muduo/net/TcpServer.h"
#include "muduo/net/EventLoop.h"

#include <boost/noncopyable.hpp>
class Server : public boost::noncopyable
{
public:
    Server(muduo::net::EventLoop* loop,
            const muduo::net::InetAddress& listenAddr);
    ~Server();
    void start();
private:
    void onConnection(const muduo::net::TcpConnectionPtr& conn);
    void onMessage(const muduo::net::TcpConnectionPtr& conn,
            muduo::net::Buffer* buf,
            muduo::Timestamp time);
    muduo::net::EventLoop* loop_;
    muduo::net::TcpServer server_;
};
#endif
