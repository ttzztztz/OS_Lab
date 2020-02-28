#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <cstring>

using std::cout, std::endl;

const char *PATH = "/tmp/rabbit.sock";
const char *BUF = "hzytql!";

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

int main() {
    int sock_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    unlink(PATH);

    struct sockaddr_un server_addr{};
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_LOCAL;
    strcpy(server_addr.sun_path, PATH);
    bind(sock_fd, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr));
    listen(sock_fd, 1);

    while (true) {
        struct sockaddr_un client_addr{};
        socklen_t client_addr_len = sizeof(client_addr);
        int conn_fd = accept(sock_fd, reinterpret_cast<sockaddr *>(&client_addr), &client_addr_len);
        if (conn_fd < 0) {
            cout << "accept error" << endl;
            continue;
        }

        printf("recv %d \n", conn_fd);

        write(conn_fd, BUF, strlen(BUF));
        close(conn_fd);
    }

    return 0;
}

#pragma clang diagnostic pop