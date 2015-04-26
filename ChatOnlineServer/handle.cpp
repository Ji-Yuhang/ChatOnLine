#include "abstract_handle.h"
#include "server.h"
Handle::Handle(Server* server)
: server_(server)
{
	server_->regCallBack("new_msg", boost::bind(&NewMessage::process, &newMessage_, _1, _2, _3));
	server_->regCallBack("send_msg", boost::bind(&SendMessage::process, &sendMessage_, _1, _2, _3));
}
 
