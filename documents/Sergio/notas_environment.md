Estoy haciendo prueba por si hay fallos intermedios que no hhaya leaks de memoria.

![alt text](<img/Captura desde 2025-10-24 19-47-10.png>)

He introducido esto en el codigo para que falle el malloc y ver si libera bien hacia arriba.

![alt text](<img/Captura desde 2025-10-24 19-53-07.png>)

Salida perfecta y sin leaks!

El shell level tambien funciona perfectamente.

![alt text](<img/Captura desde 2025-10-24 20-08-23.png>)

He añadido alguna protecciones adicionales aqui por si algun ft_strdup falla

![alt text](<img/Captura desde 2025-10-24 20-48-29.png>)

He añadido esta linea por si acaso variables parecidas daban falsos positivos

![alt text](<img/Captura desde 2025-10-24 21-05-20.png>)

![alt text](<img/Captura desde 2025-10-24 21-07-59.png>)

