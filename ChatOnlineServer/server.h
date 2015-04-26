#ifndef SERVER_H
#define SERVER_H
#include "muduo/net/TcpServer.h"
#include "muduo/net/EventLoop.h"

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include "abstract_handle.h"
class Server : public boost::noncopyable
{
public:
    Server(muduo::net::EventLoop* loop,
            const muduo::net::InetAddress& listenAddr);
    ~Server();
    void start();
    void regCallBack(const std::string& type, boost::function<void()> callBack);
private:
    void onConnection(const muduo::net::TcpConnectionPtr& conn);
    void onMessage(const muduo::net::TcpConnectionPtr& conn,
            muduo::net::Buffer* buf,
            muduo::Timestamp time);
    void disponseHandle(const muduo::net::TcpConnectionPtr& conn,
            const std::string& json, muduo::Timestamp time);
    muduo::net::EventLoop* loop_;
    muduo::net::TcpServer server_;
    std::map<std::string, boost::function<void(const muduo::net::TcpConnectionPtr&, rapidjson::Document,muduo::Timestamp)> > callBackMap_;
    std::map<muduo::net::TcpConnectionPtr, boost::shared_ptr<Handle> > handleMap_;
};
#endif
