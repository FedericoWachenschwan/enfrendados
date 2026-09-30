
#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <ctime>
#include "funciones.h"
#include "rlutil.h"
#include <windows.h>
using namespace std;



int main()
{
    srand(time(0)); // Inicializa la semilla para los nímeros aleatorios.
    //Declaro estructura para jugadores
    SetConsoleOutputCP(65001); //Muestra todo en UTF-8, así se ven bien los tildes, la ñ y los caracteres especiales
    mostrarPortada();

    string nombreJugador1, nombreJugador2;
    int puntajeJugador1 = 0, puntajeJugador2 = 0;
    // " 70" es La cantidad de dados de todos los jugadores, que es 250, pero no se usa todo normalmente por eso el numero grande
    int dadosStockJ1[70] = {};
    int dadosStockJ2[70] = {}; // El corchete pone todos los elementos (los de 6) en 0
    int cantDadosJ1 = 6; // Cantidad de dados que tiene cada jugador (se empieza con 6, pero va cambiando)
    int cantDadosJ2 = 6;
    bool partidaJugada = false; // Para que no se muestre una estadistica nula

    // Lo de arriba parece ser lo mismo, pero dadosStock guarda QUÉ dados tiene, y cantDados guarda CUÁNTOS dados tiene cada jugador.
    int opcion;

    bool finalizar = false;

    /*
        1- Mientras sea diferente a finalizar se mostrara el menu con las opciones.
        2- Se usa un switch como condicional para que se ejecute la opcion ingresada por el usuario.
        3- A medida de que el juego sea desarrollado se debera ir modificando el menu de iociones.
    */
    string nombreMejorJugadorHistorico = "Nadie";
    int maxPuntajeHistorico = 0;
    while (!finalizar)
    {

        rlutil::cls();
        opcion = menuOpciones();

        switch (opcion)
        {
        case 0:
            if (confirmarSalida() == true)
            {
                finalizar = true;
            }
            break;

        case 1:
            // Pedir nombres
            cin.ignore(); // Limpia el buffer
            pedirNombres(nombreJugador1, nombreJugador2);
            if (partidaJugada == false)   // Solo la activamos una vez para indicar que al menos una partida se jugó
            {
                partidaJugada = true;
            }

            // Decidir quién empieza
            int quienEmpieza;
            quienEmpieza = decidirQuienEmpieza(nombreJugador1, nombreJugador2);
            puntajeJugador1 = 0;
            puntajeJugador2 = 0;
            cantDadosJ1 = 6;
            cantDadosJ2 = 6;
            // Son todos reinicios de variables para cada vez que se inicia la partida

            if (quienEmpieza == 0)
            {
                // llama a la funcion orientada al primer jugador (el último número indica que el que empieza es el jugador 1)
                jugarPartida(nombreJugador1, nombreJugador2, dadosStockJ1, dadosStockJ2, cantDadosJ1, cantDadosJ2, puntajeJugador1, puntajeJugador2, 1);
            }
            else
            {
                // llama a la funcion orientada al segundo jugador (el que empieza es el jugador 2)
                jugarPartida(nombreJugador2, nombreJugador1, dadosStockJ2, dadosStockJ1, cantDadosJ2, cantDadosJ1, puntajeJugador2, puntajeJugador1, 2);
            }


            // termina la partida, y lo siguiente es ver si alguien hizo un récord histórico
            bool nuevoRecord;
            nuevoRecord = false;

            if (puntajeJugador1 > puntajeJugador2)   // Si tiene mas puntaje el jugador 1
            {
                if (puntajeJugador1 > maxPuntajeHistorico)   // Si tiene mas puntaje que el maximo historico, que empieza con 0
                {
                    maxPuntajeHistorico = puntajeJugador1; // Se le da ese titulo al jugador 1
                    nombreMejorJugadorHistorico = nombreJugador1; // y el nombre tambien
                    nuevoRecord = true;
                }
            }
            else   // Si no se cumple el primer if, quiere decir que tiene más (o empate) el jugador 2
                if (puntajeJugador2 > maxPuntajeHistorico)   // lo mismo que para el jugador 1 si se gana
                {
                    maxPuntajeHistorico = puntajeJugador2;
                    nombreMejorJugadorHistorico = nombreJugador2;
                    nuevoRecord = true;
                }

            // Muestra el puntaje final, el ganador y si hubo récord
            mostrarFinDePartida(nombreJugador1, nombreJugador2, puntajeJugador1, puntajeJugador2, cantDadosJ1, cantDadosJ2, nuevoRecord);
            break;

case 2:
            estadisticasDelJuego(nombreJugador1, nombreJugador2, puntajeJugador1, puntajeJugador2, cantDadosJ1, cantDadosJ2, nombreMejorJugadorHistorico, maxPuntajeHistorico, partidaJugada);
            break;

        case 3:
            creditos();

            break;

        case 4:
            reglamento();

            break;



        }
    }



    return 0;
}

