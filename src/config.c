#include <stdio.h>
#include "config.h"


// Function to load configuration from a file
int load_config(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: could not open configuration file '%s'.\n", filename);
        return 1;
    }

    printf("Config file loaded successfully.\n");

    fclose(file);
    return 0;
}
