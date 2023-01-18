#include <curl/curl.h>
#include <iostream>
#include <cstring>
#include "docker.h"

using namespace std;


int main(){
    //curl -X GET --unix-socket /var/run/docker.sock http://localhost/images/json
    cout << list_containers();
    return 0;
}
