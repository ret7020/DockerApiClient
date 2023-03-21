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

json list_containers(bool all=false, string host="http://localhost/v1.41");
json start_container(string id, string host="http://localhost/v1.41");
json stop_container(string id, int t=0, string host="http://localhost/v1.41");
json restart_container(string id, int t=0, string host="http://localhost/v1.41");
json kill_container(string id, string signal="SIGKILL", string host="http://localhost/v1.41");
json exec_in_container(string id, string bash_command, bool AttachStdin=true, bool AttachStdout=true, bool AttachStderr=true, bool tty=false, string working_dir="/", string host="http://localhost/v1.41");

//json attach_container(string id);
// Api class
// class API {
//     public: 
//         string Host, UnixSocket;
//         API(string Host="http://localhost/v1.41", string UnixSocket="/var/run/docker.sock");
// };
