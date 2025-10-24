
Explicación rápida

sigemptyset(&sa.sa_mask) → no bloquea señales adicionales durante la ejecución del handler.

SA_RESTART → reintenta automáticamente llamadas al sistema interrumpidas (como read() en readline).

sigaction(SIGINT, &sa, NULL) → reemplaza la forma antigua signal(SIGINT, handler) pero de forma segura y portable.

Cuando haces un fork(), el hijo hereda las acciones de señal del padre, pero en el child debes restaurar el comportamiento por defecto, ya que si no, Ctrl+C no lo matará.

1. sigemptyset no es una función externa prohibida

El subject de 42 sólo restringe llamadas a funciones externas (es decir, funciones que se resuelven por símbolos en las librerías estándar, como malloc, printf, etc.).

Sin embargo, sigemptyset (y el resto de funciones de manipulación de conjuntos de señales) no son llamadas del sistema ni funciones externas “de alto nivel”, sino macros o funciones inline definidas en <signal.h>.
👉 En la práctica, el compilador las reemplaza por operaciones sobre una estructura (sigset_t), y no implican una llamada real a libc.

Por eso:

No rompen las reglas del subject.

Son necesarias para usar correctamente sigaction().

Y todos los correctores de 42 aceptan su uso (de hecho, lo esperan).

