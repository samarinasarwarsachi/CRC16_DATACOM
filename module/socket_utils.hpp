#ifndef SOCKET_UTILS_HPP
#define SOCKET_UTILS_HPP

#include <cstddef>
#include <cstdint>
#include <unistd.h>
#include <sys/socket.h>

inline bool send_all(int fd, const uint8_t* data, size_t len) {
    size_t sent = 0;
    while (sent < len) {
        ssize_t r = send(fd, data + sent, len - sent, 0);
        if (r <= 0) return false;
        sent += static_cast<size_t>(r);
    }
    return true;
}

inline bool recv_all(int fd, uint8_t* data, size_t len) {
    size_t got = 0;
    while (got < len) {
        ssize_t r = recv(fd, data + got, len - got, 0);
        if (r <= 0) return false;
        got += static_cast<size_t>(r);
    }
    return true;
}

#endif