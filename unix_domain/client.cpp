#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <cstring>

using std::cout, std::endl;

const char *PATH = "/tmp/rabbit.sock";

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

int main() {
    int sock_fd = socket(AF_LOCAL, SOCK_STREAM, 0);

    struct sockaddr_un server_addr{};
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_LOCAL;
    strcpy(server_addr.sun_path, PATH);
    connect(sock_fd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));

    char buff[10] = {0};
    int _read = 0;
    while ((_read = read(sock_fd, buff, sizeof(buff))) > 0) {
        for (int i = 0; i < _read; i++) printf("%c", buff[i]);
    }
    close(sock_fd);

    return 0;
}

#pragma clang diagnostic pop