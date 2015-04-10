#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include <string>
#include <boost/noncopyable.hpp>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>

class ChatMessage
{
public:
    ChatMessage(int id, const std::string& content, int sender, int receiver);
    ~ChatMessage();
    int id() const {return id_;}
    std::string content() const {return content_;}
    int senderID() const {return sender_;}
    int receiverID() const {return receiver_;}
private:
    int id_;
    std::string content_;
    int sender_;
    int receiver_;
};

class MsgManager : public boost::noncopyable
{
public:
    MsgManager();
    ~MsgManager();
    int maxMsgID() const {return max_id_;}
    void query(int index, int length);
    void insert(const std::string& content, int sender, int receiver);
    void save();
    void read();
    void clear();

private:
    static MsgManager* g_msgManager_;
    int max_id_;
    std::map<int, boost::shared_ptr<ChatMessage> > msgList_;
};
#endif
