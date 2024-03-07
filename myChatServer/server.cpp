#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //inet_ntoa
#include <unistd.h>    //close()
#include <thread>

void handleRecv(int socket_server);
void handleSend(int socket_server);

int main()
{
    bool isExit = false;
    char buffer[1024];

    // CREATE
    int server_socket = socket(AF_INET, SOCK_STREAM, 0); // this is created to establish server side contact. It will be used for listening
    if (server_socket == -1)
    {
        perror("Socket: ");
        exit(-1);
    }

    // defining the struct sockaddr_in for holding address information for IPv4 internet domain sockets
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(10000);

    // BIND
    int theBind = bind(server_socket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (theBind < 0)
    {
        perror("Binding: ");
        exit(-1);
    }
    std::cout << "The binding is complete" << std::endl;
    int size = sizeof(serverAddress);

    // LISTENING
    listen(server_socket, 5);

    // ACCEPTING CLIENTS
    int client_fd = accept(server_socket, (struct sockaddr *)&serverAddress, (socklen_t *)&size);
    if (client_fd < 0)
        std::cout << "Error accepting the request";

    std::cout << "Server: Connection established"
              << "\n"
              << std::endl;
    strcpy(buffer, "You are now connect to the server.");
    send(client_fd, buffer, sizeof(buffer), 0);

    while (true)
    {
        recv(client_fd, buffer, sizeof(buffer), 0);
        std::cout << "Client: " << buffer << std::endl;
        if (buffer[0] == '#')
        {
            std::cout << "Connection has been terminated" << '\n'
                      << std::endl;
            break;
        }

        std::cout << "Server: ";
        std::cin.getline(buffer, 1024);
        send(client_fd, buffer, sizeof(buffer), 0);
    }

    // inet_ntoa converts the serverAddress.sin_addr from bytes to a readable string form.
    std::cout << "Closing connection with: " << inet_ntoa(serverAddress.sin_addr) << std ::endl;
    std::cout << "This connection is now closed";
    close(client_fd);

    return 0;
}

void handleRecv(int server_socket)
{
    bool isExit = false;
    char tempStorage[1024];

    while (!isExit)
    {
        int bytes_recv = recv(server_socket, tempStorage, sizeof(tempStorage), 0);

        if (bytes_recv < 0)
        {
            perror("Error with receiving a message (handleRecv --server.cpp): ");
            return;
        }

        if (strcmp(tempStorage, "#exit") == 0)
        {
            std::cout << "Connection terminated" << std::endl;
            close(server_socket);
            isExit = true;
        }
        std::cout << "Client: " << tempStorage << std::endl;
    }
    return;
}

void handleSend(int server_socket)
{
    bool isExit = false;
    char tempStorage[1024];
    while (!isExit)
    {
        std::cout << "You: ";
        std::cin.getline(tempStorage, 1024);
        send(server_socket, tempStorage, sizeof(tempStorage), 0);
        close(server_socket);
        isExit = true;
    }
    return;
}