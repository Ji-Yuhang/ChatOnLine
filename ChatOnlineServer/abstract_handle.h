#ifndef ABSTRACT_HANDLE_H
#define ABSTRACT_HANDLE_H
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/reader.h"
#include "rapidjson/error/en.h"
#include <iostream>
#include <boost/bind.hpp>
#include "muduo/net/TcpServer.h"
#include "muduo/net/EventLoop.h"
#include "muduo/base/Logging.h"


using namespace std;
using namespace rapidjson;

class Server;

class AbstractHandle
{
public:
    AbstractHandle(){}
    ~AbstractHandle(){}

};

template<class T>
class TcpFormat
{
public:
    typedef T DataType;
};

class JsonFormat : public TcpFormat<rapidjson::Document>
{
public:
    JsonFormat(){}
    ~JsonFormat(){}
};

class XmlFormat : public TcpFormat<rapidjson::Document>
{
public:
    XmlFormat(){}
    ~XmlFormat(){}
};

#endif
