#include <curl/curl.h>
#include <iostream>
#include <cstring>
#include "docker.h"


using namespace std;

size_t writeFunction(void* ptr, size_t size, size_t nmemb, string* data) {
    // Simple callback for response of curl
    data -> append((char*)ptr, size * nmemb);
    return size * nmemb;
}

string raw_request(string endpoint, string docker_socket){
    //curl -X GET --unix-socket /var/run/docker.sock http://localhost/images/json
    string url = endpoint;
    string response_string;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    auto curl = curl_easy_init();
    if (curl) {
        // Convert socket path + endpoint from string to *char
        int endpoint_length = endpoint.length();
        char endpoint_arr[endpoint_length + 1];
        strcpy(endpoint_arr, endpoint.c_str());
        int socket_length = docker_socket.length();
        char socket_arr[socket_length + 1];
        strcpy(socket_arr, docker_socket.c_str());
        
        curl_easy_setopt(curl, CURLOPT_UNIX_SOCKET_PATH, socket_arr);
        curl_easy_setopt(curl, CURLOPT_URL, endpoint_arr);
        string header_string;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        curl = NULL;
    }
    return response_string;
}