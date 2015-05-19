#include "chatmessage.h"
#include "muduo/base/Logging.h"
#include "server.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/reader.h"
#include "rapidjson/error/en.h"
ChatMessage::ChatMessage(int id, const std::string& content, int sender, int receiver)
    :id_(id)
     ,content_(content)
     ,sender_(sender)
     ,receiver_(receiver)
{
    
}

ChatMessage::~ChatMessage()
{

}

MsgManager* MsgManager::g_msgManager_ = 0;
MsgManager::MsgManager()
{
    assert(!g_msgManager_);
    g_msgManager_ = this;
    max_id_ = 0;
}
MsgManager::~MsgManager()
{
}
void MsgManager::insert(const std::string& content, int sender, int receiver)
{
    ++max_id_;
    boost::shared_ptr<ChatMessage> msg(new ChatMessage(max_id_, content, sender, receiver));
    std::pair<std::map<int, boost::shared_ptr<ChatMessage> >::iterator, bool> it;
    it = msgList_.insert(std::make_pair(msg->id(),msg));
    if (!it.second) {
        std::cout << "insert message ERROR!";
    }

    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator(); 
    doc.SetObject();
    doc.AddMember("type","new_msg",allocator);
    //rapidjson::Value typeValue = "new_msg";
    rapidjson::Value data;
    data.SetArray();
    rapidjson::Value oneData;
    oneData.SetObject();
    rapidjson::Value idValue;
    idValue= "101";
  
    rapidjson::Value contentValue;
    contentValue.SetString(content.c_str(),allocator);
    
    rapidjson::Value senderValue;
    senderValue = sender;
    rapidjson::Value receiverValue;
    receiverValue= receiver;
   
   
    oneData.AddMember("id",idValue,allocator);
    oneData.AddMember("content",contentValue,allocator);
    oneData.AddMember("sender",senderValue,allocator);
    oneData.AddMember("receiver",receiverValue,allocator);
    
    
    data.PushBack(oneData,allocator);
    doc.AddMember("data",data,allocator);
  
    rapidjson::StringBuffer buffer;  
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);  
    doc.Accept(writer);  
    std::string reststring = buffer.GetString(); 
    tempMessage_ = reststring + "\r\n";

    Server* server = Server::instance();
    server->callbackAllConnection(boost::bind(&MsgManager::broadcast,this,_1));
    tempMessage_ = "";
}

void MsgManager::broadcast(const muduo::net::TcpConnectionPtr& conn)
{
/*
 *
{
    "type" : "new_msg", //消息类型，客户端向其他人发送消息，如梁杰发消息问晚上谁开黑
    "data" : 
    [
    {
    "id":"101", "content":"晚上谁开黑",
    "sender":"liangjie"
    "receiver":"group202"
    }
    ]
}
 * */
       conn->send(tempMessage_);
       LOG_INFO<<"broadcast send:"<<tempMessage_;
}
