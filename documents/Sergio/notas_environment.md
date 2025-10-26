Estoy haciendo prueba por si hay fallos intermedios que no hhaya leaks de memoria.

![alt text](<img/Captura desde 2025-10-24 19-47-10.png>)

He introducido esto en el codigo para que falle el malloc y ver si libera bien hacia arriba.

![alt text](<img/Captura desde 2025-10-24 19-53-07.png>)

Salida perfecta y sin leaks!

El shell level tambien funciona perfectamente.

![alt text](<img/Captura desde 2025-10-24 20-08-23.png>)

He añadido alguna protecciones adicionales aqui por si algun ft_strdup falla

![alt text](<img/Captura desde 2025-10-24 20-48-29.png>)

Si en algun momento esto es temp->value es NULL, nos deberiamos proteger
he modificado esto en la libft

![alt text](<img/Captura desde 2025-10-24 23-41-58.png>)

Aqui tenemos si temp->value NULL devolvemos NULL y si no se encuentra una cadena vacia, ya que luego en el expanser hacemos join, no quiero pasar hay un NULL.

![alt text](<img/Captura desde 2025-10-24 23-40-31.png>)




