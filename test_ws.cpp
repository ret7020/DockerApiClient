#include <iostream>

#include "subprocess.hpp"
using namespace subprocess;

int main(){
    //auto p = Popen({"python3", "/home/stephan/Progs/DockerAPI/build/workspace/tmp/main.py"}, input{PIPE});
    //std::cout << "Data : " << obuf.buf.data() << std::endl;
    auto p = Popen({"python3", "/home/stephan/Progs/DockerAPI/build/workspace/tmp/main.py"}, output{PIPE}, input{PIPE});
    auto msg = "1 2\r";
    p.send(msg, strlen(msg));
    auto res = p.communicate();
    std::cout << res.first.buf.data() << std::endl;
    return 0;
}