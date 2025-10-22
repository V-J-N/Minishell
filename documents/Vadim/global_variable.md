
extern: se usa para declarar una variable global que está definida en otro archivo

sig_atomic: Es un tipo especial definido por la librería <signal.h>

Atómico: el sistema puede leer/escribir su valor en una sola operación, sin riesgo de corrupción.

Ideal para variables compartidas entre el programa normal y los signal handlers.

Le dice al compilador que el valor de esa variable puede cambiar en cualquier momento, fuera del control del programa.
En este caso, la variable puede cambiar cuando llega una señal, incluso en medio de otra función.

Sin volatile, el compilador podría optimizar el acceso a la variable
(por ejemplo, guardarla en un registro), y no notar que cambió su valor. Eso es peligroso cuando trabajas con señales.