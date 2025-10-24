# Variable Global

```c
extern volatile sig_atomic_t g_sigint_status;
```

Esta línea declara una **variable global** que se usa para **comunicar el estado de interrupción (Ctrl+C / SIGINT)** entre el **manejador de señales** y el **bucle principal de la shell**.

---

En una minishell, cuando el usuario pulsa `Ctrl+C`, el sistema envía una señal `SIGINT`. Para responder a esa señal sin interrumpir bruscamente la ejecución, se instala un **manejador de señales** (`signal(SIGINT, handler)`), que modifica esta variable global para indicar que ocurrió una interrupción.

Luego, el bucle principal de la shell puede consultar `g_sigint_status` y decidir cómo actuar: por ejemplo, limpiar la línea actual, mostrar un nuevo prompt, o actualizar el código de salida.

---

| Parte | Significado | Justificación en minishell |
|-------|-------------|-----------------------------|
| `extern` | Indica que la variable **se define en otro archivo** (no en el `.h`) | Permite que múltiples archivos `.c` accedan a la misma variable sin duplicarla |
| `volatile` | Le dice al compilador que **no optimice el acceso** a esta variable | Es crucial porque puede cambiar **fuera del flujo normal del programa**, por ejemplo, dentro de un manejador de señales |
| `sig_atomic_t` | Tipo seguro para operaciones atómicas con señales | Garantiza que leer o escribir esta variable **no se interrumpa** a mitad de operación por una señal |

---

Aunque `sig_atomic_t` suele ser equivalente a `int`, su uso garantiza que las operaciones sobre la variable sean **atómicas y seguras** en contexto de señales. Usar `int` podría provocar **comportamientos indefinidos** si el compilador optimiza el acceso o si se interrumpe una operación de escritura.

# Modo interactivo

En el contexto de una **minishell**, los términos *modo interactivo* y *modo no interactivo* se refieren a **cómo se comporta la shell dependiendo de si está recibiendo entrada directamente del usuario o desde otro proceso**. Esta distinción es clave para implementar correctamente el manejo de señales, el prompt, y la ejecución de comandos.

---

## Modo interactivo
- **Definición**: La shell está conectada directamente al terminal (TTY) y espera comandos del usuario.
- **Ejemplo típico**: El usuario abre la shell y escribe comandos como `ls`, `echo hola`, etc.
- **Características**:
  - Se muestra un **prompt** (`minishell$`).
  - Se espera entrada por teclado.
  - Se manejan señales como `SIGINT` (`Ctrl+C`) para **interrumpir el comando actual** sin cerrar la shell.
  - Se puede usar `readline()` para entrada enriquecida.

## Modo no interactivo
- **Definición**: La shell recibe comandos desde otro proceso, archivo o pipe, **sin interacción directa con el usuario**.
- **Ejemplo típico**:
  ```bash
  echo "ls -l" | ./minishell
  ./minishell < script.sh
  ```
- **Características**:
  - **No se muestra prompt**.
  - No se usa `readline()`, sino lectura directa (`read()` o `get_next_line()`).
  - Las señales como `SIGINT` pueden tener un comportamiento diferente (por ejemplo, terminar el proceso).
  - Se usa para ejecutar scripts o comandos encadenados con pipes/redirecciones.

---

Puedes usar `isatty()` para saber si la entrada estándar está conectada a un terminal:

Esto te permite condicionar el uso de `readline()`, mostrar el prompt, o ajustar el manejo de señales.

---

Cuando usas pipes (`|`) o redirecciones (`<`, `>`, `>>`), la shell entra en **modo no interactivo** para los comandos involucrados. Por ejemplo:

```bash
echo "ls -l" | ./minishell
```

Aquí, `./minishell` no recibe entrada del teclado, sino desde el pipe, por lo que no debe mostrar prompt ni esperar interacción.

---

- Para **evitar mostrar el prompt** cuando no hay usuario presente.
- Para **manejar correctamente las señales** (`SIGINT`, `SIGQUIT`) según el contexto.
- Para **decidir si usar `readline()` o lectura directa**.
- Para **respetar el comportamiento esperado de una shell real** como `bash`.

---
