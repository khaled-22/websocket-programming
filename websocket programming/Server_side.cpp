#include <iostream>
#include <winsock.h>
#include <string_view>
#include <fstream>
#include <string>
#include <csignal>
#include <atomic>

#pragma comment(lib, "ws2_32.lib") // Link the Winsock library

void log_keeper(const std::string_view message) {
    std::ofstream log_file("Server_error.log", std::ios::app);
    if (log_file.is_open()) {
        log_file << message << "\n";
        log_file.close();
    }
}

// Using a namespace to avoid use of global variables
namespace sock_manager {
    sockaddr_in clientAddr; // used to store the Address, Port and type later in the bind
    sockaddr_in serverAddr;

    // SOCKETS
    SOCKET acceptSocket;
    SOCKET listenSocket;
}

std::atomic<bool> server_running{ true };

// Signal handler for SIGINT (Ctrl + C) and SIGTERM
void signal_handler(int signal) {

    if (signal == SIGINT || signal == SIGTERM) {
        std::cout << "The interrupt signal is (" << signal << ") \n";
        log_keeper("There was an interrupt signal from the server");

        server_running = false;
        // Close the sockets if they are open
        if (sock_manager::acceptSocket != INVALID_SOCKET) {
            closesocket(sock_manager::acceptSocket);
        }
        if (sock_manager::listenSocket != INVALID_SOCKET) {
            closesocket(sock_manager::listenSocket);
        }
        WSACleanup();
        //exit(signal);
    }
}

int main() {
    // Register interrupt signal
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    WSADATA wsaData; // The struct used to store information about the WinSock implementation

    int addrLen = sizeof(sock_manager::clientAddr);

    // 1. Initialize Winsock
    int was_err;
    WORD versionRequesed = MAKEWORD(2, 2); // Version 2.2
    was_err = WSAStartup(versionRequesed, &wsaData);
    if (was_err != 0) {
        std::cerr << "Winsock initialization failed!" << std::endl;
        log_keeper("Winsock initialization failed!");
        return 0;
    }
    else {
        std::cout << "The Winsock was initialized\n";
        std::cout << "The status " << wsaData.szSystemStatus << "\n";
    }

    // 2. Create a socket
    // creates a socket that is bound to a specific transport service provider
    std::cout << "Step 2 - Set up the socket server\n";

    sock_manager::listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // AF_INET: IPv4 for UDP or TCP || SOCK_STREAM: TCP 

    if (sock_manager::listenSocket == INVALID_SOCKET) { // Unsure if it returns null or not
        std::cerr << "Failed to create server socket!" << WSAGetLastError() << std::endl; // get the last error for the socket
        log_keeper("Failed to create server socket!");

        WSACleanup();
        return 0;
    }
    else {
        std::cout << "The socket() is created\n";
    }
    std::cout << "\n";

    // 3. Bind the socket to an address and port. Prepare the socket to receive from a client
    std::cout << "3. Bind of the socket\n";

    sock_manager::serverAddr.sin_family = AF_INET;

    // Use input IP and port number
    std::string m_ip;
    std::cout << "Enter IP: ";
    std::cin >> m_ip;
    sock_manager::serverAddr.sin_addr.s_addr = inet_addr(m_ip.c_str());

    int portInput;
    std::cout << "Enter a valid port: ";
    std::cin >> portInput;
    sock_manager::serverAddr.sin_port = htons(portInput); // Comes from console input

    if (bind(sock_manager::listenSocket, (SOCKADDR*)&sock_manager::serverAddr, sizeof(sock_manager::serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed!" << std::endl;
        log_keeper("Bind failed!");

        closesocket(sock_manager::listenSocket);
        WSACleanup();
        return 0;
    }
    else { std::cout << "Bind() ok\n"; }

    // 4. Listen for connections
    std::cout << "\n";
    std::cout << "4. Listen for a client\n";
    if (listen(sock_manager::listenSocket, 3) == SOCKET_ERROR) { // Listen on listenSocket, number of clients/connections
        std::cerr << "Listen failed!" << std::endl;
        log_keeper("Listen failed!");
        closesocket(sock_manager::listenSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Server listening on port " << portInput << std::endl;
    log_keeper("Server started and listening on port " + std::to_string(portInput) + "...");

    std::cout << "\n";

    // 5. Accept a client connection
    std::cout << "5. Waiting for an accept call from the client\n";

    //clientSocket = accept(listenSocket, NULL, NULL);

    while (server_running) {
        sock_manager::acceptSocket = accept(sock_manager::listenSocket, (sockaddr*)&sock_manager::clientAddr, &addrLen);
        if (sock_manager::acceptSocket == INVALID_SOCKET) {
            if (!server_running) break;
            std::cerr << "Failed to accept client connection!" << WSAGetLastError() << std::endl;
            log_keeper("Failed to accept client connection!");
            continue;
            WSACleanup();
        }
        std::cout << "Client connected!" << std::endl;

        // Send message to client

        // 6. Communicate with the client
        constexpr int size = 1024;
        char buffer[size];
        while (server_running) {
            int bytesReceived = recv(sock_manager::acceptSocket, buffer, sizeof(buffer) - 1, 0); // (from where, buffer to store, error handling for null character, flag)
            if (bytesReceived > 0) {
                buffer[bytesReceived] = '\0'; // Null-terminate string
                std::cout << "Received: " << buffer << std::endl;

                // Respond to the client
                std::string response = "Hello from server!\n";
                int bytes_send = send(sock_manager::acceptSocket, response.c_str(), response.size(), 0);
                if (bytes_send == SOCKET_ERROR) {
                    std::cerr << "Failed to send a response: " << WSAGetLastError() << "\n";
                    log_keeper("Failed to send a response!");
                    break;
                }
                else {
                    std::cout << "Response sent to the client: " << response << "\n";
                }
            }
            else if (bytesReceived == 0) {
                std::cout << "Client disconnected.\n";
                log_keeper("The client has disconnected!");

                break;
            }
            else {
                std::cerr << "recv failed: " << WSAGetLastError() << "\n";
                log_keeper("recv failed");
                break;
            }
        }

        closesocket(sock_manager::acceptSocket);
        //std::system("pause");
    }
    // 7. Close sockets
    std::cout << "Shutting down server\n";
    closesocket(sock_manager::acceptSocket);
    closesocket(sock_manager::listenSocket);

    WSACleanup();

    return 0;
}
