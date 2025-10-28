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

    // HttpComponentRange start_line_range{};
    // HttpComponentRange request_headers_range{};
    // HttpComponentRange body_range{};
    // HttpHeaderParser::calculate_components_range(buffer, 4096, &start_line_range, &request_headers_range, &body_range);

    // std::cout << "startline range: " << start_line_range.start_at << " , " << start_line_range.finish_at << std::endl;
    // std::cout << "req headers range: " << request_headers_range.start_at << " , " << request_headers_range.finish_at << std::endl;
    // std::cout << "req body range: " << body_range.start_at << " , " << body_range.finish_at << std::endl;

}

int main() {

    TCPServer* tcp_server = new TCPServer();

    tcp_server->bind_settings(30120, 1024);
    tcp_server->bind_connection_handler(handler);
    tcp_server->start();

    getchar();
    return 0;

}
