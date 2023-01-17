#include <curl/curl.h>
#include <iostream>
#include <cstring>
#include "docker.h"

using namespace std;


int main(){
    //curl -X GET --unix-socket /var/run/docker.sock http://localhost/images/json
    cout << raw_request("http://localhost/images/json");
    return 0;
}
