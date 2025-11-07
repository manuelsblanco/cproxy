# cproxy

Pequeño reverse proxy escrito en C. Actualmente el proyecto solo levanta un listener TCP básico y acepta conexiones entrantes, sin reenviarlas a ningún backend.

## Lo que ya hicimos
- `main.c`: parseo mínimo de argumentos (`--config`) y bootstrap del listener en un puerto fijo de prueba.
- `listener.c`: creación/configuración del socket (`SO_REUSEADDR`, `bind`, `listen`) y ciclo de aceptación básico.
- `main.c` (`listener_accept_loop`): registro de conexiones entrantes y cierre ordenado del bucle cuando `accept` falla tras el shutdown del listener.
- `config.c`: stub que abre un archivo de configuración y verifica que exista (sin parsear contenido aún).

## Lo que falta para completar el proyecto
- **Formato de configuración**
  - Definir la estructura en cabeceras (puerto de escucha, backend host/puerto, timeouts, etc.).
  - Implementar el parser real (p. ej. pares clave=valor o JSON simple) y validaciones.
  - Integrar los valores en `main.c` para eliminar los hardcodes (puerto, destinos).
- **Bucle de proxy**
  - Diseñar cómo manejar cada conexión (bloqueante, hilos, pool, `select`/`epoll`).
  - Implementar la función `handle_client_connection` (o equivalente) que conecte al backend y haga relay bidireccional entre `client_fd` y `backend_fd`.
  - Añadir timeouts, cierre limpio y logs de éxito/error por conexión.
- **Recursos y robustez**
  - Manejar señales (SIGINT/SIGTERM) para detener el loop estableciendo un flag global.
  - Implementar backoff o límites ante errores repetidos (`EMFILE`, `ECONNABORTED`).
  - Añadir métricas/logging estructurado (niveles, timestamps) para facilitar el debug.
- **Testing y herramientas**
  - Crear binarios de prueba (scripts o `Makefile` targets) para lanzar el proxy y un backend dummy.
  - Escribir pruebas unitarias/integración básicas para el parser y el listener.
  - Configurar sanitizers (`-fsanitize=address,undefined`) y opciones de compilación estrictas (`-Wall -Wextra -Werror`).
- **Documentación y empaquetado**
  - Explicar el formato de configuración con ejemplos.
  - Documentar cómo compilar, ejecutar y probar.
  - Preparar versiones futuras (por ejemplo, soporte TLS o múltiples backends) una vez cubierto lo esencial.

Usa esta lista como guía: implementa cada bloque en orden y ve marcando lo completado en este README para mantener la visibilidad del progreso.
