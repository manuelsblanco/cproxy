#include "listener.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>

struct listener {
    int fd;
    in_port_t port;
};

struct listener *listener_create(in_port_t port) {
    struct listener *listener = malloc(sizeof(*listener));
    if (listener == NULL) {
        return NULL;
    }
    listener->fd = -1;
    listener->port = port;
    return listener;
}

static int listener_configure_socket(struct listener *listener) {
    int yes = 1;
    if (setsockopt(listener->fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(listener->port);

    if (bind(listener->fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        return -1;
    }
    if (listen(listener->fd, SOMAXCONN) < 0) {
        return -1;
    }
    return 0;
}

int listener_start(struct listener *listener) {
    if (listener == NULL) {
        return -1;
    }
    if (listener->fd != -1) {
        return 0;
    }

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        return -1;
    }

    listener->fd = fd;
    if (listener_configure_socket(listener) < 0) {
        listener_close(listener);
        return -1;
    }
    return 0;
}

int listener_get_fd(const struct listener *listener) {
    if (listener == NULL) {
        return -1;
    }
    return listener->fd;
}

void listener_close(struct listener *listener) {
    if (listener == NULL) {
        return;
    }
    if (listener->fd >= 0) {
        close(listener->fd);
        listener->fd = -1;
    }
}

void listener_destroy(struct listener *listener) {
    if (listener == NULL) {
        return;
    }
    listener_close(listener);
    free(listener);
}
