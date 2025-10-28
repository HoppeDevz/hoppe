
#include <tcp_server.hpp>

void TCPServer::bind_settings(int port, int max_connections) {

    this->state.port = port;
    this->state.max_connections = max_connections;

}

void TCPServer::bind_connection_handler(connection_handler handler) {

    this->handler = handler;

}

void TCPServer::start() {

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(this->state.port);

    int opt = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

    // TODO: Error handling
    bind(socket_fd, (sockaddr*)&addr, sizeof(addr));
    listen(socket_fd, this->state.max_connections);

    this->state.socket_fd = socket_fd;
    this->invoke_thread();

}

void TCPServer::invoke_thread() {

    if (this->state.thread_count <= 10) {

        std::thread t([this]() {
            this->thread_entry();
        });

        t.detach();

    } else {

        // TODO: Warning/Error handling
        std::cout << "[WARNING] - Thread overflow" << std::endl;

    }
    

}
void TCPServer::thread_entry() {
    
    this-state.thread_count++;

    while (true) {

        int client = accept(this->state.socket_fd, nullptr, nullptr);

        if (client != -1) {
            this->invoke_thread();
            this->handler(client);
        } else {
            continue;
        }

        this-state.thread_count--;
        break;

    }

}
