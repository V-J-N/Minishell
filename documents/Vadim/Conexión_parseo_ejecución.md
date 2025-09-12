# Resumen de Cambios en la Rama

## 1. Reescritura de ejecución, pipes y redirección

Se ha reescrito la lógica de ejecución, gestión de pipes y redirecciones para trabajar directamente con la estructura de datos parseados (`t_command`).  
Ahora, la ejecución de comandos y la gestión de pipes se realiza iterando sobre la lista enlazada de nodos de comandos generada por el parser, lo que permite un flujo más claro y flexible.

---

## 2. Preparación de `main` para tests básicos

Se ha añadido lo esencial en `main` para poder realizar tests básicos tanto de comandos simples como de comandos encadenados mediante pipes.  
Esto facilita la validación de la nueva lógica de ejecución y el parser.

---

## 3. Nuevas funciones auxiliares en `parser_utils.c`

Se han añadido dos funciones auxiliares:
- **`number_of_commands`**: Devuelve el número de nodos de comandos en la lista parseada.
- **`number_of_redirs`**: Devuelve el número de redirecciones en la lista (puede que no sea necesaria finalmente).

Además, se confirma que el campo `argument_count` dentro de cada nodo de comando es necesario para la ejecución, por lo que debe conservarse.

---

## 4. Mejoras en el parser: separación de comandos por pipes

En `parser.c`, dentro de la función `parse_command()`, se ha añadido la creación de nuevos nodos de comandos separados por pipes, siguiendo la sugerencia de la IA.  
Esta mejora ha permitido que los tests con pipes funcionen correctamente, aunque conviene revisar el código para asegurar su robustez.

---

## 5. Trabajo pendiente

- Realizar más tests y comprobar posibles memory leaks.
- Reformular el manejo de múltiples redirecciones en un mismo bloque de comandos.
- En la ejecución, decidir cómo proceder si se recibe un path relativo (posiblemente usando `getcwd()` o similar).
- Más adelante, implementar el manejo de la opción `~` para rutas de usuario.

---

## Explicación paso a paso: Flujo de `t_command` en ejecución y pipes

1. **main()**  
   Llama a `parse_command(tokenlist)` para obtener una lista enlazada de estructuras `t_command` (cada nodo representa un comando con sus argumentos y redirecciones).

2. **execute_all(commands, &environment)**  
   Recibe la lista de comandos y decide:
   - Si hay un solo comando, llama a `command_in`.
   - Si hay varios, inicializa la estructura `t_pipe` y llama a `pipes`.

3. **command_in**  
   Ejecuta un comando individual, gestionando redirecciones y llamando a `execute_command`.

4. **pipes**  
   Itera sobre la lista de comandos (`t_command *commands` dentro de `t_pipe`), y para cada uno:
   - Crea procesos hijos.
   - Llama a `command_in` para cada comando, gestionando los pipes y redirecciones.

5. **execute_command**  
   Convierte la estructura `t_command` en un array de argumentos y ejecuta el binario con `execve`.

---

**Resumen:**  
La estructura `t_command` fluye desde el parseo en `main`, pasa a `execute_all`, y de ahí a `command_in` (para un comando) o a través de la estructura `t_pipe` y la función `pipes` (para varios comandos con pipes), llegando finalmente a `execute_command` donde se ejecuta