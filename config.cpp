
#include "config.h"

#include <fstream>
#include <iostream>


cast_config::cast_config(const string path)
    :m_path(path)
     , m_server("")
     , m_port(8003)
{
    init();
}

cast_config::~cast_config()
{
}

bool cast_config::init()
{
    Json::Value root;
    std::ifstream ifs;
    ifs.open(m_path);

    //cout << "m_path = " << m_path << endl;

    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;
    JSONCPP_STRING errs;
    if (!parseFromStream(builder, ifs, &root, &errs)) {
	std::cout << errs << std::endl;
	return false;
    }
    //std::cout << root << std::endl;

    m_server = root["server"].asString();
    m_port = root["port"].asInt();

    ifs.close();

    return true;
}


string cast_config::get_server()
{
    return m_server;
}

int cast_config::get_port()
{
    return m_port;
}
