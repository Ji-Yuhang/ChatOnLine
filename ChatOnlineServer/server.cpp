#include "server.h"
#include "muduo/base/Logging.h"
#include "boost/bind.hpp"
#include "boost/function.hpp"
Server::Server(muduo::net::EventLoop* loop,
            const muduo::net::InetAddress& listenAddr)
    : loop_(loop)
    , server_(loop, listenAddr, "ChatServer")
{
    server_.setConnectionCallback(boost::bind(&Server::onConnection, this, _1));
    server_.setMessageCallback(boost::bind(&Server::onMessage, this, _1, _2, _3));

}
Server::~Server()
{

}
void Server::start()
{
    server_.start();
}
void Server::onConnection(const muduo::net::TcpConnectionPtr& conn)
{
    LOG_INFO << "ChatServer - " << conn->peerAddress().toIpPort()<< " -> "
        << conn->localAddress().toIpPort() << "is " << (conn->connected()?"UP":"DOWN");
}
void Server::onMessage(const muduo::net::TcpConnectionPtr& conn,
            muduo::net::Buffer* buf,
            muduo::Timestamp time)
{
    muduo::string msg(buf->retrieveAllAsString());
    LOG_INFO << conn->name() << "ChatServer" << msg.size() << "bytes, "
        << "data received at " << time.toString();
    conn->send(msg);
}
 
