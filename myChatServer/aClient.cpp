#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //inet_ntoa
#include <unistd.h>    //close()


int main()
{
    int bufsize = 1024;
    char buffer[1024];
    // CREATE
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
        std::cout << "Error in creating a client socket";
        exit(-1);
    }

    // struct sockaddr_in to place the data of the IPv4
    struct sockaddr_in clientAddress;
    clientAddress.sin_addr.s_addr = INADDR_ANY;
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_port = htons(10000);

    // CONNECTION
    connect(client_socket, (struct sockaddr *)&clientAddress, sizeof(clientAddress));

    std::cout << "Awaiting confirmation from the server" << std::endl;

    // recv(client_socket, buffer, bufsize, 0);
    // std::cout << "The message has been confirmed" << std::endl;
    // std::cout << "Buffer: " << buffer << std::endl;

    recv(client_socket, buffer, bufsize, 0);
    std::cout << "CONNECTION UPDATE: " << buffer << std::endl;
    bool isExit = true;
    while (isExit)
    {
        std::cout << "What is your message ?";
        std::cin.getline(buffer, 1024);

        send(client_socket, buffer, sizeof(buffer), 0);
        if (buffer[0] == '#')
            isExit = false;

        if ((recv(client_socket, buffer, sizeof(buffer), 0) > 0))
        {
            std::cout << "Server: " << buffer << std::endl;
            if (buffer[0] == '#')
                isExit = false;
        }
    }

    std::cout << "Connection terminated..." << std::endl;
    close(client_socket);

    return 0;
}