#include "chatmessage.h"

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
}
