#include <curl/curl.h>
#include <iostream>
#include <cstring>
#include "docker.h"
#include <fmt/core.h>
#include "subprocess.hpp"

using namespace subprocess;
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

// Function for ShTP Checker
// TODO Move to ITClassDev/Checker
void init_workspace(string submission_id){
    // Create folder in ./workspace with name = submission_id
    string bash = "mkdir ./workspace/" + submission_id + " 2> /dev/null";
    system(bash.c_str());
}

void cleanup_workspace(string submission_id){
    string bash = "rm -r ./workspace/" + submission_id + " 2> /dev/null";
    system(bash.c_str());
}

// Utility test function
// In fact we will get source code files via socket or from backend directly uploaded to ./workspace dir
void copy_demo_submission_source(string submission_id){
    string bash = "cp -r ./demo_source/* ./workspace/" + submission_id + " 2> /dev/null";
    system(bash.c_str());
}

std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}


// Ok, I loose, shit docker engine api
string shtp_python_cli_run(string submission_id){
    string workspace_absolute_path = "/home/stephan/Progs/DockerAPI/build/workspace";
    string mount_to = "/home/code";
    string container = "python:latest";
    string entrypoint = "bash";
    string bash = fmt::v9::format("docker run --network none -itd -v {}/{}:{} {} {}", workspace_absolute_path, submission_id, mount_to, container, entrypoint);
    // Example command
    //docker run --network none -itd -v /home/stephan/Progs/DockerAPI/build/workspace/tmp:/home/code python:latest bash
    return exec(bash.c_str());   
}

// string shtp_python_exec(string)

int main()
{
    string container = "4c45d07e9038";
    string image = "python:latest";

    // API test
    // cout << inspect_container(container)["State"];
    // cout << processes_in_container(container, "aux");
    // Example curl to docker api unix socket
    // curl -X GET --unix-socket /var/run/docker.sock http://localhost/images/json
    //
    // Methods from lib
    // cout << list_containers(true);

    // stop_container("7acca23a34e", 10);
    // restart_container("7acca23a34e", 10);

    //start_container(container);
    //  this_thread::sleep_for(std::chrono::milliseconds(5000));

    //cout << exec_in_container(container, "/home/a", false); 

    // kill_container(container);

    // API DockerApiClient("http://localhost/v1.41", "/var/run/docker.sock");

    // Our checker pipeline (refer to github.com/ItClassDev/Checker)
    const string demo_submission = "666";

    cleanup_workspace(demo_submission);
    init_workspace(demo_submission);
    // For test; gain source code into submission workspace
    //copy_demo_submission_source(demo_submission);
    // Run docker container
    //string submission_container = shtp_python_cli_run(demo_submission);
    string submission_container = "29ecd4fb47c21688ef5fa96ab2e825602d3a7c5b32e09b7961752a82389fa8c7";
    auto p = Popen({"docker", "attach", submission_container}, output{PIPE}, input{PIPE});
    auto msg = "python3 /home/code/main.py\r";
    p.send(msg, strlen(msg));
    auto res = p.communicate();
    std::cout << res.first.buf.data() << std::endl;
    
    // kill_container(submission_container);
    // cleanup_workspace(demo_submission);

    // Pipeline finish
    return 0;
}