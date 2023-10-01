#ifndef __CAST_CONFIG_H__
#define __CAST_CONFIG_H__

#include <string>
#include <json/json.h>

using namespace std;

class cast_config
{
    public:
	cast_config(const string path);
	~cast_config();
	string get_server();
	int    get_port();
    private:
	cast_config() {};
	bool init();

    private:
	string m_path;
	string m_server;
	int m_port;
};

#endif
