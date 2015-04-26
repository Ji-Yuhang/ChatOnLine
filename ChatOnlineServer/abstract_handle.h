#ifndef ABSTRACT_HANDLE_H
#define ABSTRACT_HANDLE_H
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/reader.h"
#include "rapidjson/error/en.h"
#include <iostream>
#include "server.h"
#include <boost/bind.hpp>
using namespace std;
using namespace rapidjson;


class AbstractHandle
{
public:
    AbstractHandle(){}
    ~AbstractHandle(){}

};

template<class T>
class TcpFormat
{
public:
    typedef T DataType;
};

class JsonFormat : public TcpFormat<rapidjson::Document>
{
public:
    JsonFormat(){}
    ~JsonFormat(){}
};

class XmlFormat : public TcpFormat<rapidjson::Document>
{
public:
    XmlFormat(){}
    ~XmlFormat(){}
};


class NewMessage : public JsonFormat
{
public:
    NewMessage(){}
    ~NewMessage(){}
    void process(const muduo::net::TcpConnectionPtr& conn, const DataType& data,muduo::Timestamp time)
    {
        std::cout << "This NewMessage Process function";
    
    }

};

class SendMessage : public JsonFormat
{
public:
    SendMessage(){}
    ~SendMessage(){}
    void process(const muduo::net::TcpConnectionPtr& conn, const DataType& data,muduo::Timestamp time)
    {
        std::cout << "This SendMessage Process function";
    
    }

};

class Handle : public AbstractHandle
{
public:
    Handle(Server* server)
        : server(server)
    {
        server_->regCallBack("new_msg", boost::bind(&NewMessage::process, &newMessage_, _1, _2, _3))
        server_->regCallBack("send_msg", boost::bind(&SendMessage::process, &sendMessage_, _1, _2, _3))
    }
    ~Handle(){}
private:
    Server* server_;
    NewMessage newMessage_;
    SendMessage sendMessage_;
};
#endif
