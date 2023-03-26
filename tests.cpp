#include <iostream>
#include "json.hpp"
#include "docker.h"
#include <cstring>

using json = nlohmann::json;
int main(){
    json res = json::array();
    res.push_back({{"status", true}, {"stderr", "aa"}});
    string str = res[0]["stderr"];
    cout << str;
    return 0;
}