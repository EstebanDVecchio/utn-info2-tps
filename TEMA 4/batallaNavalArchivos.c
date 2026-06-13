/*
Manejo de archivos en C:
1 – Utilizando el programa de la “Batalla Naval” del Trabajo Práctico Nº3, modifique el programa para
agregar “persistencia” y poder cumplir con los siguientes requisitos:

-Permita almacenar el ranking de jugadores guardando los datos en un archivo llamado
ranking.txt que contenga el nombre de cada jugador y la cantidad de disparos realizados. Este
archivo debe actualizarse cuando se termine un juego. A cada línea del archivo, agregue una marca
temporal (fecha y hora) para referencia.

-Al iniciar el juego, recupere el ranking de jugadores y lo almacene en memoria. De esta manera el
programa “recordará” quien ha sido el mejor jugador.

-En la pantalla principal, y como “incentivo” para los otros jugadores, muestre quién ha sido el
mejor jugador, y cuanto tiempo hace que no se supera el record.

-Almacene el “historial de disparos” de cada jugador en un archivo llamado
disparos_<nombre_del_jugador>.txt guardando en dicho archivo el dato fila, columna y resultado
(agua / hundido) separando los datos con una coma (,)

2 – Agregue al programa anterior la posibilidad de guardar el juego hasta el momento (tablero, disparos
realizados, etc) y la posibilidad de continuar un juego guardado
*/