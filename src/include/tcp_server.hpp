#ifndef TCP_SERVER
#define TCP_SERVER

#include <iostream>
#include <thread>
#include <atomic>
#include <sys/socket.h>
#include <netinet/in.h>

typedef void (*connection_handler)(int);

struct TCPServerState {
    int port;
    int max_connections;
    int socket_fd;
    std::atomic<int> thread_count;
};

class TCPServer {
    
    private:
        TCPServerState state;
        connection_handler handler;

        void thread_entry();
        void invoke_thread();

    public:
        void bind_settings(int port, int max_connections);
        void bind_connection_handler(connection_handler handler);
        void start();

};

#endif