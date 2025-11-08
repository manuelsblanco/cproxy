#include <stdio.h>   // fprintf, fopen, fgets
#include <stdlib.h>  // strtol
#include <string.h>  // strlen, strchr, strncpy, memmove

#include "config.h"

static struct config g_config;              // almacenamiento estático de la configuración cargada
static int cfg_listen_set = 0;              // bandera para saber si listen_port fue definido
static int cfg_backend_host_set = 0;        // bandera para saber si backend_host fue definido
static int cfg_backend_port_set = 0;        // bandera para saber si backend_port fue definido

static void trim(char *s) {
    char *start = s;                        // puntero que avanzará por el inicio de la cadena
    while (*start == ' ' || *start == '\t') {
        ++start;                            // elimina espacios iniciales avanzando el puntero
    }

    char *end = start + strlen(start);      // apunta al final (carácter nulo) de la cadena
    while (end > start && (end[-1] == '\n' || end[-1] == '\r' || end[-1] == ' ' || end[-1] == '\t')) {
        --end;                              // retrocede para quitar saltos de línea o espacios finales
    }
    *end = '\0';                            // coloca terminador justo después del último carácter válido

    if (start != s) {
        memmove(s, start, end - start + 1); // compacta la cadena al inicio si hubo espacios iniciales
    }
}

static int parse_port(const char *value, in_port_t *out_port) {
    char *endptr = NULL;                    // usaremos endptr para detectar conversiones inválidas
    long parsed = strtol(value, &endptr, 10); // convierte la cadena en base 10
    if (value[0] == '\0' || *endptr != '\0') {
        return -1;                          // falla si no se consumieron todos los caracteres
    }
    if (parsed < 1 || parsed > 65535) {
        return -1;                          // falla si está fuera del rango de puertos TCP
    }
    *out_port = (in_port_t)parsed;          // almacena el puerto validado
    return 0;                               // éxito
}

static int set_config_value(const char *key, const char *value) {
    if (strcmp(key, "listen_port") == 0) {
        if (parse_port(value, &g_config.listen_port) != 0) {
            fprintf(stderr, "Valor inválido para listen_port: %s\n", value);
            return -1;                      // aborta si el puerto es inválido
        }
        cfg_listen_set = 1;                 // marca que el campo fue seteado correctamente
        return 0;
    }
    if (strcmp(key, "backend_host") == 0) {
        strncpy(g_config.backend_host, value, sizeof(g_config.backend_host) - 1);
        g_config.backend_host[sizeof(g_config.backend_host) - 1] = '\0';  // asegura terminación
        cfg_backend_host_set = 1;            // marca que el host fue seteado
        return 0;
    }
    if (strcmp(key, "backend_port") == 0) {
        if (parse_port(value, &g_config.backend_port) != 0) {
            fprintf(stderr, "Valor inválido para backend_port: %s\n", value);
            return -1;                      // aborta si el puerto es inválido
        }
        cfg_backend_port_set = 1;           // marca que el puerto backend fue seteado
        return 0;
    }

    fprintf(stderr, "Clave desconocida en config: %s\n", key);  // avisa si la clave no está soportada
    return -1;                              // considera la clave desconocida como error
}

int load_config(const char *filename) {
    FILE *file = fopen(filename, "r");      // abre el archivo para lectura
    if (file == NULL) {
        fprintf(stderr, "No se pudo abrir el archivo de configuración '%s'.\n", filename);
        return -1;                          // aborta si no se pudo abrir
    }

    char line[512];                         // buffer temporal para cada línea
    int line_no = 0;                        // contador de línea para mensajes de error
    while (fgets(line, sizeof(line), file) != NULL) {
        ++line_no;                          // actualiza el número de línea actual
        trim(line);                         // limpia espacios y saltos
        if (line[0] == '\0' || line[0] == '#') {
            continue;                       // ignora líneas vacías o comentarios
        }

        char *sep = strchr(line, '=');      // busca el separador clave=valor
        if (sep == NULL) {
            fprintf(stderr, "Línea %d inválida: falta '='.\n", line_no);
            fclose(file);
            return -1;                      // aborta si no hay separador
        }

        *sep = '\0';                        // separa en dos cadenas independientes
        char *key = line;                   // parte izquierda como clave
        char *value = sep + 1;              // parte derecha como valor
        trim(key);                          // limpia espacios de la clave
        trim(value);                        // limpia espacios del valor

        if (set_config_value(key, value) != 0) {
            fprintf(stderr, "Error al procesar la línea %d.\n", line_no);
            fclose(file);
            return -1;                      // aborta si la asignación falló
        }
    }

    fclose(file);                           // cierra el archivo una vez leído

    if (!cfg_listen_set || !cfg_backend_host_set || !cfg_backend_port_set) {
        fprintf(stderr, "Config incompleta: listen_port=%d backend_host=%d backend_port=%d\n",
                cfg_listen_set, cfg_backend_host_set, cfg_backend_port_set);
        return -1;                          // aborta si faltan campos obligatorios
    }

    printf("Config file loaded successfully.\n");  // confirma que todo salió bien
    return 0;                               // éxito
}

const struct config *get_config(void) {
    return &g_config;                       // expone un puntero solo de lectura a la config cargada
}
