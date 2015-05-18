#ifndef HANDLE_H
#define HANDLE_H
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/reader.h"
#include "rapidjson/error/en.h"
#include <iostream>
#include <boost/bind.hpp>
#include "muduo/net/TcpServer.h"
#include "muduo/net/EventLoop.h"
#include "muduo/base/Logging.h"
#include "abstract_handle.h"


using namespace std;
using namespace rapidjson;
class NewMessage : public JsonFormat
{
public:
    NewMessage(){}
    ~NewMessage(){}
    void process(const muduo::net::TcpConnectionPtr& conn, DataType& data,muduo::Timestamp time);
};

class SendMessage : public JsonFormat
{
public:
    SendMessage(){}
    ~SendMessage(){}
    void process(const muduo::net::TcpConnectionPtr& conn, DataType& data,muduo::Timestamp time);

};

class Handle : public AbstractHandle
{
public:
    Handle(Server* server);
    ~Handle(){}
private:
    Server* server_;
    NewMessage newMessage_;
    SendMessage sendMessage_;
};
#endif
