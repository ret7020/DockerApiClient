#include <curl/curl.h>
#include <iostream>
#include <cstring>
#include "docker.h"


using namespace std;


int main(){
    // Example curl to docker api unix socket
    //curl -X GET --unix-socket /var/run/docker.sock http://localhost/images/json
    //
    // Methods from libs
    //cout << list_containers(true);
    
    // stop_container("7acca23a34e", 10);
    // restart_container("7acca23a34e", 10);

    // run_container("7acca23a34e1");
    // this_thread::sleep_for(std::chrono::milliseconds(5000));
    // kill_container("7acca23a34e");

    // run_container("7acca23a34e");
    // cout << exec_in_container("7acca23a34e", "ping ya.ru");
    API DockerApiClient("http://localhost/v1.41", "/var/run/docker.sock");
    return 0;
}
