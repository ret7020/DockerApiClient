#g++ main.cpp docker.cpp -lcurl -o client
g++ -std=c++23 main.cpp docker.cpp -lcurl -lfmt -o client