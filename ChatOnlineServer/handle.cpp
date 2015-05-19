#include "abstract_handle.h"
#include "server.h"
#include "chatmessage.h"
void NewMessage::process(const muduo::net::TcpConnectionPtr& conn, DataType& data,muduo::Timestamp time)
{

    LOG_INFO << "NewMessage::process ";
    //std::cout << "This NewMessage Process function";
    std::string temp = "NewMessage\r\n";
    conn->send(temp);

}
void SendMessage::process(const muduo::net::TcpConnectionPtr& conn, DataType& data,muduo::Timestamp time)
{
/*
 *
{
    "type" : "send_msg", //消息类型，客户端向其他人发送消息，如梁杰发消息问晚上谁开黑
    "data" : 
    [
    {
    "id":"101",
    "content":"晚上谁开黑",
    "sender":"liangjie"
    "receiver":"group202"
    }
    ]
}
 * */

    LOG_INFO << "SendMessage::process ";
    //std::cout << "This SendMessage Process function"<<std::endl;

    rapidjson::Value::MemberIterator it = data.FindMember("data");
    
    if (it == data.MemberEnd()) {
        LOG_ERROR << "Have no member named data";
        conn->shutdown();
        return;
    }
    rapidjson::Value& block = it->value;
    //if (block.IsObject()) {
    if (block.IsArray()) {

        LOG_INFO << "data is array ";
        for (rapidjson::Value::ValueIterator it = block.Begin(); it != block.End(); ++it) {
            rapidjson::Value& one = (*it);
            if (one.HasMember("id")&&one.HasMember("content")&&one.HasMember("sender")&&one.HasMember("receiver")) {
                rapidjson::Value& _id = one["id"];
                rapidjson::Value& _content = one["content"];
                rapidjson::Value& _sender = one["sender"];
                rapidjson::Value& _receiver = one["receiver"];
                std::string id = _id.GetString();
                std::string content = _content.GetString();
                std::string sender = _sender.GetString();
                std::string receiver = _receiver.GetString();
                //rapidjson::Value& msg = it->GetMember("id");
                //std::string id = msg.GetString();
                LOG_INFO << "id -> " <<id;
                LOG_INFO << "content -> " <<content;
                LOG_INFO << "sender -> " <<sender;
                LOG_INFO << "receiver -> " <<receiver;
                MsgManager* MM = MsgManager::instance();
                assert(MM);
                // TODO: covert sender and receiver from string to int
                int sender_id = 1;
                int receiver_id = 2;
                MM->insert(content, sender_id, receiver_id);
            }
            
        }

    }

    
    //std::string temp = "SendMessage\r\n";
    //conn->send(temp);


}




Handle::Handle(Server* server)
: server_(server)
{

	server_->regCallBack("new_msg", boost::bind(&NewMessage::process, &newMessage_, _1, _2, _3));
	server_->regCallBack("send_msg", boost::bind(&SendMessage::process, &sendMessage_, _1, _2, _3));

}
 
