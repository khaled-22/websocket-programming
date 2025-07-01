# C++ TCP Client-Server Chat (Winsock)
This project demonstrates a simple **TCP client-server communication system** using **C++ and the Winsock API** on Windows. It allows a user to connect to a server, send messages, and receive responses in real-time.


##  Features

-  Server can accept client connections and respond to messages
-  Client can send messages to the server and receive replies
-  Uses **TCP sockets (SOCK_STREAM)** for reliable communication
-  Logs errors and system events to a `Server_error.log` file
-  Graceful shutdown via signal handling (e.g. Ctrl+C)
-  Uses **modular code** (e.g. namespacing, error handling)
-  Illustrates how to use Winsock on Windows systems


##  Technologies Used

- C++
- Winsock2 (WSA)
- Windows APIs (`ws2_32.lib`)
- `signal`, `fstream`, `atomic`, `string_view`
