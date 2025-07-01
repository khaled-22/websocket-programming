/*
#include <iostream>
#include <winsock.h>
#include <string>
#include <cstring> // for strln

#pragma comment(lib, "ws2_32.lib") // Link to the WinSock library DLL

int main() {

    // 1. Step initialize socket
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return 1;
    }
    // Create the client socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket. Error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // 3. bind automatically

    // Read from terminal for server address
    std::string serverIP;
    std::cout << "Enter IP: ";
    std::cin >> serverIP;

    sockaddr_in serverAdress;

    sockaddr_in clientService;
    serverAdress.sin_family = AF_INET;
    serverAdress.sin_addr.s_addr = inet_addr(serverIP.c_str());

    // Read the port number from terminal for the server
    int port;
    std::cout << "Enter a valid port: ";
    std::cin >> port;
    serverAdress.sin_port = htons(port);

    // Connect to the server
    if (connect(clientSocket, (sockaddr*)&serverAdress, sizeof(serverAdress)) == SOCKET_ERROR) {
        std::cerr << "Connection to server failed. Error: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server at " << serverIP << ":" << port << std::endl;

    while (true) {
        std::string msg;
        std::cout << "Enter a message to send (or type 'exit' to quit): ";
        std::getline(std::cin, msg);

        if (msg == "exit") {
            std::cout << "Exiting\n";
            break;
        }

        // (the connected_socket, pointer to the buffer of data, flags)
        int byte = send(clientSocket, msg.c_str(), sizeof(msg) - 1, 0);

        if (byte == SOCKET_ERROR) {
            std::cout << "Message sent to server." << WSAGetLastError() << "\n" << std::endl;
            break;
        }
        else {
            std::cout << "Message sent to server." << std::endl;
        }

        // Receive response from the server
        char response[1024];
        int bytesReceived = recv(clientSocket, response, sizeof(response) - 1, 0);
        if (bytesReceived > 0) {
            response[bytesReceived] = '\0'; // Null-terminate string
            std::cout << "Received from the server: " << response << std::endl;
        }
        else if (bytesReceived == 0) {
            std::cout << "The server closed the connection\n";
        }
        else {
            std::cout << "Failed to receive from the server " << WSAGetLastError() << "\n";
        }
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}

*/