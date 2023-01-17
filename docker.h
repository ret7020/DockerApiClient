#include <curl/curl.h>
#include <iostream>
#include <cstring>


using namespace std;

size_t writeFunction(void* ptr, size_t size, size_t nmemb, string* data);
string raw_request(string endpoint, string docker_socket="/var/run/docker.sock");