#include "server.h"
#include "muduo/base/Logging.h"
#include "boost/bind.hpp"
#include "boost/function.hpp"
#include "chatmessage.h"
#include <assert.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/reader.h"
#include "rapidjson/error/en.h"
#include <iostream>
using namespace std;
using namespace rapidjson;

struct SimpleHandler {
    bool Null() { cout << "Null()" << endl; return true; }
    bool Bool(bool b) { cout << "Bool(" << boolalpha << b << ")" << endl; return true; }
    bool Int(int i) { cout << "Int(" << i << ")" << endl; return true; }
    bool Uint(unsigned u) { cout << "Uint(" << u << ")" << endl; return true; }
    bool Int64(int64_t i) { cout << "Int64(" << i << ")" << endl; return true; }
    bool Uint64(uint64_t u) { cout << "Uint64(" << u << ")" << endl; return true; }
    bool Double(double d) { cout << "Double(" << d << ")" << endl; return true; }
    bool String(const char* str, SizeType length, bool copy) { 
        cout << "String(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        return true;
    }
    bool StartObject() { cout << "StartObject()" << endl; return true; }
    bool Key(const char* str, SizeType length, bool copy) {
        cout << "Key(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        return true;
    }
    bool EndObject(SizeType memberCount) { cout << "EndObject(" << memberCount << ")" << endl; return true; }
    bool StartArray() { cout << "StartArray()" << endl; return true; }
    bool EndArray(SizeType elementCount) { cout << "EndArray(" << elementCount << ")" << endl; return true; }
};
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
    //TODO: 增加分包逻辑
    //TODO: 增加回调处理
    //
    LOG_INFO << buf->findCRLF();
    std::string temp(buf->findCRLF());
    // TODO: 解析json
    //rapidjson::Document doc;
    //doc.Parse(buf->findCRLF());
    //rapidjson::Value& type = doc["type"];
    //LOG_INFO << type.GetString();
    LOG_INFO << temp<<temp.size();
    
    //parseJson();
    // messageCallBack(conn, temp, time);
    std::string msg(buf->peek(), buf->readableBytes());

    LOG_INFO << conn->name() << " ChatServer " << msg.size() << " bytes, "
        << "data received at " << time.toString();
    rapidjson::Document doc;
    SimpleHandler jsonHandler;
    rapidjson::Reader reader;
    //rapidjson::StringStream ss(msg.c_str());
    char jsonBuf[sizeof(msg.c_str())];
memcpy(jsonBuf,msg.c_str(),buf->readableBytes());
    LOG_INFO << jsonBuf << " size: "<< buf->readableBytes();
    if (!doc.ParseInsitu(jsonBuf).HasParseError()) {
    //if (reader.Parse(ss, jsonHandler)) {

        //TODO: dispose
	    rapidjson::Value& type = doc["type"];
	    LOG_INFO << type.GetString();
	    conn->send(msg);
	    MsgManager* MM = MsgManager::instance();
	    assert(MM);
	    int sender = -1;
	    int receiver = -1;
	    MM->insert(msg, sender, receiver);
	    buf->retrieve(buf->readableBytes());
    } else {
        LOG_ERROR << "CANNOT PARSE JSON"<<msg;
    }
}
 
