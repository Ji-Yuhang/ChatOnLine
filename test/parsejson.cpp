#include <iostream>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/reader.h"
#include "rapidjson/error/en.h"

int main()
{

    std::string json = "{\"type\":\"new_message\",\"data\":[{\"id\":\"123\",\"content\":\"中国\",\"sender\":\"liangjie\",\"receiver\":\"group202\"}]}\r\n";
    rapidjson::Document doc;
    doc.Parse(json.c_str());
    if (doc.HasParseError()) {
        std::cout << "CANNOT PARSE JSON:"<<json<<std::endl;
        std::cout <<"GetParseError:"<<doc.GetParseError()<<" "<< rapidjson::GetParseError_En(doc.GetParseError()) <<" offset:"<<doc.GetErrorOffset()<<std::endl;  
        exit(0);
    }
    std::cout << "parse truth:"<<json<<std::endl;
    if (doc.IsObject() && doc.HasMember("type")) {
        std::cout << "have type"<<std::endl;
    }
    return 0;
}

