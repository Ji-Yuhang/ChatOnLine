require 'socket'
require 'awesome_print'
require 'json'
socket = TCPSocket.new('localhost',2020)
json = %Q{{"type":"new_msg","data":[{"id":"101","content":"english","sender":"liangjie","receiver":"group202"}]}\r\n}
json1 = %Q{{"type":"new_msg","data":[{"id":"101","content":"晚上谁开黑","sender":"liangjie","receiver":"group202"}]}\r\n}
# parse core dump
json2 = %Q{{"type":"new_msg","data":[{"id":"101","content":"good"}]}\r\n}
# parse ERROR invalid value
json3 = %Q{{"type":"new_msg","data":[{"id":"101"}]}\r\n}
# parse when individual pass, flowering json2 error
json4 = %Q{{"type":"new_msg","data":[]}\r\n}
# ok
json5 = %Q{{"type":"new_msg","data":"中国"}\r\n}
=begin
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
=end
newMessage = {}
newMessage["type"] = "send_msg"
data = []
one = {}
one["id"] = "101"
one["content"] = "content"
one["sender"] = "liangjie"
one["receiver"] = "group202"
data.push one
newMessage["data"] = data;
ap newMessage

aJson = newMessage.to_json
ap aJson
socket.write(aJson+"\r\n")
loop do
    r = socket.readline
    ap r
end
socket.close()
