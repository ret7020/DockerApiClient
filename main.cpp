#include <curl/curl.h>
#include <iostream>
#include <cstring>

using namespace std;

size_t writeFunction(void* ptr, size_t size, size_t nmemb, string* data) {
    data -> append((char*)ptr, size * nmemb);
    return size * nmemb;
}



int main(){
    //curl -X GET --unix-socket /var/run/docker.sock http://localhost/images/json
    string url = "https://google.com";
    curl_global_init(CURL_GLOBAL_DEFAULT);
    auto curl = curl_easy_init();
    if (curl) {
        // Convert url from string to *char
        int url_length = url.length();
        char url_arr[url_length + 1];
        strcpy(url_arr, url.c_str());
        curl_easy_setopt(curl, CURLOPT_UNIX_SOCKET_PATH, "/var/run/docker.sock");
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/images/json");
        string response_string;
        string header_string;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        curl = NULL;
        cout << response_string;
    }
    return 0;
}
