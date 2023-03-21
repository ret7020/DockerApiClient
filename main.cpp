#include <curl/curl.h>
#include <iostream>
#include <cstring>
#include "docker.h"

using namespace std;

// Single task checking pipeline (beta)
// Compile c++ program
// Start Docker Container
// Copy bin or python source to container
// Start timer (get current unix time)
// Start binary/script and pass stdin inside process
// Wait until process finished or timer get over than max excution time
// Get stdout and compare it with test
// Repeat this for each test
// After all tests finished, kill container

int main()
{
    string container = "11aa6f74b938";
    // cout << inspect_container(container)["State"];
    cout << processes_in_container(container, "aux");
    // Example curl to docker api unix socket
    // curl -X GET --unix-socket /var/run/docker.sock http://localhost/images/json
    //
    // Methods from lib
    // cout << list_containers(true);

    // stop_container("7acca23a34e", 10);
    // restart_container("7acca23a34e", 10);

    // start_container(container);
    //  this_thread::sleep_for(std::chrono::milliseconds(5000));

    //cout << exec_in_container(container, "echo 'Hello, world!' > /proc/1/fd/1");

    // kill_container(container);

    // API DockerApiClient("http://localhost/v1.41", "/var/run/docker.sock");
    return 0;
}