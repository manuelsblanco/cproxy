#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

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
    

    return 0;
}
