#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "json.hpp"
#include <iostream>
using json = nlohmann::json;

int main(){
    int socket_desc, client_sock, c, read_size;
    struct sockaddr_in server, client;
    char client_message[8000];


    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) std::cout << "No Socket created - ERR";
    std::cout << "Socket created - OK\n";
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(1234);

    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0){
        std::cout << "Error\n";
        return 1;
    }
    std::cout << "Binding - OK\n";
    listen(socket_desc, 1);
    std::cout << "Waiting for clients\n";
    c = sizeof(struct sockaddr_in);
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0){
        std::cout << "Can't accept client\n";
        return 1;
    }
    std::cout << "Connection ok\n";
        while ((read_size = recv(client_sock, client_message, 2000, 0)) > 0) {
            std::cout << "Chunk";
            //write(client_sock, client_message, strlen(client_message));  // echo back message           
            //write(client_sock, ans, 6)
        }
        
        json tests = json::parse(client_message);
        std::cout << tests << "\n";
        std::string answer = "ANSWER";
        write(client_sock, answer.data(), answer.size());
        return 1;



        if (read_size == 0) {
            std::cout << "Client disconnected\n";
        }

    
    
    return 0;
}

