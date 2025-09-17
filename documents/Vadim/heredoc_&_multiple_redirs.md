# Resumen de Cambios de la Rama

## PRINCIPALES CAMBIOS

### 1. Ejecución y manejo avanzado de redirecciones

Se han añadido nuevas funciones y condiciones en la parte de ejecución y gestión de redirecciones para imitar el comportamiento de bash, permitiendo:

- **Manejo de múltiples redirecciones**  
  Ahora es posible gestionar correctamente varios tipos de redirecciones (entrada, salida, append, heredoc) tanto en comandos simples como dentro de pipelines.  
  El flujo de ejecución revisa todas las redirecciones asociadas a cada bloque de comando, aplicando la última encontrada de cada tipo, como hace bash.

- **Redirecciones dentro y fuera de pipes**  
  El sistema de ejecución ha sido adaptado para que cada comando dentro de un pipeline pueda tener sus propias redirecciones, gestionando correctamente los file descriptors y asegurando que no se produzcan interferencias entre comandos encadenados.

### 2. Manejo robusto de heredoc

- **Adaptación a ejemplos reales de bash**  
  El manejo de heredoc (`<<`) se ha ajustado para comportarse como en bash, incluyendo la gestión de señales (Ctrl+C) y la correcta transmisión del contenido al comando correspondiente.

- **Nueva variable `heredoc_fd` en la estructura de redirección**  
  Se ha añadido un campo `heredoc_fd` en la estructura `t_redir` (dentro de cada nodo de la lista de comandos) para almacenar el file descriptor temporal asociado al heredoc.  
  Esto permite que cada comando pueda acceder a su propio heredoc, incluso dentro de pipelines complejos.

- **Liberación segura de recursos**  
  En la función `free_redirs`, se ha añadido una condición para cerrar correctamente el file descriptor de heredoc (`heredoc_fd`) si está abierto (mayor que 2), evitando fugas de recursos y asegurando que no queden descriptores abiertos tras la ejecución.

---

## Detalles Técnicos

- **Redirecciones múltiples:**  
  El código recorre la lista de redirecciones de cada comando y aplica secuencialmente cada una, cerrando los file descriptors anteriores si es necesario.  
  Esto permite que, por ejemplo, un comando como `cat < in1 < in2 > out1 > out2` utilice solo la última redirección de cada tipo, como hace bash.

- **Integración con pipes:**  
  Cada proceso hijo creado para un comando dentro de un pipeline configura sus propias redirecciones antes de ejecutar el comando, asegurando que los flujos de entrada y salida sean los correctos.

- **Heredoc:**  
  El heredoc se gestiona en un proceso hijo separado, y su file descriptor se almacena en el nodo de redirección correspondiente.  
  Si el usuario interrumpe el heredoc con Ctrl+C, solo se cancela el heredoc y no el minishell completo.

- **Liberación de recursos:**  
  Al liberar la lista de redirecciones (`free_redirs`), se comprueba si el tipo es HEREDOC y si el file descriptor está abierto, cerrándolo para evitar leaks.

---

## Resumen

Estos cambios permiten que el minishell gestione correctamente escenarios complejos de redirección y heredoc, tanto en comandos simples como en pipelines, acercando su comportamiento al de bash y mejorando la robustez y limpieza de recursos del