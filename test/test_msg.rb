require 'socket'
require 'awesome_print'
socket = TCPSocket.new('localhost',2020)
json = %Q{{"type":"new_msg"}\r\n}
socket.write(json)
r = socket.readline
ap r
socket.close()
