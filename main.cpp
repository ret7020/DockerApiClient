#include <curl/curl.h>
#include <iostream>
#include <cstring>
#include "docker.h"

// For tests
// #include <chrono>
// #include <thread>


using namespace std;


int main(){
    //curl -X GET --unix-socket /var/run/docker.sock http://localhost/images/json
    //cout << list_containers(true);
    // stop_container("7acca23a34e", 10);
    // restart_container("7acca23a34e", 10);

    /*run_container("7acca23a34e1");
    this_thread::sleep_for(std::chrono::milliseconds(5000));
    kill_container("7acca23a34e");*/
    return 0;
}
