#ifndef CONFIG_H
#define CONFIG_H

#include <netinet/in.h>
#include <stddef.h>
#include <stdint.h>

/* Balance strategies */
enum balance_strategy {
    BALANCE_RR = 0,
    BALANCE_LEAST_CONN = 1
};

/* Un backend: host en heap (strdup), puerto en host order */
struct backend {
    char *host;         /* heap-allocated */
    uint16_t port;      /* host byte order */
    unsigned int weight;
    unsigned int cur_conn; /* runtime (opcional) */
};

struct cproxy_config {
    uint16_t listen_port;      /* puerto en host order */

    struct backend *backends;  /* array dinámico (malloc/realloc) */
    size_t backend_count;
    size_t backend_capacity;

    enum balance_strategy strategy;

    unsigned int connect_timeout_ms;
    unsigned int read_timeout_ms;
    unsigned int idle_timeout_ms;

    unsigned int max_connections;
    int listen_backlog;

    char *log_file;            /* NULL -> stderr */
    int log_level;

    int allow_ipv6;
    int admin_port;

    size_t rr_next;            /* índice para RR (protegido externamente si multithread) */
};

/* Carga el archivo de configuración en 'out'. Retorna 0 = OK, <0 = error */
int config_load(const char *filename, struct cproxy_config *out);

/* Libera toda la memoria alocada dentro de cfg */
void config_free(struct cproxy_config *cfg);

/* Opcional: obtener pointer al singleton cargado (si decides implementar singleton) */
const struct cproxy_config *config_get(void);

#endif /* CONFIG_H */
