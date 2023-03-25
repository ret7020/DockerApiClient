#include <curl/curl.h>
#include <iostream>
#include <fmt/core.h>
#include <cstring>
#include "json.hpp"
#include "docker.h"


using namespace std;
using json = nlohmann::json;

size_t writeFunction(void *ptr, size_t size, size_t nmemb, string *data)
{
    /* Simple callback for response of curl */
    data->append((char *)ptr, size * nmemb);
    return size * nmemb;
}
string convert_http_query(string str)
{
    // Process http query to use it with curl
    string::size_type pos = 0;
    while ((pos = str.find(' ', pos)) != string::npos)
    {
        str.replace(pos, 1, "%20");
        pos += 3;
    }

    return str;
}

string raw_request(string endpoint, int method, string data, string docker_socket)
{
    /* Raw request to docker endpoint
        Choose docker socket
        Choose http host
        Choose http request type (GET, POST)
    */

    // Example curl cli command
    // curl -X GET --unix-socket /var/run/docker.sock http://localhost/images/json
    // cout << "RW " << endpoint << " " << method << " " << data << "\n";
    string url = convert_http_query(endpoint);
    // cout << url << endl;
    string response_string;
    string header_string;
    long http_code = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    auto curl = curl_easy_init();
    if (curl)
    {
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

        if (method == 1) // POST
        {
            // cout << "POST\n";
            struct curl_slist *hs = NULL;
            hs = curl_slist_append(hs, "Content-Type: application/json");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hs);
            // cout << data << "\n";
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        }else if (method == 2) // PUT
        {
            
        }

        curl_easy_perform(curl);
        auto res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        // cout << "HTTP code: " << http_code << "\n";
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        curl = NULL;
    }
    return response_string;
}

json raw_api(string endpoint, int method, string data, string docker_socket)
{
    string plain_text = raw_request(endpoint, method, data);
    //cout << "\n" << plain_text << "\n";
    json no_data = {
        {"data", false}};

    if (plain_text.length() == 0)
        return no_data;
    else
        return json::parse(plain_text);
}

json list_containers(bool all, string host)
{
    return raw_api(fmt::v9::format("{}/containers/json?all={}", host, all));
}

json inspect_container(string id, string host)
{
    return raw_api(fmt::v9::format("{}/containers/{}/json", host, id));
}
json processes_in_container(string id, string ps_args, string host){
    return raw_api(fmt::v9::format("{}/containers/{}/top?ps_args={}", host, id, ps_args));
}

json start_container(string id, string host)
{
    return raw_api(fmt::v9::format("{}/containers/{}/start", host, id), 1);
}

json stop_container(string id, int t, string host)
{
    return raw_api(fmt::v9::format("{}/containers/{}/stop?t={}", host, id, t), 1);
}

json restart_container(string id, int t, string host)
{
    return raw_api(fmt::v9::format("{}/containers/{}/restart?t={}", host, id, t), 1);
}

json kill_container(string id, string signal, string host)
{
    return raw_api(fmt::v9::format("{}/containers/{}/kill?signal={}", host, id, signal), 1);
}

json exec_in_container(string id, string bash_command, bool bash, bool AttachStdin, bool AttachStdout, bool AttachStderr, bool tty, string working_dir, string host)
{
    // Creating exec instance
    string endpoint = fmt::v9::format("{}/containers/{}/exec", host, id);
    
    json payload = {
        {"AttachStdin", AttachStdin},
        {"AttachStdout", AttachStdout},
        {"AttachStderr", AttachStderr},
        {"Tty", tty},
        {"WorkingDir", working_dir}
    };
    // To execute bash command in docker, you need to use smth, like bash -c COMMAND HERE
    // To execute binary in docker, you need to use smth, like /path/to/bin
    if (bash) payload["Cmd"] = {"bash", "-c", bash_command};
    else payload["Cmd"] = {bash_command};
    string payload_string = payload.dump();
    cout << payload_string << "\n";
    json res = raw_api(endpoint, 1, payload_string);
    // Start exec instance
    payload = {
        {"Detach", true},
        {"Tty", true}};
    payload_string = payload.dump();
    return raw_api(fmt::v9::format("{}/exec/{}/start", host, res["Id"]), 1, payload_string);
}

json create_container(string image, int StopTimeout, int MemoryLimit, string bash_init_cmd, string WorkingDir, bool AttachStdin, bool AttachStdout, bool AttachStderr, bool NetworkDisabled, string host){
    json payload = {
        {"Image", image},
        // {"Entrypoint", {"/bin/bash", },
        {"Cmd", {"echo", "Hello, world!"}},
        //{"StopTimeout", StopTimeout},
        {"HostConfig", {{"Memory", MemoryLimit}}},
        {"NetworkDisabled", NetworkDisabled},
        // {"WorkingDir", WorkingDir},
        // {"AttachStdin", AttachStdin},
        // {"AttachStdout", AttachStdout},
        // {"AttachStderr", AttachStderr}
    };
    string payload_string = payload.dump();
    cout << "\n" << payload_string << "\n";
    return raw_api(fmt::v9::format("{}/containers/create", host), 1, payload_string);
}

json put_archive(string id, string host){

}

/*class API {
    public:
        string Host, UnixSocket;
        API(string Host="http://localhost/v1.41", string UnixSocket="/var/run/docker.sock"){
            cout << "Class constructor";
        }
};*/
