#include <curl/curl.h>
#include <iostream>
#include <cstring>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

size_t writeFunction(void* ptr, size_t size, size_t nmemb, string* data);
string raw_request(string endpoint, string docker_socket="/var/run/docker.sock");
json raw_api(string endpoint, string docker_socket="/var/run/docker.sock");
json list_containers(bool all=true);