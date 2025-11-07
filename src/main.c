#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "listener.h"

#include "config.h"

static int listener_accept_loop(struct listener *listener) {
    int status = 0;
    while (status == 0) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        int client_fd = accept(listener_get_fd(listener),
                               (struct sockaddr *)&client_addr,
                               &client_len);
        if (client_fd < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("accept");
            return -1;
            break;
        }

        char ip_str[INET_ADDRSTRLEN] = "desconocido";
        if (inet_ntop(AF_INET, &client_addr.sin_addr, ip_str, sizeof(ip_str)) == NULL) {
            strncpy(ip_str, "invalido", sizeof(ip_str));
            ip_str[sizeof(ip_str) - 1] = '\0';
        }

        printf("Conexión entrante de %s:%u\n", ip_str, ntohs(client_addr.sin_port));
        close(client_fd);
    }
    return status;
}

static int run_listener(in_port_t port){
    struct listener *listener = listener_create(port);
    if (listener == NULL) {
        fprintf(stderr, "No se pudo crear el listener en el puerto %u\n", port);
        return -1;
    }
    if (listener_start(listener) < 0) {
        fprintf(stderr, "No se pudo iniciar el listener en el puerto %u\n", port);
        listener_destroy(listener);
        return -1;
    }
    printf("Listener iniciado en el puerto %u\n", port);

    const int loop_status = listener_accept_loop(listener);
    listener_destroy(listener);
    return loop_status;
}

int main(int argc, char *argv[]){
    printf("cproxy v0.1 - Reverse Proxy in C\n");
    if (argc < 3) {
        printf("Usage: cproxy --config <file>\n");
        return 1;
    }
    if (strcmp(argv[1], "--config") == 0) {
        printf("Using config file: %s\n", argv[2]);
        if (load_config(argv[2]) != 0) {
            printf("Failed to load configuration file.\n");
            return 1;
        }
    }
    else {
        printf("Usage: cproxy --config <file>\n");
        return 1;
    }
    const in_port_t port = 8080;  // TODO: obtener desde la configuración
    if (run_listener(port) != 0) {
        return 1;
    }
    return 0;
}
