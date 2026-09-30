# Enfrendados

Juego de dados por turnos para dos jugadores, en consola, hecho en **C++** para Programación I (Tecnicatura Universitaria en Programación, UTN).

![Turno de Enfrendados](https://federicowachenschwan.github.io/portfolio/img/enfrendados/turno.jpg)

## Mecánica del juego

- Dos dados de 12 caras marcan el **número objetivo** de la ronda.
- Cada jugador tira sus dados y elige primero **cuántos** va a combinar y después **cuáles**, para que sumen exactamente el objetivo.
- Si acierta, le pasa dados al rival; si no, recibe una penalización. Gana quien se queda sin dados o suma más puntos. Hay desempate con "última oportunidad".

## Conceptos aplicados

- Funciones y modularización en archivos `.h` / `.cpp`.
- Vectores, validación de datos y lógica de turnos.
- Interfaz de consola con recuadros, colores y dibujo de los dados (librería `rlutil`).

## Compilación

Abrir `Enfrendados.cbp` con Code::Blocks, o desde la terminal:

```
g++ -std=gnu++11 main.cpp funciones.cpp -o enfrendados
```

Proyecto individual de **Federico Wachenschwan** · [Ver el video en mi portfolio](https://federicowachenschwan.github.io/portfolio/#proyectos)
