#include <curl/curl.h>
#include <iostream>
#include <cstring>
#include "docker.h"

using namespace std;


int main(){
    //curl -X GET --unix-socket /var/run/docker.sock http://localhost/images/json
    json data = raw_api("http://localhost/images/json");
    cout << data[0];
    return 0;
}
