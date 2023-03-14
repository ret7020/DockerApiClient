#pragma once

#include <curl/curl.h>
#include <iostream>
#include <cstring>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

size_t writeFunction(void* ptr, size_t size, size_t nmemb, string* data);
string raw_request(string endpoint, int method=0, string data="", string docker_socket="/var/run/docker.sock");
json raw_api(string endpoint, int method=0, string data="", string docker_socket="/var/run/docker.sock");
json list_containers(bool all=false);
json run_container(string id);
json stop_container(string id, int t=0);
json restart_container(string id, int t=0);
json kill_container(string id, string signal="SIGKILL");
json attach_container(string id);
json exec_in_container(string id, string bash_command, bool AttachStdin=true, bool AttachStdout=true, bool AttachStderr=true, bool tty=false, string working_dir="/");

// Api class
class API {
    public: 
        string Host, UnixSocket;
        API(string Host="http://localhost/v1.41", string UnixSocket="/var/run/docker.sock");
};
