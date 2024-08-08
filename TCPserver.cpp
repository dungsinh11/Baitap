#include <iostream>
#include <cstring>
#include <cstdlib>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

// Function designed for chat between client and server.
void func(SOCKET connfd)
{
    char buff[MAX];
    int n;
    // Infinite loop for chat
    for (;;)
    {
        memset(buff, 0, MAX);

        // Read the message from client and copy it in buffer
        int bytesRead = recv(connfd, buff, sizeof(buff), 0);
        if (bytesRead <= 0)
        {
            std::cerr << "Connection closed or error in reading" << std::endl;
            break;
        }
        // Print buffer which contains the client contents
        std::cout << "From client: " << buff << "\t To client: ";
        memset(buff, 0, MAX);
        n = 0;
        // Copy server message in the buffer
        while ((buff[n++] = getchar()) != '\n')
            ;

        // And send that buffer to client
        send(connfd, buff, sizeof(buff), 0);

        // If msg contains "exit" then server exit and chat ended.
        if (strncmp("exit", buff, 4) == 0)
        {
            std::cout << "Server Exit..." << std::endl;
            break;
        }
    }
}

// Driver function
int main()
{
    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    SOCKET sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // Socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    else
    {
        std::cout << "Socket successfully created.." << std::endl;
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if (bind(sockfd, (SA*)&servaddr, sizeof(servaddr)) == SOCKET_ERROR)
    {
        std::cerr << "Socket bind failed: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    else
    {
        std::cout << "Socket successfully binded.." << std::endl;
    }

    // Now server is ready to listen and verification
    if (listen(sockfd, 5) == SOCKET_ERROR)
    {
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    else
    {
        std::cout << "Server listening.." << std::endl;
    }

    int len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd == INVALID_SOCKET)
    {
        std::cerr << "Server accept failed: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    else
    {
        std::cout << "Server accepted the client..." << std::endl;
    }

    // Function for chatting between client and server
    func(connfd);

    // After chatting close the socket
    closesocket(sockfd);
    WSACleanup();

    return 0;
}
