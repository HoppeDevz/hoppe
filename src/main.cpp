#include <iostream>
#include <unistd.h>

#include <http_request_buffer_parser.hpp>
#include <tcp_server.hpp>


void handler(int client_fd) {

    char buffer[4096]{0};
    read(client_fd, buffer, sizeof(buffer));

    for (int i = 0; i < 4096; i++) {
        std::cout << buffer[i];
    }
    std::cout << std::endl;

    HttpRequest req{};
    HttpRequestParser::parse_http_request_buffer(buffer, 4096, &req);

    std::cout << "Method: " << req.details.method << std::endl;
    std::cout << "Route: " << req.details.route << std::endl;
    std::cout << "HTTP Version: " << req.details.http_version << std::endl;

}

int main() {

    TCPServer* tcp_server = new TCPServer();

    tcp_server->bind_settings(30120, 1024);
    tcp_server->bind_connection_handler(handler);
    tcp_server->start();

    getchar();
    return 0;

}
