#include <curl/curl.h>
#include <iostream>
#include <cstring>
#include "docker.h"
#include <fmt/core.h>

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
void init_workspace(string submission_id)
{
    // Create folder in ./workspace with name = submission_id
    string bash = "mkdir ./workspace/" + submission_id + " 2> /dev/null";
    system(bash.c_str());
}

void cleanup_workspace(string submission_id)
{
    string bash = "rm -r ./workspace/" + submission_id + " 2> /dev/null";
    system(bash.c_str());
}

// Utility test function
// In fact we will get source code files via socket or from backend directly uploaded to ./workspace dir
void copy_demo_submission_source(string submission_id)
{
    string bash = "cp -r ./demo_source/* ./workspace/" + submission_id + " 2> /dev/null";
    system(bash.c_str());
}

std::string exec(const char *cmd)
{
    char buffer[128];
    std::string result = "";
    FILE *pipe = popen(cmd, "r");
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    try
    {
        while (fgets(buffer, sizeof buffer, pipe) != NULL)
        {
            result += buffer;
        }
    }
    catch (...)
    {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

// Ok, I loose, shit docker engine api
string shtp_python_cli_run(string submission_id, string test="1 2")
{
    string workspace_absolute_path = "/home/stephan/Progs/DockerAPI/build/workspace";
    string mount_to = "/home/code";
    string container = "python:latest";
    string entrypoint = "bash -c \"python3 /home/code/main.py <<< '1 2'\"";
    string bash = fmt::v9::format("docker run --network none -itd -v {}/{}:{} {} {}", workspace_absolute_path, submission_id, mount_to, container, entrypoint);
    // Example command
    // docker run --network none -itd -v /home/stephan/Progs/DockerAPI/build/workspace/tmp:/home/code python:latest bash
    string container_id_messy = exec(bash.c_str());
    return container_id_messy.erase(container_id_messy.size() - 1);
}


// string shtp_python_exec(string)

int main()
{
    // Our Pipeline
    string image = "python:latest";
    const string demo_submission = "666"; // Get it from backend
    
    // ONLY FOR TEST WITHOUT BACKEND; WORKSPACE INITIALIZATION IS BACKEND DUTY
    cleanup_workspace(demo_submission); // To be sure, there is no such workspace folder
    init_workspace(demo_submission); // Create folder
    copy_demo_submission_source(demo_submission); // For test; gain source code into submission workspace
    // ONLY FOR TEST WITHOUT BACKEND; WORKSPACE INITIALIZATION IS BACKEND DUTY

    string submission_container = shtp_python_cli_run(demo_submission); // Run docker container with python3 process 
    cout << submission_container << "\n"; // Print container id for debug
    int status_code = wait_for_container(submission_container)["StatusCode"]; // Wait until container finish
    if (!status_code){
        cout << get_container_logs(submission_container); // Get stdout of container to compare with tests
    }else {} // Stderr handler here
    cleanup_workspace(demo_submission); // Delete current submission folder from workspace
    remove_container(submission_container); // Delete container to prevent spam of containers
    // Pipeline finish

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

    // start_container(container);
    //   this_thread::sleep_for(std::chrono::milliseconds(5000));

    // cout << exec_in_container(container, "/home/a", false);

    // kill_container(container);

    // API DockerApiClient("http://localhost/v1.41", "/var/run/docker.sock");

    // Our checker pipeline (refer to github.com/ItClassDev/Checker)
    // string container = "4c45d07e9038";
    //string process_exit_sequence = "SHTP_PROCESS_EXIT_SEQUENCE";
    
    // websocket::stream<tcp::socket> ws = attach_to_container_ws(submission_container);                                 // Connect to websocket of started container
    // ws.write(net::buffer(fmt::v9::format("python3 /home/code/main.py < \n\r"))); // Run python source code execution
    // ws.write(net::buffer(string("1 2\r")));// Send test to stdin
    // int exit_sequences_found = 0;
    // string result = "";
    // try
    // {
    //     while (exit_sequences_found == 0)
    //     { // TODO

    //         if (exit_sequences_found == 0){
    //             beast::flat_buffer buffer; // buffer
    //             ws.read(buffer);           // read data from socket to buffer

    //             string buffer_processed = beast::buffers_to_string(buffer.data());
    //             result += buffer_processed;
    //             cout << result << "---" << "\n";
            
    //             // //buffer_processed = buffer_processed.erase(buffer_processed.size() - 1);
    //             // //cout << buffer_processed; // print debug
    //             // // Finish execution trigger
    //             // cout << buffer_processed;
    //             if (count_substr(result.c_str(), process_exit_sequence.c_str()) >= 2){
    //                 cout << "EXIT";
    //                 exit_sequences_found = 1;
    //                 //ws.close(websocket::close_code::normal);
    //             }
    //         }
            
            
    //         // if (count_substr(result.c_str(), process_exit_sequence.c_str()) >= 2){
    //         //     exit_sequences_found = 2;
    //         //     cout << "EXIT" << " " << exit_sequences_found << " ";
    //         //     //break;
    //         // }
    //         // if (strstr(buffer_processed.c_str(), process_exit_sequence.c_str())) break;
    //         // if (buffer_processed.find(process_exit_sequence) != string::npos) break; // Execution finished
    //     }
    //     //
    // }
    // catch (std::exception const &e)
    // {
    //     cout << result;
    // }
    
    // cout << "PROCESS FINISHED";
    //kill_container(submission_container);

    return 0;
    // auto p = Popen({"docker", "attach", submission_container}, output{PIPE}, input{PIPE});
    // auto msg = "python3 /home/code/main.py\r";
    // p.send(msg, strlen(msg));
    // auto res = p.communicate();
    // std::cout << res.first.buf.data() << std::endl;
}