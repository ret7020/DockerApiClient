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