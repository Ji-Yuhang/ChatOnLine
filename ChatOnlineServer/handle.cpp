#include "abstract_handle.h"
#include "server.h"
void NewMessage::process(const muduo::net::TcpConnectionPtr& conn, DataType& data,muduo::Timestamp time)
{

    LOG_INFO << "NewMessage::process ";
    //std::cout << "This NewMessage Process function";
    std::string temp = "NewMessage\r\n";
    conn->send(temp);

}
void SendMessage::process(const muduo::net::TcpConnectionPtr& conn, DataType& data,muduo::Timestamp time)
{

    LOG_INFO << "SendMessage::process ";
    //std::cout << "This SendMessage Process function"<<std::endl;

    rapidjson::Value& block = data["data"];
    //if (block.IsObject()) {
    if (block.IsArray()) {
        for (rapidjson::Value::ValueIterator it = block.Begin(); it != block.End(); ++it) {
           // TODO: parse sendmessage 
            //rapidjson::Value& msg = it->GetMember("id");
            //std::string id = msg.GetString();
            //LOG_INFO << "id -> " <<id;
            
        }

    }

    
    std::string temp = "SendMessage\r\n";
    conn->send(temp);


}




Handle::Handle(Server* server)
: server_(server)
{

	server_->regCallBack("new_msg", boost::bind(&NewMessage::process, &newMessage_, _1, _2, _3));
	server_->regCallBack("send_msg", boost::bind(&SendMessage::process, &sendMessage_, _1, _2, _3));

}
 
