#include <iostream>
#include "json.hpp"
#include "docker.h"
#include <cstring>

using json = nlohmann::json;
int main(){
    json res = json::array();
    res.push_back({{"status", true}, {"stderr", "aa"}});
    cout << res;
    return 0;
}