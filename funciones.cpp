#include <iostream>
#include <cstdlib>
#include <ctime>
#include "funciones.h"
#include "rlutil.h"
#include <string>
#include "vectores.h"
using namespace std;


void tirarDados(int dados[], int cantidad)
{
    for (int i = 0; i < cantidad; i++)
    {
        dados[i] = (rand() % 6) + 1;
    }

}

void mostrarMensajeYEsperar(string mensaje)
{
    // Muestra el mensaje dentro de un recuadro (el recuadro esta hecho a medida del mensaje por defecto)
    cout << "╔════════════════════════════════════════════╗" << endl;
    cout << "║ " << mensaje << " ║" << endl;
    cout << "╚════════════════════════════════════════════╝" << endl;
    rlutil::msleep(1000);
    rlutil::anykey();
}

// Dibuja un recuadro de doble linea. (x1, y1) es la esquina de arriba a la izquierda y (x2, y2) la de abajo a la derecha
void dibujarRecuadro(int x1, int y1, int x2, int y2)
{
    // Linea de arriba
    rlutil::locate(x1, y1);
    cout << "╔";
    for (int i = x1 + 1; i < x2; i++)
    {
        cout << "═";
    }
    cout << "╗";

    // Paredes verticales
    for (int j = y1 + 1; j < y2; j++)
    {
        rlutil::locate(x1, j);
        cout << "║";
        rlutil::locate(x2, j);
        cout << "║";
    }

    // Linea de abajo
    rlutil::locate(x1, y2);
    cout << "╚";
    for (int i = x1 + 1; i < x2; i++)
    {
        cout << "═";
    }
    cout << "╝";
}

// Dibuja una linea horizontal que divide el recuadro en la fila indicada
void dibujarSeparador(int x1, int x2, int fila)
{
    rlutil::locate(x1, fila);
    cout << "╠";
    for (int i = x1 + 1; i < x2; i++)
    {
        cout << "═";
    }
    cout << "╣";
}

// Escribe "Presioná cualquier tecla..." en gris, centrado dentro del recuadro (columnas 15 a 103), y espera una tecla
void esperarTecla(int fila)
{
    rlutil::setColor(rlutil::DARKGREY);
    escribirCentrado("Presioná cualquier tecla para continuar", 59, fila);
    rlutil::msleep(500);
    rlutil::anykey();
    rlutil::setColor(rlutil::WHITE);
    rlutil::cls();
}

void jugarPartida(string jugadorInicial, string jugadorOponente, int dadosInicial[], int dadosOponente[], int& cantDadosInicial, int& cantDadosOponente, int& puntosInicial, int& puntosOponente, int numeroInicial)
{
    int numeroOponente = 3 - numeroInicial; // Si el que empieza es el jugador 1, el oponente es el 2 (3 - 1 = 2), y al revés (3 - 2 = 1)

    for (int ronda = 1; ronda <= 3; ronda++)   // Itera 3 rondas
    {
        mostrarEstadoDeRonda(ronda, jugadorInicial, jugadorOponente, puntosInicial, puntosOponente, cantDadosInicial, cantDadosOponente, numeroInicial);

        // Turno del jugador inicial. realizarTurno devuelve true si el jugador se quedó sin dados
        bool inicialSinDados = realizarTurno(jugadorInicial, jugadorOponente, dadosInicial, dadosOponente, cantDadosInicial, cantDadosOponente, puntosInicial, ronda, numeroInicial);

        // Si el que empieza se quedó sin dados, el oponente igual juega su turno de esta ronda,
        // así los dos jugaron la misma cantidad de turnos y tiene la chance de empatar
        if (inicialSinDados == true)
        {
            mostrarUltimaOportunidad(jugadorInicial, jugadorOponente, numeroInicial);
        }

        // Turno del jugador oponente
        bool oponenteSinDados = realizarTurno(jugadorOponente, jugadorInicial, dadosOponente, dadosInicial, cantDadosOponente, cantDadosInicial, puntosOponente, ronda, numeroOponente);

        // Si alguno de los dos se quedó sin dados, la partida termina al final de la ronda y vuelve al main.cpp
        // (si se quedaron los dos sin dados, se desempata por puntos en la pantalla final)
        if (inicialSinDados == true || oponenteSinDados == true)
        {
            return;
        }

        // Si nadie se quedó sin dados, como es un for "va" hacia la siguiente ronda y se repite lo mismo
    }
}

// Pantalla que avisa que el que empezó se quedó sin dados y que el rival tiene su último turno
void mostrarUltimaOportunidad(string jugadorSinDados, string rival, int numeroSinDados)
{
    int y = 10; // El recuadro mide 11 filas: (30 - 11) / 2 + 1 = 10, así queda centrado

    rlutil::cls();
    dibujarVentana(y, y + 10, "¡ÚLTIMA OPORTUNIDAD!", rlutil::YELLOW);

    rlutil::setColor(colorJugador(numeroSinDados));
    escribirCentrado("¡" + jugadorSinDados + " se quedó sin dados y sumó 10000 puntos!", 59, y + 2);
    rlutil::setColor(rlutil::YELLOW);
    dibujarSeparador(15, 103, y + 4);

    rlutil::setColor(colorJugador(3 - numeroSinDados));
    escribirCentrado("Ahora " + rival + " juega su último turno.", 59, y + 5);
    rlutil::setColor(rlutil::WHITE);
    escribirCentrado("Si también se queda sin dados, se desempata por puntos.", 59, y + 6);

    rlutil::setColor(rlutil::YELLOW);
    dibujarSeparador(15, 103, y + 8);
    esperarTecla(y + 9);
}




bool realizarTurno(string jugadorActual, string oponente, int dadosActual[], int dadosOponente[], int& cantDadosActual, int& cantDadosOponente, int& puntosActual, int ronda, int numeroActual)
{
    int color = colorJugador(numeroActual); // Rojo si juega el jugador 1, celeste si juega el jugador 2
    int colorOponente = colorJugador(3 - numeroActual);

    /// ---------- TIRADA DE LOS DADOS DE 12 CARAS ----------
    rlutil::cls();
    dibujarMarcoTurno(ronda, jugadorActual, numeroActual, puntosActual, cantDadosActual, oponente, cantDadosOponente);

    rlutil::setColor(rlutil::WHITE);
    escribirCentrado("Tirando los dos dados de 12 caras...", 59, 28);
    rlutil::msleep(1500);

    int dado1 = rand() % 12 + 1; // Crea un numero entre el 1 y el 12.
    rlutil::setColor(color);
    dibujarDadoDoce(dado1, 43, 6);
    rlutil::msleep(1000);

    int dado2 = rand() % 12 + 1;
    rlutil::setColor(color);
    dibujarDadoDoce(dado2, 55, 6);
    rlutil::msleep(1000);

    int numeroObjetivo = dado1 + dado2;
    dibujarObjetivo(dado1, dado2, numeroObjetivo, color);

    limpiarFila(28);
    rlutil::setColor(rlutil::YELLOW);
    escribirCentrado("¡El número objetivo es " + to_string(numeroObjetivo) + "!", 59, 28);
    rlutil::msleep(2000);

    /// ---------- TIRADA DE LOS DADOS DE 6 CARAS ----------
    limpiarFila(28);
    rlutil::setColor(rlutil::WHITE);
    escribirCentrado("Tirando tus " + textoDados(cantDadosActual) + " de 6 caras...", 59, 28);
    rlutil::msleep(1500);

    tirarDados(dadosActual, cantDadosActual); // tira los dados

    int dadosElegidos[70] = {}; // e.g al principio esto va a ser dadosElegidos[0] = 0 ya que no elegimos nada, pero va cambiando en el turno
    int sumaParcial = 0;
    int cantDadosElegidos = 0;
    int cantDadosACombinar = 0; // Cuántos dados dijo el jugador que va a usar
    bool usado[70] = {false}; // Se ponen todos los dados en false. si se usa uno, se cambia a true
    bool pasoDeTurno = false; // si ingresa 0, se cambia a true y termina el turno sin llegar al objetivo
    bool tePasaste = false; // si la suma supera al objetivo, se cambia a true y termina el turno
    bool llegoAntes = false; // si llega al objetivo antes de usar todos los dados que dijo, se cambia a true

    dibujarDados(dadosActual, cantDadosActual, usado, 15, color, true); // muestra los dados dibujados
    rlutil::msleep(1500);

    /// ---------- PASO 1: ELEGIR CUÁNTOS DADOS VA A COMBINAR ----------
    while (cantDadosACombinar == 0 && pasoDeTurno == false)
    {
        rlutil::cls();
        dibujarMarcoTurno(ronda, jugadorActual, numeroActual, puntosActual, cantDadosActual, oponente, cantDadosOponente);
        dibujarObjetivo(dado1, dado2, numeroObjetivo, color);

        rlutil::setColor(rlutil::WHITE);
        escribirCentrado("PASO 1 DE 2:  ¿CON CUÁNTOS DADOS VAS A SUMAR EXACTAMENTE " + to_string(numeroObjetivo) + "?", 59, 13);
        rlutil::setColor(rlutil::YELLOW);
        escribirCentrado("Si acertás: ganás " + to_string(numeroObjetivo) + " puntos por cada dado usado y se los pasás a tu rival.", 59, 14);

        dibujarDados(dadosActual, cantDadosActual, usado, 15, color, true);

        rlutil::setColor(rlutil::WHITE);
        escribirCentrado("Ingresá cuántos dados vas a usar (del 1 al " + to_string(cantDadosActual) + ")   (0 = no puedo llegar al objetivo)", 59, 28);
        rlutil::setColor(color);
        rlutil::locate(57, 29);
        cout << "> ";

        int cantidad;
        cin >> cantidad;

        rlutil::setColor(rlutil::YELLOW);
        if (cin.fail())    // Si se ingresa un tipo de dato incorrecto al preestablecido...
        {
            limpiarFila(29);
            escribirCentrado("Entrada inválida. Tiene que ser un número.", 59, 29);
            cin.clear(); // Borra el error de buffer, pero todavia queda lo que se escribió, para eso...
            cin.ignore(); // Descarta todo lo que escribimos.
            rlutil::msleep(1500);
        }
        else if (cantidad == 0)
        {
            pasoDeTurno = true;
        }
        else if (cantidad < 0 || cantidad > cantDadosActual)
        {
            limpiarFila(29);
            escribirCentrado("Tenés " + textoDados(cantDadosActual) + ". Elegí una cantidad del 1 al " + to_string(cantDadosActual) + ".", 59, 29);
            rlutil::msleep(1500);
        }
        else
        {
            cantDadosACombinar = cantidad;
        }
    }

    /// ---------- PASO 2: ELEGIR CUÁLES DADOS ----------
    // Se eligen exactamente la cantidad de dados que dijo en el paso 1
    while (pasoDeTurno == false && cantDadosElegidos < cantDadosACombinar)
    {
        rlutil::cls();
        dibujarMarcoTurno(ronda, jugadorActual, numeroActual, puntosActual, cantDadosActual, oponente, cantDadosOponente);
        dibujarObjetivo(dado1, dado2, numeroObjetivo, color);

        // En qué dado va, cuánto lleva sumado y cuánto le falta
        rlutil::setColor(rlutil::WHITE);
        escribirCentrado("PASO 2 DE 2:  ELEGÍ EL DADO " + to_string(cantDadosElegidos + 1) + " DE " + to_string(cantDadosACombinar) + "   |   Suma actual: " + to_string(sumaParcial) + "   |   Te faltan: " + to_string(numeroObjetivo - sumaParcial), 59, 13);
        rlutil::setColor(rlutil::YELLOW);
        escribirCentrado("Llegá a " + to_string(numeroObjetivo) + " con exactamente " + textoDados(cantDadosACombinar) + "   |   Si acertás: " + to_string(numeroObjetivo) + " x " + to_string(cantDadosACombinar) + " = " + to_string(numeroObjetivo * cantDadosACombinar) + " puntos", 59, 14);

        dibujarDados(dadosActual, cantDadosActual, usado, 15, color, true); // Los dados ya usados se ven en gris

        rlutil::setColor(rlutil::WHITE);
        escribirCentrado("Ingresá el número que está abajo del dado que querés sumar   (0 = no puedo llegar)", 59, 28);
        rlutil::setColor(color);
        rlutil::locate(57, 29);
        cout << "> ";

        int ind; // Indice del dado
        cin >> ind;

        rlutil::setColor(rlutil::YELLOW);
        if (cin.fail())    // Si se ingresa un tipo de dato incorrecto al preestablecido...
        {
            limpiarFila(29);
            escribirCentrado("Entrada inválida. Tiene que ser el número que está abajo del dado.", 59, 29);
            cin.clear(); // Borra el error de buffer, pero todavia queda lo que se escribió, para eso...
            cin.ignore(); // Descarta todo lo que escribimos.
            rlutil::msleep(1500);
            continue; // continue significa continuar hacia el final del while, y repetir el while
        }

        if (ind == 0)   // Si ingresa 0, termina el turno sin llegar al objetivo
        {
            pasoDeTurno = true;
            break;
        }

        ind--; // Ajustar índice para el array. Como los arrays son de 0 a 6 (e.g), al elegir ind 2 estariamos eligiendo el espacio 3 del array y no el 2

        if (ind < 0 or ind >= cantDadosActual)   // si el indice es menor, o si es mayor o igual que el ultimo valor que guardamos (inutilizado por arrays)
        {
            limpiarFila(29);
            escribirCentrado("Ese dado no existe. Elegí un número del 1 al " + to_string(cantDadosActual) + ".", 59, 29);
            rlutil::msleep(1500);
            continue;
        }
        if (usado[ind] == true)   // Si usamos el dado..
        {
            limpiarFila(29);
            escribirCentrado("Ese dado ya lo sumaste (está en gris). Elegí otro.", 59, 29);
            rlutil::msleep(1500);
            continue ;
        }

        dadosElegidos[cantDadosElegidos] = dadosActual[ind]; // Guardamos los dados y su valor que elegimos para posteriormente mostrarlos y/o dárselos al rival
        sumaParcial += dadosActual[ind]; // Guarda el valor del dado que elegimos en suma parcial
        usado[ind] = true; // Lo cambia si usamos
        cantDadosElegidos++; // Suma la cantidad, y tambien sirve para seguir con el proximo espacio del array

        limpiarFila(29);
        rlutil::setColor(color);
        if (sumaParcial > numeroObjetivo)   // Si la suma se pasó del objetivo, ya no se puede llegar
        {
            tePasaste = true;
            escribirCentrado("Sumaste un " + to_string(dadosActual[ind]) + " y te pasaste del objetivo.", 59, 29);
            rlutil::msleep(1200);
            break;
        }
        if (sumaParcial == numeroObjetivo && cantDadosElegidos < cantDadosACombinar)   // Llegó, pero todavía le faltan dados por sumar
        {
            llegoAntes = true;
            escribirCentrado("Llegaste a " + to_string(numeroObjetivo) + " pero todavía te falta sumar dados: te vas a pasar.", 59, 29);
            rlutil::msleep(1800);
            break;
        }
        escribirCentrado("Sumaste un " + to_string(dadosActual[ind]) + ".", 59, 29);
        rlutil::msleep(900);
    }

    /// ---------- RESULTADO DEL TURNO ----------
    // Llegó al objetivo solo si usó exactamente la cantidad de dados que dijo y la suma es igual al objetivo
    bool llegoAlObjetivo = false;
    if (pasoDeTurno == false && cantDadosElegidos == cantDadosACombinar && sumaParcial == numeroObjetivo)
    {
        llegoAlObjetivo = true;
    }

    // El alto del recuadro depende de cuántos renglones de dados elegidos hay que dibujar (entran 8 dados por renglón)
    bool ningunoUsado[70] = {false}; // Para dibujar los dados elegidos sin pintarlos de gris
    int filasDados = (cantDadosElegidos + 7) / 8; // Si no eligió ningún dado da 0
    int alto = 12 + filasDados * 6;
    int y = (30 - alto) / 2 + 1; // Fila donde empieza el recuadro, así queda centrado en la pantalla
    int t = y + 4 + filasDados * 6; // Fila donde empiezan los textos del resultado
    bool sinDados = false;

    rlutil::cls();
    dibujarVentana(y, y + alto - 1, "RESULTADO DEL TURNO", color);
    rlutil::setColor(color);
    escribirCentrado(jugadorActual + "  (JUGADOR " + to_string(numeroActual) + ")", 59, y + 1);
    dibujarSeparador(15, 103, y + 2);

    // Los dados que eligió y cuánto suman
    rlutil::setColor(rlutil::WHITE);
    if (cantDadosElegidos > 0)
    {
        dibujarDados(dadosElegidos, cantDadosElegidos, ningunoUsado, y + 4, color, false);
        rlutil::setColor(rlutil::WHITE);
        escribirCentrado("Suma de los dados elegidos: " + to_string(sumaParcial) + "   |   Objetivo: " + to_string(numeroObjetivo), 59, t);
    }
    else
    {
        escribirCentrado("No elegiste ningún dado.   |   Objetivo: " + to_string(numeroObjetivo), 59, t);
    }

    if (llegoAlObjetivo == true)
    {
        int puntosGanados = sumaParcial * cantDadosElegidos; // Calcula el puntaje
        puntosActual += puntosGanados;
        cantDadosActual -= cantDadosElegidos; // Restamos de nuestros dados los que elegimos

        rlutil::setColor(rlutil::LIGHTGREEN);
        escribirCentrado("¡LLEGASTE AL OBJETIVO!   Puntos: " + to_string(sumaParcial) + " (objetivo) x " + to_string(cantDadosElegidos) + " (dados usados) = +" + to_string(puntosGanados), 59, t + 1);

        rlutil::setColor(color);
        if (cantDadosOponente > 0)
        {
            cantDadosOponente += cantDadosElegidos; // Le suma al oponente los dados que usamos
            escribirCentrado("Le pasás a " + oponente + " los dados que usaste. Te quedan " + textoDados(cantDadosActual) + ".", 59, t + 2);
        }
        else   // Si el oponente ya se quedó sin dados (terminó la partida), no se le pasan
        {
            escribirCentrado(oponente + " ya no tiene dados, así que los que usaste quedan afuera. Te quedan " + textoDados(cantDadosActual) + ".", 59, t + 2);
        }

        if (cantDadosActual <= 0)   // Si no tenemos dados...
        {
            puntosActual += 10000;
            sinDados = true; // Indica que se quedó sin dados y la partida termina al final de la ronda
            rlutil::setColor(rlutil::LIGHTGREEN);
            escribirCentrado("¡" + jugadorActual + " se quedó sin dados!   +10000 puntos", 59, t + 3);
        }
    }
    else   // NO LLEGÓ AL OBJETIVO
    {
        // Por qué no llegó
        rlutil::setColor(rlutil::YELLOW);
        if (pasoDeTurno == true)
        {
            escribirCentrado("No llegaste al objetivo, así que no sumás puntos en este turno.", 59, t + 1);
        }
        else if (tePasaste == true)
        {
            escribirCentrado("¡Te pasaste del objetivo! No sumás puntos en este turno.", 59, t + 1);
        }
        else if (llegoAntes == true)
        {
            escribirCentrado("Llegaste a " + to_string(numeroObjetivo) + " con menos dados de los que elegiste usar. No sumás puntos.", 59, t + 1);
        }
        else
        {
            escribirCentrado("Con " + textoDados(cantDadosElegidos) + " no llegaste a " + to_string(numeroObjetivo) + ". No sumás puntos en este turno.", 59, t + 1);
        }

        // Penalización: el rival le pasa un dado. Como gana el que se queda sin dados,
        // recibir un dado te perjudica a vos y ayuda a tu rival.
        // Solo se lo pasa si le queda más de uno: si le pasara el último, el rival ganaría sin haber acertado.
        if (cantDadosOponente > 1)   // Si tiene mas de un dado el oponente
        {
            cantDadosActual++; // Le suma uno al actual
            cantDadosOponente--; // Y se lo resta dando la logica de que se lo da de su repositorio
            rlutil::setColor(rlutil::YELLOW);
            escribirCentrado("PENALIZACIÓN: " + oponente + " te pasa 1 de sus dados.", 59, t + 2);
            rlutil::setColor(rlutil::WHITE);
            escribirCentrado("Ahora tenés " + textoDados(cantDadosActual) + " y " + oponente + " tiene " + textoDados(cantDadosOponente) + ".", 59, t + 3);
        }
        else if (cantDadosOponente == 1)
        {
            rlutil::setColor(colorOponente);
            escribirCentrado("Sin penalización: " + oponente + " tiene 1 solo dado y no puede pasártelo", 59, t + 2);
            escribirCentrado("(si te lo pasara, se quedaría sin dados sin haber acertado).", 59, t + 3);
        }
        else
        {
            rlutil::setColor(colorOponente);
            escribirCentrado("Sin penalización: " + oponente + " ya no tiene dados para pasarte.", 59, t + 2);
        }
    }

    // Puntaje acumulado del jugador después de este turno
    rlutil::setColor(rlutil::WHITE);
    escribirCentrado("Puntaje total de " + jugadorActual + ": " + to_string(puntosActual) + " puntos", 59, t + 4);

    rlutil::setColor(color);
    dibujarSeparador(15, 103, t + 5);
    esperarTecla(t + 6);

    return sinDados; // true: se quedó sin dados y la partida termina al final de la ronda. false: sigue el juego
}

// Devuelve la cantidad de dados escrita bien: "1 dado" o "3 dados"
string textoDados(int cantidad)
{
    if (cantidad == 1)
    {
        return "1 dado";
    }
    return to_string(cantidad) + " dados";
}






// Funcion que crea y muestra las estadisticas del juego.

void estadisticasDelJuego(string jugador1, string jugador2, int puntos1, int puntos2, int dados1, int dados2, string nombreMejorHistorico, int puntosMejorHistorico, bool huboPartidaJugada)
{
    rlutil::cls();

    // "y" es la fila donde empieza el recuadro. Todo lo demas se ubica a partir de ella,
    // asi el recuadro queda centrado en el medio de la pantalla (que tiene 30 filas)
    int y;

    if (huboPartidaJugada == false)
    {
        y = 9; // El recuadro mide 13 filas: (30 - 13) / 2 + 1 = 9

        dibujarVentana(y, y + 12, "ESTADÍSTICAS", rlutil::YELLOW);

        // Un dado "vacío" de adorno
        rlutil::setColor(rlutil::DARKGREY);
        dibujarRecuadroSimple(55, y + 2, 63, y + 6);
        escribirCentrado("?", 59, y + 4);

        rlutil::setColor(rlutil::WHITE);
        escribirCentrado("Todavía no se jugó ninguna partida.", 59, y + 8);
        rlutil::setColor(rlutil::DARKGREY);
        escribirCentrado("Jugá una desde el menú (opción 1) para ver acá los resultados.", 59, y + 9);

        rlutil::setColor(rlutil::YELLOW);
        dibujarSeparador(15, 103, y + 10);
        esperarTecla(y + 11);
    }
    else
    {
        y = 7; // El recuadro mide 17 filas: (30 - 17) / 2 + 1 = 7

        dibujarVentana(y, y + 16, "ESTADÍSTICAS", rlutil::YELLOW);

        rlutil::setColor(rlutil::YELLOW);
        escribirCentrado("ÚLTIMA PARTIDA", 59, y + 1);
        dibujarSeparador(15, 103, y + 2);

        // Tarjetas de los dos jugadores; la del ganador dice GANADOR abajo
        dibujarTarjetaJugador(jugador1, 1, puntos1, dados1, y + 3, puntos1 > puntos2);
        dibujarTarjetaJugador(jugador2, 2, puntos2, dados2, y + 3, puntos2 > puntos1);
        rlutil::setColor(rlutil::WHITE);
        escribirCentrado("VS", 59, y + 5);

        // Condicional para validar qué jugador ganó o si hay un empate
        if ( puntos1 > puntos2 )
        {
            rlutil::setColor(colorJugador(1));
            escribirCentrado("Ganó " + jugador1 + " por " + to_string(puntos1 - puntos2) + " puntos de diferencia.", 59, y + 10);
        }
        else if ( puntos2 > puntos1 )   // Si los puntos del jugador 2 son mayores
        {
            rlutil::setColor(colorJugador(2));
            escribirCentrado("Ganó " + jugador2 + " por " + to_string(puntos2 - puntos1) + " puntos de diferencia.", 59, y + 10);
        }
        else     // Si los puntos son iguales
        {
            rlutil::setColor(rlutil::YELLOW);
            escribirCentrado("La partida terminó en empate.", 59, y + 10);
        }

        rlutil::setColor(rlutil::YELLOW);
        dibujarSeparador(15, 103, y + 11);
        escribirCentrado("RÉCORD DE LA SESIÓN", 59, y + 12);

        rlutil::setColor(rlutil::WHITE);
        if (puntosMejorHistorico > 0)   // Solo si hay un record > 0
        {
            escribirCentrado(nombreMejorHistorico + "  ·  " + to_string(puntosMejorHistorico) + " puntos", 59, y + 13);
        }
        else
        {
            escribirCentrado("Todavía no hay un récord en esta sesión.", 59, y + 13);
        }

        rlutil::setColor(rlutil::YELLOW);
        dibujarSeparador(15, 103, y + 14);
        esperarTecla(y + 15);
    }
}



int decidirQuienEmpieza(string nombre1, string nombre2)
{
    int dado_j1, dado_j2;
    int y = 8; // El recuadro mide 16 filas: (30 - 16) / 2 + 1 = 8, así queda centrado

    do
    {
        rlutil::cls();
        dibujarVentana(y, y + 15, "¿QUIÉN EMPIEZA?", rlutil::YELLOW);

        rlutil::setColor(rlutil::WHITE);
        escribirCentrado("Cada jugador tira un dado de 6 caras. Empieza el que saque el número más alto.", 59, y + 1);
        rlutil::setColor(rlutil::YELLOW);
        dibujarSeparador(15, 103, y + 2);

        // Nombres de cada jugador con su color, uno a cada lado
        rlutil::setColor(colorJugador(1));
        escribirCentrado("JUGADOR 1", 37, y + 5);
        escribirCentrado(nombre1, 37, y + 6);
        rlutil::setColor(colorJugador(2));
        escribirCentrado("JUGADOR 2", 81, y + 5);
        escribirCentrado(nombre2, 81, y + 6);
        rlutil::setColor(rlutil::WHITE);
        escribirCentrado("VS", 59, y + 9);
        rlutil::msleep(1500);

        // Cada jugador tira UN solo dado de 6 caras
        dado_j1 = (rand() % 6) + 1;
        rlutil::setColor(colorJugador(1));
        dibujarDado(dado_j1, 33, y + 7);
        rlutil::msleep(1500);

        dado_j2 = (rand() % 6) + 1;
        rlutil::setColor(colorJugador(2));
        dibujarDado(dado_j2, 77, y + 7);
        rlutil::msleep(1000);

        if (dado_j1 == dado_j2)
        {
            rlutil::setColor(rlutil::YELLOW);
            escribirCentrado("¡Empate! Se vuelve a tirar...", 59, y + 12);
            rlutil::msleep(2000);
        }

    }
    while (dado_j1 == dado_j2);   // El bucle se repite si los dados son iguales

    int quienEmpieza;
    if (dado_j1 > dado_j2)
    {
        quienEmpieza = 0; // Gana el jugador 1
        rlutil::setColor(colorJugador(1));
        escribirCentrado("¡" + nombre1 + " empieza el juego!", 59, y + 12);
    }
    else
    {
        quienEmpieza = 1; // Gana el jugador 2
        rlutil::setColor(colorJugador(2));
        escribirCentrado("¡" + nombre2 + " empieza el juego!", 59, y + 12);
    }

    rlutil::setColor(rlutil::YELLOW);
    dibujarSeparador(15, 103, y + 13);
    esperarTecla(y + 14);
    return quienEmpieza;
}

int menuOpciones()
{
    // Funcion que muestra el menu principal dl juego.
    int opciones;
    bool opcionValida = false;

    int y = 3; // El recuadro mide 25 filas: (30 - 25) / 2 + 1 = 3, así queda centrado

    do
    {
        rlutil::cls();

        // Ventana con dos divisiones: logo arriba, opciones en el medio y la entrada abajo
        dibujarVentana(y, y + 24, "MENÚ PRINCIPAL", rlutil::YELLOW);
        rlutil::setColor(rlutil::YELLOW);
        dibujarSeparador(15, 103, y + 9);
        dibujarSeparador(15, 103, y + 21);

        // Logo ENFRENDADOS con un dado a cada lado
        dibujarLogo(y + 2);

        rlutil::setColor(rlutil::DARKGREY);
        escribirCentrado("Un juego de dados y estrategia para dos jugadores", 59, y + 8);

        // Opciones: el número en amarillo, el nombre en blanco y una descripción en gris
        dibujarOpcionMenu("1", "JUGAR", "Empezar una partida nueva", y + 11);
        dibujarOpcionMenu("2", "ESTADÍSTICAS", "Última partida y récord de la sesión", y + 13);
        dibujarOpcionMenu("3", "CRÉDITOS", "Quién hizo el juego", y + 15);
        dibujarOpcionMenu("4", "REGLAMENTO", "Cómo se juega y la gracia del juego", y + 17);
        dibujarOpcionMenu("0", "SALIR", "Cerrar el juego", y + 19);

        rlutil::setColor(rlutil::WHITE);
        rlutil::locate(38, y + 22);
        cout << "Elegí una opción y presioná ENTER:  ";
        rlutil::setColor(rlutil::YELLOW);
        cout << "> ";

        cin >> opciones;

        rlutil::setColor(rlutil::YELLOW);
        if (cin.fail())
        {
            escribirCentrado("Entrada inválida. Ingresá un número del 0 al 4.", 59, y + 23);
            cin.clear();
            cin.ignore(); // Limpia el buffer de entrada para que no afecte a la siguiente entrada
            rlutil::msleep(1500);
        }
        else if (opciones >= 0 && opciones <= 4)
        {
            opcionValida = true;
            rlutil::cls();
        }
        else
        {
            escribirCentrado("La opción " + to_string(opciones) + " no existe. Ingresá un número del 0 al 4.", 59, y + 23);
            rlutil::msleep(1500);
        }
    }
    while (!opcionValida);

    return opciones;
}

void reglamento()
{
    /// ---------- PÁGINA 1: LA IDEA DEL JUEGO (por qué los dados van y vienen) ----------
    rlutil::cls();

    // El texto arranca en la columna 21 para que los renglones largos queden centrados en el recuadro
    dibujarVentana(1, 30, "REGLAMENTO", rlutil::YELLOW);
    rlutil::setColor(rlutil::YELLOW);
    escribirCentrado("Página 1 de 2  ·  La idea del juego", 59, 2);
    dibujarSeparador(15, 103, 3);

    rlutil::setColor(rlutil::WHITE);
    rlutil::locate(21, 5);
    cout << "Tus dados son a la vez tu herramienta y tu carga:";
    rlutil::locate(21, 6);
    cout << "- Con más dados es más fácil armar el número objetivo y sumar puntos.";
    rlutil::locate(21, 7);
    cout << "- Pero para ganar tenés que QUEDARTE SIN DADOS: eso da 10000 puntos,";
    rlutil::locate(21, 8);
    cout << "   muchísimo más de lo que se junta acertando.";

    rlutil::setColor(rlutil::YELLOW);
    rlutil::locate(21, 10);
    cout << "¿POR QUÉ LOS DADOS VAN Y VIENEN?";

    rlutil::setColor(rlutil::WHITE);
    rlutil::locate(21, 11);
    cout << "- Si acertás, le pasás a tu rival los dados que usaste: vos te acercás a";
    rlutil::locate(21, 12);
    cout << "   ganar y tu rival queda más cargado.";
    rlutil::locate(21, 13);
    cout << "- Si fallás, tu rival te pasa 1 de sus dados como castigo: vos te alejás";
    rlutil::locate(21, 14);
    cout << "   de ganar y tu rival se acerca.";

    // Ejemplo con los dados de cada jugador dibujados como cuadraditos
    rlutil::setColor(rlutil::YELLOW);
    rlutil::locate(21, 16);
    cout << "EJEMPLO:";

    rlutil::setColor(rlutil::WHITE);
    rlutil::locate(21, 17);
    cout << "Los dos empiezan con 6 dados:";
    dibujarBarraDados(1, 6, 24, 18);
    dibujarBarraDados(2, 6, 62, 18);

    rlutil::setColor(rlutil::WHITE);
    rlutil::locate(21, 19);
    cout << "El jugador 1 acierta usando 3 dados y se los pasa al jugador 2:";
    dibujarBarraDados(1, 3, 24, 20);
    dibujarBarraDados(2, 9, 62, 20);

    rlutil::setColor(rlutil::WHITE);
    rlutil::locate(21, 21);
    cout << "El jugador 2 falla, así que el jugador 1 le pasa 1 de sus dados como castigo:";
    dibujarBarraDados(1, 2, 24, 22);
    dibujarBarraDados(2, 10, 62, 22);

    rlutil::setColor(rlutil::LIGHTGREEN);
    rlutil::locate(21, 24);
    cout << "LA ESTRATEGIA:";

    rlutil::setColor(rlutil::WHITE);
    rlutil::locate(21, 25);
    cout << "- Cuantos más dados uses en un acierto, más puntos sumás (objetivo x dados)";
    rlutil::locate(21, 26);
    cout << "   y más dados te sacás de encima. ¡Animate a combinar muchos dados!";
    rlutil::locate(21, 27);
    cout << "- Si nadie se queda sin dados en 3 rondas, gana el que tenga más puntos.";

    rlutil::setColor(rlutil::YELLOW);
    dibujarSeparador(15, 103, 28);
    esperarTecla(29);

    /// ---------- PÁGINA 2: LAS REGLAS ----------
    dibujarVentana(1, 30, "REGLAMENTO", rlutil::YELLOW);
    rlutil::setColor(rlutil::YELLOW);
    escribirCentrado("Página 2 de 2  ·  Las reglas", 59, 2);
    dibujarSeparador(15, 103, 3);

    rlutil::setColor(rlutil::WHITE);
    rlutil::locate(21, 4);
    cout << "Enfrendados es un juego de dados para dos jugadores que se juega en 3 rondas.";
    rlutil::locate(21, 5);
    cout << "El objetivo es QUEDARSE SIN DADOS y sumar la mayor cantidad de puntos.";

    rlutil::setColor(rlutil::YELLOW);
    rlutil::locate(21, 7);
    cout << "AL EMPEZAR:";

    rlutil::setColor(rlutil::WHITE);
    rlutil::locate(21, 8);
    cout << "- Cada jugador tiene 6 dados de 6 caras.";
    rlutil::locate(21, 9);
    cout << "- Cada uno tira un dado y empieza el que saque más (si empatan, se repite).";

    rlutil::setColor(rlutil::YELLOW);
    rlutil::locate(21, 11);
    cout << "EN CADA TURNO:";

    rlutil::setColor(rlutil::WHITE);
    rlutil::locate(21, 12);
    cout << "- Se tiran 2 dados de 12 caras. Su suma es el NÚMERO OBJETIVO.";
    rlutil::locate(21, 13);
    cout << "- Después tirás tus dados de 6 caras y decidís CUÁNTOS vas a combinar.";
    rlutil::locate(21, 14);
    cout << "- Elegís CUÁLES: tienen que sumar EXACTAMENTE el número objetivo.";
    rlutil::locate(21, 15);
    cout << "- Si ves que no podés llegar, ingresá 0.";

    rlutil::setColor(rlutil::LIGHTGREEN);
    rlutil::locate(21, 17);
    cout << "SI LLEGÁS AL OBJETIVO:";

    rlutil::setColor(rlutil::WHITE);
    rlutil::locate(21, 18);
    cout << "- Ganás puntos: NÚMERO OBJETIVO x CANTIDAD DE DADOS que usaste.";
    rlutil::locate(21, 19);
    cout << "   Ejemplo: 3 dados que suman 12 dan 12 x 3 = 36 puntos.";
    rlutil::locate(21, 20);
    cout << "- Le pasás a tu rival los dados que usaste, así te quedan menos.";

    rlutil::setColor(rlutil::YELLOW);
    rlutil::locate(21, 22);
    cout << "SI NO LLEGÁS (O TE PASÁS):";

    rlutil::setColor(rlutil::WHITE);
    rlutil::locate(21, 23);
    cout << "- Tu rival te pasa 1 de sus dados como penalización (si tiene más de 1).";

    rlutil::setColor(rlutil::LIGHTGREEN);
    rlutil::locate(21, 25);
    cout << "CÓMO SE GANA:";

    rlutil::setColor(rlutil::WHITE);
    rlutil::locate(21, 26);
    cout << "- Si te quedás sin dados sumás 10000 puntos. Tu rival igual juega su turno.";
    rlutil::locate(21, 27);
    cout << "- Al final gana el de más puntos (si los dos se quedan sin dados, desempatan así).";

    rlutil::setColor(rlutil::YELLOW);
    dibujarSeparador(15, 103, 28);
    esperarTecla(29);
}

void creditos()
{
    // Funcion que muestra los creditos del juego.
    rlutil::cls();

    // "y" es la fila donde empieza el recuadro. El recuadro mide 22 filas: (30 - 22) / 2 + 1 = 5
    // asi queda centrado en el medio de la pantalla (que tiene 30 filas)
    int y = 5;

    dibujarVentana(y, y + 21, "CRÉDITOS", rlutil::YELLOW);
    dibujarLogo(y + 2);
    rlutil::setColor(rlutil::DARKGREY);
    escribirCentrado("Un juego de dados y estrategia para dos jugadores", 59, y + 8);
    rlutil::setColor(rlutil::YELLOW);
    dibujarSeparador(15, 103, y + 9);

    // Integrante y grupo
    rlutil::setColor(rlutil::YELLOW);
    escribirCentrado("DESARROLLO", 59, y + 10);
    rlutil::setColor(rlutil::WHITE);
    escribirCentrado("Federico Wachenschwan", 59, y + 11);
    rlutil::setColor(rlutil::DARKGREY);
    escribirCentrado("Grupo 2", 59, y + 12);

    // Menciones especiales
    rlutil::setColor(rlutil::YELLOW);
    escribirCentrado("IDEA ORIGINAL", 59, y + 14);
    rlutil::setColor(rlutil::WHITE);
    escribirCentrado("Juego inventado por Angel Simón. Levemente inspirado en el juego Mafia.", 59, y + 15);

    rlutil::setColor(rlutil::YELLOW);
    escribirCentrado("RECURSOS", 59, y + 17);
    rlutil::setColor(rlutil::WHITE);
    escribirCentrado("Íconos obtenidos de Freepik y logo hecho en Logo Maker.", 59, y + 18);

    rlutil::setColor(rlutil::YELLOW);
    dibujarSeparador(15, 103, y + 19);
    esperarTecla(y + 20);
}


void mostrarPortada()
{
    int y = 7; // El recuadro mide 17 filas: (30 - 17) / 2 + 1 = 7, así queda centrado

    rlutil::hidecursor(); //Hace que no titile el cursor mientras se muestra la portada
    rlutil::cls();

    dibujarVentana(y, y + 16, "", rlutil::YELLOW);
    dibujarLogo(y + 3);
    rlutil::setColor(rlutil::DARKGREY);
    escribirCentrado("Un juego de dados y estrategia para dos jugadores", 59, y + 10);

    // El mensaje de abajo "parpadea": se alterna entre blanco y gris hasta que se presione una tecla
    bool encendido = true;
    while (true)
    {
        if (encendido == true)
        {
            rlutil::setColor(rlutil::WHITE);
        }
        else
        {
            rlutil::setColor(rlutil::DARKGREY);
        }
        escribirCentrado("Presioná cualquier tecla para comenzar", 59, y + 13);
        encendido = !encendido;

        rlutil::msleep(600); //Detiene la ejecucion del programa un determinado tiempo

        // Si se presionó una tecla, salir del bucle
        if (kbhit())   //Determina si se ha pulsado el teclado
        {
            getch(); // Consume la tecla
            rlutil::setColor(rlutil::WHITE);
            rlutil::showcursor(); // Vuelve a mostrar el cursor para cuando haya que escribir
            rlutil::cls(); //Limpia la pantalla
            return;
        }
    }
}

void mostrarEstadoDeRonda(int ronda, string jugador1, string jugador2, int puntos1, int puntos2, int dados1, int dados2, int numeroInicial)
{
    // jugador1 es el que empieza (su número es numeroInicial) y jugador2 es el oponente
    int y = 8; // El recuadro mide 15 filas: (30 - 15) / 2 + 1 = 8, así queda centrado

    rlutil::cls();
    dibujarVentana(y, y + 14, "RONDA " + to_string(ronda) + " DE 3", rlutil::YELLOW);

    // Progreso de la partida: un círculo lleno por cada ronda que ya empezó
    string progreso = "";
    for (int i = 1; i <= 3; i++)
    {
        if (i <= ronda)
        {
            progreso = progreso + "● ";
        }
        else
        {
            progreso = progreso + "○ ";
        }
    }
    rlutil::setColor(rlutil::YELLOW);
    escribirCentrado(progreso, 59, y + 1);
    dibujarSeparador(15, 103, y + 2);

    // Cada jugador va de su lado: el jugador 1 a la izquierda y el jugador 2 a la derecha
    dibujarTarjetaJugador(jugador1, numeroInicial, puntos1, dados1, y + 4, false);
    dibujarTarjetaJugador(jugador2, 3 - numeroInicial, puntos2, dados2, y + 4, false);
    rlutil::setColor(rlutil::WHITE);
    escribirCentrado("VS", 59, y + 6);

    rlutil::setColor(colorJugador(numeroInicial));
    escribirCentrado("Primer turno de la ronda: " + jugador1, 59, y + 11);

    rlutil::setColor(rlutil::YELLOW);
    dibujarSeparador(15, 103, y + 12);
    esperarTecla(y + 13);
}

void mostrarFinDePartida(string nombre1, string nombre2, int puntos1, int puntos2, int dados1, int dados2, bool nuevoRecord)
{
    int y = 8; // El recuadro mide 16 filas: (30 - 16) / 2 + 1 = 8, así queda centrado

    rlutil::cls();
    dibujarVentana(y, y + 15, "FIN DE LA PARTIDA", rlutil::YELLOW);

    // Cómo terminó la partida
    rlutil::setColor(rlutil::WHITE);
    if (dados1 == 0 && dados2 == 0)
    {
        escribirCentrado("¡Los dos se quedaron sin dados! Se desempata por puntos.", 59, y + 1);
    }
    else if (dados1 == 0)
    {
        rlutil::setColor(colorJugador(1));
        escribirCentrado(nombre1 + " se quedó sin dados y sumó 10000 puntos.", 59, y + 1);
    }
    else if (dados2 == 0)
    {
        rlutil::setColor(colorJugador(2));
        escribirCentrado(nombre2 + " se quedó sin dados y sumó 10000 puntos.", 59, y + 1);
    }
    else
    {
        escribirCentrado("Se jugaron las 3 rondas y nadie se quedó sin dados. Gana el de más puntos.", 59, y + 1);
    }

    rlutil::setColor(rlutil::YELLOW);
    dibujarSeparador(15, 103, y + 2);

    // Tarjetas de los dos jugadores; la del ganador dice GANADOR abajo
    dibujarTarjetaJugador(nombre1, 1, puntos1, dados1, y + 4, puntos1 > puntos2);
    dibujarTarjetaJugador(nombre2, 2, puntos2, dados2, y + 4, puntos2 > puntos1);
    rlutil::setColor(rlutil::WHITE);
    escribirCentrado("VS", 59, y + 6);

    // El ganador y por cuántos puntos
    if (puntos1 > puntos2)
    {
        rlutil::setColor(colorJugador(1));
        escribirCentrado("¡GANÓ " + nombre1 + " por " + to_string(puntos1 - puntos2) + " puntos de diferencia!", 59, y + 11);
    }
    else if (puntos2 > puntos1)
    {
        rlutil::setColor(colorJugador(2));
        escribirCentrado("¡GANÓ " + nombre2 + " por " + to_string(puntos2 - puntos1) + " puntos de diferencia!", 59, y + 11);
    }
    else     // si ni j1 ni j2 tienen mas puntaje que el otro, quiere decir empate
    {
        rlutil::setColor(rlutil::YELLOW);
        escribirCentrado("¡EMPATE! Los dos terminaron con " + to_string(puntos1) + " puntos.", 59, y + 11);
    }

    if (nuevoRecord == true)
    {
        rlutil::setColor(rlutil::LIGHTGREEN);
        escribirCentrado("¡NUEVO RÉCORD DE LA SESIÓN!", 59, y + 12);
    }

    rlutil::setColor(rlutil::YELLOW);
    dibujarSeparador(15, 103, y + 13);
    esperarTecla(y + 14);
}

void pedirNombres(string& nombre1, string& nombre2)
{
    int y = 10; // El recuadro mide 12 filas: (30 - 12) / 2 + 1 = 10, así queda centrado

    rlutil::cls();
    dibujarVentana(y, y + 11, "NUEVA PARTIDA", rlutil::YELLOW);

    rlutil::setColor(rlutil::WHITE);
    escribirCentrado("Ingresen sus nombres para comenzar", 59, y + 1);
    rlutil::setColor(rlutil::YELLOW);
    dibujarSeparador(15, 103, y + 2);

    rlutil::setColor(rlutil::DARKGREY);
    escribirCentrado("Hasta 15 letras. No pueden estar vacíos ni ser iguales.", 59, y + 3);

    // Cada jugador escribe su nombre con su color. Se vuelve a pedir hasta que sea válido
    nombre1 = pedirNombreValido(1, "", y + 5);
    nombre2 = pedirNombreValido(2, nombre1, y + 8);

    rlutil::setColor(rlutil::WHITE);
}

// Pide el nombre de un jugador en la fila indicada hasta que sea válido:
// que no esté vacío (ni sea solo espacios), que tenga 15 letras como máximo y que no sea igual al del otro jugador.
// Si no es válido, muestra el error en la fila de abajo y lo vuelve a pedir.
string pedirNombreValido(int numero, string nombreOtro, int fila)
{
    string nombre;

    while (true)
    {
        limpiarFila(fila);
        rlutil::setColor(colorJugador(numero));
        rlutil::locate(41, fila);
        cout << "Nombre del JUGADOR " << numero << ": ";
        getline(cin, nombre);

        // Cuenta cuántos caracteres no son espacios
        int cantLugares = nombre.length();
        int letras = 0;
        for (int i = 0; i < cantLugares; i++)
        {
            if (nombre[i] != ' ')
            {
                letras++;
            }
        }

        limpiarFila(fila + 1);
        rlutil::setColor(rlutil::YELLOW);
        if (letras == 0)
        {
            escribirCentrado("El nombre no puede estar vacío. Escribí al menos una letra.", 59, fila + 1);
        }
        else if (largoTexto(nombre) > 15)
        {
            escribirCentrado("El nombre es muy largo: puede tener hasta 15 letras.", 59, fila + 1);
        }
        else if (nombre == nombreOtro)
        {
            escribirCentrado("Ese nombre ya lo eligió el jugador 1. Elegí uno distinto.", 59, fila + 1);
        }
        else
        {
            // Nombre válido: muestra una confirmación en la fila de abajo
            rlutil::setColor(rlutil::LIGHTGREEN);
            escribirCentrado("¡Listo, " + nombre + "!", 59, fila + 1);
            return nombre;
        }
    }
}

bool confirmarSalida()
{
    char opcionSalir; // caracter
    int y = 12; // El recuadro mide 8 filas: (30 - 8) / 2 + 1 = 12, así queda centrado

    while (true)
    {
        rlutil::cls();
        dibujarVentana(y, y + 7, "SALIR DEL JUEGO", rlutil::YELLOW);

        rlutil::setColor(rlutil::WHITE);
        escribirCentrado("¿Seguro que querés salir del juego?", 59, y + 2);
        rlutil::setColor(rlutil::DARKGREY);
        escribirCentrado("Escribí S para salir o N para volver al menú", 59, y + 3);
        rlutil::setColor(rlutil::YELLOW);
        rlutil::locate(57, y + 5);
        cout << "> ";
        rlutil::setColor(rlutil::WHITE);
        cin >> opcionSalir;

        if (opcionSalir == 's' || opcionSalir == 'S')
        {
            // Pantalla de despedida con el logo
            rlutil::cls();
            dibujarVentana(9, 21, "", rlutil::YELLOW);
            dibujarLogo(11);
            rlutil::setColor(rlutil::LIGHTGREEN);
            escribirCentrado("¡Gracias por jugar!", 59, 18);
            rlutil::setColor(rlutil::WHITE);
            rlutil::locate(1, 23); // Deja el cursor debajo del recuadro
            return true;
        }
        if (opcionSalir == 'n' || opcionSalir == 'N')
        {
            return false;
        }

        rlutil::setColor(rlutil::YELLOW);
        escribirCentrado("Entrada inválida. Debe ser S o N.", 59, y + 6);
        cin.clear();
        cin.ignore();
        rlutil::msleep(1000);
    }
}


/// ==================== FUNCIONES PARA DIBUJAR ====================

// Devuelve el color de cada jugador: rojo para el jugador 1 y celeste para el jugador 2
int colorJugador(int numero)
{
    if (numero == 1)
    {
        return rlutil::LIGHTRED;
    }
    return rlutil::LIGHTCYAN;
}

// Devuelve cuántas letras ocupa el texto en pantalla
int largoTexto(string texto)
{
    int cantLugares = texto.length();
    int largo = 0; // Cuantas letras se ven en pantalla

    for (int i = 0; i < cantLugares; i++)
    {
        // Las letras con tilde, la ñ, ¡, ¿ y los símbolos como ■ ocupan 2 o 3 lugares dentro del string,
        // pero en pantalla se ven como 1 sola letra. Los lugares "de más" tienen un valor menor a -64, así que esos no los contamos.
        if (texto[i] >= -64)
        {
            largo++;
        }
    }
    return largo;
}

// Escribe el texto centrado en la columna "centro", en la fila indicada.
// 59 es el centro del recuadro grande (que va de la columna 15 a la 103).
void escribirCentrado(string texto, int centro, int fila)
{
    rlutil::locate(centro - largoTexto(texto) / 2, fila);
    cout << texto;
}

// Dibuja una ventana: el recuadro grande (columnas 15 a 103) con el título metido en el borde de arriba,
// así:  ╔══════╡ TÍTULO ╞══════╗   El borde va del color indicado y el título en blanco.
void dibujarVentana(int y1, int y2, string titulo, int color)
{
    rlutil::setColor(color);
    dibujarRecuadro(15, y1, 103, y2);

    if (titulo != "")
    {
        rlutil::locate(59 - (largoTexto(titulo) + 4) / 2, y1);
        cout << "╡ ";
        rlutil::setColor(rlutil::WHITE);
        cout << titulo;
        rlutil::setColor(color);
        cout << " ╞";
    }
}

// Dibuja un recuadro de línea simple. (x1, y1) es la esquina de arriba a la izquierda y (x2, y2) la de abajo a la derecha
void dibujarRecuadroSimple(int x1, int y1, int x2, int y2)
{
    rlutil::locate(x1, y1);
    cout << "┌";
    for (int i = x1 + 1; i < x2; i++)
    {
        cout << "─";
    }
    cout << "┐";

    for (int j = y1 + 1; j < y2; j++)
    {
        rlutil::locate(x1, j);
        cout << "│";
        rlutil::locate(x2, j);
        cout << "│";
    }

    rlutil::locate(x1, y2);
    cout << "└";
    for (int i = x1 + 1; i < x2; i++)
    {
        cout << "─";
    }
    cout << "┘";
}

// Dibuja la tarjeta de un jugador: un recuadro en su color con su nombre, sus puntos y sus dados como cuadraditos.
// El jugador 1 va a la izquierda (columnas 19 a 57) y el jugador 2 a la derecha (columnas 61 a 99). Ocupa 6 filas.
// Si "ganador" es true, abajo de la tarjeta dice GANADOR.
void dibujarTarjetaJugador(string nombre, int numero, int puntos, int dados, int fila, bool ganador)
{
    int x1 = 19;
    if (numero == 2)
    {
        x1 = 61;
    }
    int centro = x1 + 19;

    rlutil::setColor(colorJugador(numero));
    dibujarRecuadroSimple(x1, fila, x1 + 38, fila + 5);
    escribirCentrado("┤ JUGADOR " + to_string(numero) + " ├", centro, fila);
    escribirCentrado(nombre, centro, fila + 1);

    rlutil::setColor(rlutil::WHITE);
    escribirCentrado("Puntos: " + to_string(puntos), centro, fila + 3);

    // Los dados que le quedan, dibujados como cuadraditos
    string barra = "Dados: ";
    if (dados == 0)
    {
        barra = barra + "¡ninguno!";
    }
    for (int i = 0; i < dados; i++)
    {
        barra = barra + "■ ";
    }
    barra = barra + "(" + to_string(dados) + ")";
    rlutil::setColor(colorJugador(numero));
    escribirCentrado(barra, centro, fila + 4);

    if (ganador == true)
    {
        rlutil::setColor(rlutil::YELLOW);
        escribirCentrado("┤ GANADOR ├", centro, fila + 5);
    }
}

// Dibuja el logo ENFRENDADOS en letras grandes, con un dado a cada lado. Ocupa 5 filas desde la fila indicada.
// "ENFREN" va en rojo (jugador 1) y "DADOS" en celeste (jugador 2). El logo mide 65 columnas y arranca en la 27.
void dibujarLogo(int fila)
{
    rlutil::setColor(colorJugador(1));
    dibujarDado(5, 17, fila);
    rlutil::setColor(colorJugador(2));
    dibujarDado(6, 93, fila);

    rlutil::setColor(colorJugador(1));
    rlutil::locate(27, fila);
    cout << "█████ █   █ █████ ████  █████ █   █";
    rlutil::locate(27, fila + 1);
    cout << "█     ██  █ █     █   █ █     ██  █";
    rlutil::locate(27, fila + 2);
    cout << "████  █ █ █ ████  ████  ████  █ █ █";
    rlutil::locate(27, fila + 3);
    cout << "█     █  ██ █     █  █  █     █  ██";
    rlutil::locate(27, fila + 4);
    cout << "█████ █   █ █     █   █ █████ █   █";

    rlutil::setColor(colorJugador(2));
    rlutil::locate(63, fila);
    cout << "████   ███  ████   ███   ████";
    rlutil::locate(63, fila + 1);
    cout << "█   █ █   █ █   █ █   █ █    ";
    rlutil::locate(63, fila + 2);
    cout << "█   █ █████ █   █ █   █  ███ ";
    rlutil::locate(63, fila + 3);
    cout << "█   █ █   █ █   █ █   █     █";
    rlutil::locate(63, fila + 4);
    cout << "████  █   █ ████   ███  ████ ";
}

// Borra lo que haya escrito dentro del recuadro grande en esa fila (sin borrar las paredes)
void limpiarFila(int fila)
{
    rlutil::locate(16, fila);
    for (int i = 16; i <= 102; i++)
    {
        cout << " ";
    }
}

// Dibuja un dado de 6 caras con sus puntitos. (x, y) es la esquina de arriba a la izquierda.
// Ocupa 9 columnas y 5 filas.
void dibujarDado(int valor, int x, int y)
{
    // Cada lugar donde puede ir un puntito empieza vacío
    string arribaIzq = " ", arribaDer = " ";
    string medioIzq = " ", centro = " ", medioDer = " ";
    string abajoIzq = " ", abajoDer = " ";

    if (valor >= 2)   // 2, 3, 4, 5 y 6 tienen las esquinas de una diagonal
    {
        arribaIzq = "●";
        abajoDer = "●";
    }
    if (valor >= 4)   // 4, 5 y 6 tienen las otras dos esquinas
    {
        arribaDer = "●";
        abajoIzq = "●";
    }
    if (valor == 6)   // 6 tiene además los dos del medio
    {
        medioIzq = "●";
        medioDer = "●";
    }
    if (valor % 2 == 1)   // 1, 3 y 5 (los impares) tienen el puntito del centro
    {
        centro = "●";
    }

    rlutil::locate(x, y);
    cout << "┌───────┐";
    rlutil::locate(x, y + 1);
    cout << "│ " << arribaIzq << "   " << arribaDer << " │";
    rlutil::locate(x, y + 2);
    cout << "│ " << medioIzq << " " << centro << " " << medioDer << " │";
    rlutil::locate(x, y + 3);
    cout << "│ " << abajoIzq << "   " << abajoDer << " │";
    rlutil::locate(x, y + 4);
    cout << "└───────┘";
}

// Dibuja un dado de 12 caras con su número en el medio. Ocupa 9 columnas y 5 filas.
void dibujarDadoDoce(int valor, int x, int y)
{
    rlutil::locate(x, y);
    cout << "╔═══════╗";
    rlutil::locate(x, y + 1);
    cout << "║       ║";
    rlutil::locate(x, y + 2);
    if (valor < 10)
    {
        cout << "║   " << valor << "   ║";
    }
    else   // Si tiene 2 cifras, arranca un lugar antes
    {
        cout << "║  " << valor << "   ║";
    }
    rlutil::locate(x, y + 3);
    cout << "║       ║";
    rlutil::locate(x, y + 4);
    cout << "╚═══════╝";
}

// Dibuja todos los dados centrados a partir de la fila indicada. Entran 8 dados por renglón.
// Los dados ya usados se dibujan en gris. Si conNumeros es true, abajo de cada dado va su número para elegirlo.
void dibujarDados(int dados[], int cantidad, bool usado[], int fila, int color, bool conNumeros)
{
    for (int i = 0; i < cantidad; i++)
    {
        int renglon = i / 8;       // En qué renglón de dados va (0 el primero, 1 el segundo)
        int lugar = i % 8;         // En qué lugar de ese renglón va

        int dadosEnRenglon = cantidad - renglon * 8; // Cuántos dados hay en este renglón
        if (dadosEnRenglon > 8)
        {
            dadosEnRenglon = 8;
        }

        int ancho = dadosEnRenglon * 10 - 1; // Cada dado ocupa 9 columnas más 1 de espacio
        int x = 59 - ancho / 2 + lugar * 10;
        int y = fila + renglon * 6;

        if (usado[i] == true)
        {
            rlutil::setColor(rlutil::DARKGREY);
        }
        else
        {
            rlutil::setColor(color);
        }
        dibujarDado(dados[i], x, y);

        if (conNumeros == true)
        {
            rlutil::setColor(rlutil::WHITE);
            if (i + 1 < 10)
            {
                rlutil::locate(x + 3, y + 5);
            }
            else
            {
                rlutil::locate(x + 2, y + 5);
            }
            cout << "[" << i + 1 << "]";
        }
    }
}

// Dibuja los dos dados de 12 caras y el número objetivo: [dado 1] + [dado 2] = [objetivo]
void dibujarObjetivo(int dado1, int dado2, int objetivo, int color)
{
    rlutil::setColor(color);
    dibujarDadoDoce(dado1, 43, 6);
    dibujarDadoDoce(dado2, 55, 6);

    rlutil::setColor(rlutil::WHITE);
    rlutil::locate(53, 8);
    cout << "+";
    rlutil::locate(65, 8);
    cout << "=";

    rlutil::setColor(rlutil::YELLOW);
    dibujarDadoDoce(objetivo, 67, 6);

    rlutil::setColor(rlutil::WHITE);
    rlutil::locate(44, 11);
    cout << "Dado 1";
    rlutil::locate(56, 11);
    cout << "Dado 2";
    rlutil::setColor(rlutil::YELLOW);
    rlutil::locate(67, 11);
    cout << "OBJETIVO";
}

// Dibuja el recuadro de la pantalla del turno, con el color del jugador que está jugando
void dibujarMarcoTurno(int ronda, string nombre, int numero, int puntos, int dados, string oponente, int dadosOponente)
{
    dibujarVentana(1, 30, "RONDA " + to_string(ronda) + " DE 3", colorJugador(numero));
    dibujarSeparador(15, 103, 4);
    dibujarSeparador(15, 103, 12);
    dibujarSeparador(15, 103, 27);

    // De quién es el turno y cómo viene
    rlutil::setColor(colorJugador(numero));
    escribirCentrado("TURNO DE " + nombre + "  (JUGADOR " + to_string(numero) + ")", 59, 2);
    rlutil::setColor(rlutil::DARKGREY);
    escribirCentrado("Tus puntos: " + to_string(puntos) + "   ·   Tus dados: " + to_string(dados) + "   ·   Dados de " + oponente + ": " + to_string(dadosOponente), 59, 3);
}

// Dibuja una opción del menú: [número] en amarillo, el nombre en blanco y la descripción en gris.
// Las tres partes van en columnas fijas para que queden alineadas y el bloque centrado.
void dibujarOpcionMenu(string numero, string nombre, string descripcion, int fila)
{
    rlutil::setColor(rlutil::YELLOW);
    rlutil::locate(31, fila);
    cout << "[" << numero << "]";

    rlutil::setColor(rlutil::WHITE);
    rlutil::locate(36, fila);
    cout << nombre;

    rlutil::setColor(rlutil::DARKGREY);
    rlutil::locate(53, fila);
    cout << descripcion;
}

// Dibuja los dados de un jugador como cuadraditos, en su color. Ej: "JUGADOR 1  ■ ■ ■  (3)"
void dibujarBarraDados(int numero, int cantidad, int x, int fila)
{
    rlutil::setColor(colorJugador(numero));
    rlutil::locate(x, fila);
    cout << "JUGADOR " << numero << "  ";
    for (int i = 0; i < cantidad; i++)
    {
        cout << "■ ";
    }
    cout << " (" << cantidad << ")";
}
