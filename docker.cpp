#include <curl/curl.h>
#include <iostream>
#include <fmt/core.h>
#include <cstring>
#include "json.hpp"
#include "docker.h"


using namespace std;
using json = nlohmann::json;

size_t writeFunction(void* ptr, size_t size, size_t nmemb, string* data) {
    // Simple callback for response of curl
    data -> append((char*)ptr, size * nmemb);
    return size * nmemb;
}
string convert_http_query(string str){

    string::size_type pos = 0;    
    while ((pos = str.find(' ', pos)) != string::npos)
    {
        str.replace(pos, 1, "%20");
        pos += 3;
    }

    return str;
}

string raw_request(string endpoint, int method, string data, string docker_socket){
    // Example curl cli command
    // cout << "RW " << endpoint << " " << method << " " << data << "\n";
    //curl -X GET --unix-socket /var/run/docker.sock http://localhost/images/json
    string url = convert_http_query(endpoint);
    cout << url << endl;
    string response_string;
    string header_string;
    long http_code = 0;

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
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);
        

        if (method == 1){
            cout << "POST\n";
            struct curl_slist *hs = NULL;
            hs = curl_slist_append(hs, "Content-Type: application/json");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hs);
            cout << data << "\n";
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        }
 
        curl_easy_perform(curl);
        auto res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        cout << "HTTP code: " << http_code << "\n";
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        curl = NULL;
    }
    return response_string;
}

json raw_api(string endpoint, int method, string data, string docker_socket) {
    string plain_text = raw_request(endpoint, method, data);
    json no_data = {
        {"data", false}
    };

    if (plain_text.length() == 0)
        return no_data;
    else
        return json::parse(plain_text);
}

json list_containers(bool all, string host="http://localhost/v1.41/") {
    return raw_api(fmt::v9::format("{}/containers/json?all={}", host, all));
}

json run_container(string id, string host="http://localhost/v1.41/")) {
    return raw_api(fmt::v9::format("http://localhost/v1.41/containers/{}/start", id), 1);
}

json stop_container(string id, int t, string host="http://localhost/v1.41/") {
    return raw_api(fmt::v9::format("http://localhost/v1.41/containers/{}/stop?t={}", id, t), 1);
}

json restart_container(string id, int t, string host="http://localhost/v1.41/") {
    return raw_api(fmt::v9::format("http://localhost/v1.41/containers/{}/restart?t={}", id, t), 1);
}

json kill_container(string id, string signal, string host="http://localhost/v1.41/") {
    return raw_api(fmt::v9::format("http://localhost/v1.41/containers/{}/kill?signal={}", id, signal), 1);
}

json exec_in_container(string id, string bash_command, bool AttachStdin, bool AttachStdout, bool AttachStderr, bool tty, string working_dir){
    string endpoint = fmt::v9::format("http://localhost/v1.41/containers/{}/exec", id);
    //string payload = fmt::v9::format("AttachStdin={}&AttachStdout={}&AttachStderr={}&tty={}&cmd=['{}']", AttachStdin, AttachStdout, AttachStderr, tty, bash_command);
    //cout << payload;
    json payload = {
        {"AttachStdin", AttachStdin},
        {"AttachStdout", AttachStdout},
        {"AttachStderr", AttachStderr},
        {"tty", tty},
        {"cmd", {bash_command}},
    };
    string payload_string = payload.dump();
    return raw_api(endpoint, 1, payload_string);
}
