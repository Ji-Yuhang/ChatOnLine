require 'socket'
require 'awesome_print'
require 'json'
#socket = TCPSocket.new('23.88.3.15',2020)
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
=begin
Thread.new {
    # Thread #2 runs this code
    loop do
        r = socket.readline
        #ap "receive string > " + r
        temp = JSON.parse r

        #ap "receive json > "
        #ap temp
        rmsg = temp["data"][0]["content"]
        ap "receive > " +rmsg
    end


}
=end
content = " I am content"
newMessage = {}
newMessage["type"] = "send_msg"
data = []
one = {}
one["id"] = "101"
one["content"] = content
one["sender"] = "liangjie"
one["receiver"] = "group202"
data.push one
newMessage["data"] = data;
ap "tojson > " 
ap newMessage
aJson = newMessage.to_json

socketList = []
num = 0
max = ARGV[0].to_i
loop do
    ap num
    break if num> max
    
    socket = TCPSocket.new('127.0.0.1',2007)
    socketList.push socket
    socket.write(aJson+"\r\n")
    num +=1
end
socketList.each do |conn|
    conn.close
end

