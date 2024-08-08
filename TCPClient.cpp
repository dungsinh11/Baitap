#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

#define MAX 80
#define PORT 8080

void func(SOCKET sockfd) {
    char buff[MAX];
    int n;
    for (;;) {
        std::memset(buff, 0, sizeof(buff));
        std::cout << "Enter the string: ";
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        send(sockfd, buff, sizeof(buff), 0);  
        std::memset(buff, 0, sizeof(buff));
        recv(sockfd, buff, sizeof(buff), 0);
        std::cout << "From Server: " << buff;
        if (strncmp(buff, "exit", 4) == 0) {
            std::cout << "Client Exit...\n";
            break;
        }
    }
}

int main() {
    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    SOCKET sockfd;
    struct sockaddr_in servaddr;

    // Socket creation and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        std::cout << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    std::cout << "Socket successfully created..\n";

    std::memset(&servaddr, 0, sizeof(servaddr));

    // Assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);

    // Convert IP address
    if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0) {
        std::cout << "Invalid address/ Address not supported\n";
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    // Connect the client socket to server socket
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        std::cout << "Connection with the server failed: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    std::cout << "Connected to the server..\n";

    // Function for chat
    func(sockfd);

    // Close the socket
    closesocket(sockfd);
    WSACleanup();

    return 0;
}
