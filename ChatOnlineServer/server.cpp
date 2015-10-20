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
Server* Server::g_server_ = 0;
Server::Server(muduo::net::EventLoop* loop,
            const muduo::net::InetAddress& listenAddr)
    : loop_(loop)
    , server_(loop, listenAddr, "ChatServer")
    , handle_(0)
{
    assert(!g_server_);
    g_server_ = this;
    server_.setConnectionCallback(boost::bind(&Server::onConnection, this, _1));
    server_.setMessageCallback(boost::bind(&Server::onMessage, this, _1, _2, _3));
    handle_ = new Handle(this);
    server_.setThreadNum(4);

}
Server::~Server()
{
    delete handle_;

}
void Server::start()
{
    server_.start();
}

void Server::callbackAllConnection(boost::function<void(const muduo::net::TcpConnectionPtr&)> callback)
{
    for (int i = 0; i < allConnection_.size(); ++i) {
        callback(allConnection_[i]);
        //if (allConnection_[i] == conn) index = i;
    }
    
}
void Server::onConnection(const muduo::net::TcpConnectionPtr& conn)
{
    static int max = 0;

    LOG_INFO << "ChatServer - " << conn->peerAddress().toIpPort()<< " -> "
        << conn->localAddress().toIpPort() << "is " << (conn->connected()?"UP":"DOWN");

    muduo::MutexLockGuard lock(mutex_);
    if (conn->connected()) {
        max++;
        allConnection_.push_back(conn);

        LOG_INFO<<"MAX CONNCTION NUMBER IS : "<<max;
        LOG_INFO<<"CURRNET CONNCTION NUMBER IS : "<<allConnection_.size();
        // UP
        //boost::shared_ptr<Handle> handlePtr(new Handle(this));
        //handleMap_.insert(std::make_pair(conn, handlePtr ));
    } else {
        int index = -1;
        for (int i = 0; i < allConnection_.size(); ++i) {
            if (allConnection_[i] == conn) index = i;
        }
        if (index != -1) allConnection_.erase(allConnection_.begin() + index);
        LOG_INFO<<"MAX CONNCTION NUMBER IS : "<<max;
        LOG_INFO<<"CURRNET CONNCTION NUMBER IS : "<<allConnection_.size();
        // UP
        // UP
        //std::map<muduo::net::TcpConnectionPtr, boost::shared_ptr<Handle> >::iterator it = handleMap_.find(conn);
        //if (it != handleMap_.end()) {
            //handleMap_.erase(it);
        //} else {
            //LOG_ERROR << "BIG WARNNING: CANNOT ERASE HANDLE. THIS WILL LEAK MEMERY";
        //}
    }

}
void Server::onMessage(const muduo::net::TcpConnectionPtr& conn,
            muduo::net::Buffer* buf,
            muduo::Timestamp time)
{

    muduo::MutexLockGuard lock(mutex_);
    if (!buf->findCRLF()) return;
    
    std::string msg(buf->peek(), buf->readableBytes());

    LOG_INFO << conn->name() << " ChatServer " << msg.size() << " bytes, "
        << "data received at " << time.toString();
    rapidjson::Document doc;
    //char jsonBuf[strlen(msg.c_str())];
    //memcpy(jsonBuf,msg.c_str(),msg.length());
    //LOG_INFO << jsonBuf << " strlen of jsonBuf: "<< strlen(jsonBuf);
    LOG_INFO << msg.c_str() << " strlen of msg: "<<strlen(msg.c_str());
    //LOG_INFO <<buf << " readableBytes: "<<buf->readableBytes();

    //doc.Parse<0>(jsonBuf);
    doc.Parse<0>(msg.c_str());
    rapidjson::Document doc2;

    if (!doc.HasParseError()) {
        if (doc.IsObject() &&doc.HasMember("type")) {

            rapidjson::Value& type = doc["type"];
            std::string handleType = type.GetString();
            LOG_INFO << "will dispose to -> "<<handleType;
            dispatcherHandle(conn, doc,time);

            //conn->send(msg);
            //MsgManager* MM = MsgManager::instance();
            //assert(MM);
            //int sender = -1;
            //int receiver = -1;
            //MM->insert(msg, sender, receiver);
            buf->retrieve(buf->readableBytes() );
        }
    } else {
        //LOG_ERROR << "CANNOT PARSE JSON:"<<jsonBuf;
        LOG_ERROR << "CANNOT PARSE JSON:"<<msg;
        LOG_ERROR <<"GetParseError:"<<doc.GetParseError()<<" "<< rapidjson::GetParseError_En(doc.GetParseError()) <<" offset:"<<doc.GetErrorOffset();  
        conn->shutdown();
    }
}
void Server::dispatcherHandle(const muduo::net::TcpConnectionPtr& conn,rapidjson::Document& doc,muduo::Timestamp time) 
{
    //rapidjson::Document doc;
    //SimpleHandler jsonHandler;
    //rapidjson::Reader reader;
    //rapidjson::StringStream ss(msg.c_str());
    //char jsonBuf[sizeof(json.c_str())];
//memcpy(jsonBuf,json.c_str(),json.size() - 2);
    //LOG_INFO << jsonBuf << " size: "<< json.size();
    //if (!doc.ParseInsitu(jsonBuf).HasParseError()) {
    if (!doc.HasParseError()) {
    //if (reader.Parse(ss, jsonHandler)) {

        //TODO: dispose
	    rapidjson::Value& type = doc["type"];
        std::string handleType = type.GetString();
	    //rapidjson::Value& type = doc["type"];
        //std::string handleType = type.GetString();
	
	    LOG_INFO << "actually process -> "<<handleType;
        
	    //conn->send(msg);
        // TODO: handle
        std::map<std::string, TcpCallBack >::iterator it = callBackMap_.find(handleType);
        if (it != callBackMap_.end()) {
            TcpCallBack callback = it->second;
            callback(conn,doc,time);
        } else {
            LOG_ERROR << "WRONG CALLBACK TYPE: " << handleType;
        }
	   /* MsgManager* MM = MsgManager::instance();*/
		//assert(MM);
		//int sender = -1;
		//int receiver = -1;
		/*MM->insert(handleType, sender, receiver);*/
    } else {
        LOG_ERROR << "CANNOT PARSE JSON";
    }
}

void Server::regCallBack(const std::string& type, TcpCallBack callBack)
{
    callBackMap_.insert(std::make_pair(type, callBack));
}
 
 
