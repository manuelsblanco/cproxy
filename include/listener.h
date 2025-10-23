#ifndef LISTENER_H
#define LISTENER_H


#include <netinet/in.h>   // struct sockaddr_in, in_port_t
#include <sys/socket.h>   // socklen_t, constants como SOCK_STREAM

struct listener;

/* Reserva y configura la estructura listener con el puerto indicado.
 * Retorna NULL si no hay memoria.
 */
struct listener *listener_create(in_port_t port);

/* Crea el socket, aplica opciones (p. ej. SO_REUSEADDR), hace bind y listen.
 * Devuelve 0 en éxito, -1 en error.
 */
int listener_start(struct listener *listener);

/* Expone el file descriptor del socket de escucha para registrarlo en el loop. */
int listener_get_fd(const struct listener *listener);

/* Cierra el socket del listener si está activo. */
void listener_close(struct listener *listener);

/* Libera la memoria asociada a la estructura listener. */
void listener_destroy(struct listener *listener);

#endif  // LISTENER_H       