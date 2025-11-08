#ifndef CONFIG_H
#define CONFIG_H

#include <netinet/in.h>

struct config {
    in_port_t listen_port;
    char backend_host[256];
    in_port_t backend_port;
};

int load_config(const char *filename);
const struct config* get_config();

#endif
