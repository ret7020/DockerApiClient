#include <curl/curl.h>
#include <iostream>
#include <cstring>
#include "docker.h"

using namespace std;


int main(){
    //curl -X GET --unix-socket /var/run/docker.sock http://localhost/images/json
    //cout << list_containers(true);
    cout << run_container("daceab19628f29988ee2cb85325560a92207988750e0a05bcc7545996f62747b");
    return 0;
}
