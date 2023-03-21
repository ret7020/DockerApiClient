#include <curl/curl.h>
#include <iostream>
#include <cstring>
#include "docker.h"


using namespace std;


int main(){
    string container = "11aa6f74b938";
    // Example curl to docker api unix socket
    //curl -X GET --unix-socket /var/run/docker.sock http://localhost/images/json
    //
    // Methods from lib
    //cout << list_containers(true);
    
    // stop_container("7acca23a34e", 10);
    // restart_container("7acca23a34e", 10);

    //start_container(container);
    // this_thread::sleep_for(std::chrono::milliseconds(5000));

    cout << exec_in_container(container, "echo 'Hello' > /home/res.txt");

    //kill_container(container);

    //API DockerApiClient("http://localhost/v1.41", "/var/run/docker.sock");
    return 0;
}

