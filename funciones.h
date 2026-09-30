#pragma once
#include <string>


void mostrarPortada();
void mostrarMensajeYEsperar(std::string mensaje = "Presione cualquier tecla para continuar...");
void dibujarRecuadro(int x1, int y1, int x2, int y2);
void dibujarSeparador(int x1, int x2, int fila);
void esperarTecla(int fila);
int decidirQuienEmpieza(std::string nombre1, std::string nombre2);
int menuOpciones();
void reglamento();
void creditos();
int dadoDoceCaras();
void jugarPartida(std::string jugadorInicial, std::string jugadorOponente, int dadosInicial[], int dadosOponente[], int& cantDadosInicial, int& cantDadosOponente, int& puntosInicial, int& puntosOponente, int numeroInicial);
void mostrarUltimaOportunidad(std::string jugadorSinDados, std::string rival, int numeroSinDados);
bool realizarTurno(std::string jugadorActual, std::string oponente, int dadosActual[], int dadosOponente[], int& cantDadosActual, int& cantDadosOponente, int& puntosActual, int ronda, int numeroActual);
void estadisticasDelJuego(std::string jugador1, std::string jugador2, int puntos1, int puntos2, int dados1, int dados2, std::string nombreMejorHistorico, int puntosMejorHistorico, bool huboPartidaJugada);
void tirarDados(int dados[], int cantidad);
void mostrarEstadoDeRonda(int ronda, std::string jugador1, std::string jugador2, int puntos1, int puntos2, int dados1, int dados2, int numeroInicial);
void mostrarFinDePartida(std::string nombre1, std::string nombre2, int puntos1, int puntos2, int dados1, int dados2, bool nuevoRecord);
void pedirNombres(std::string& nombre1, std::string& nombre2);
std::string pedirNombreValido(int numero, std::string nombreOtro, int fila);
bool confirmarSalida();

// Funciones para dibujar
int colorJugador(int numero);
std::string textoDados(int cantidad);
int largoTexto(std::string texto);
void escribirCentrado(std::string texto, int centro, int fila);
void limpiarFila(int fila);
void dibujarVentana(int y1, int y2, std::string titulo, int color);
void dibujarRecuadroSimple(int x1, int y1, int x2, int y2);
void dibujarTarjetaJugador(std::string nombre, int numero, int puntos, int dados, int fila, bool ganador);
void dibujarLogo(int fila);
void dibujarDado(int valor, int x, int y);
void dibujarDadoDoce(int valor, int x, int y);
void dibujarDados(int dados[], int cantidad, bool usado[], int fila, int color, bool conNumeros);
void dibujarObjetivo(int dado1, int dado2, int objetivo, int color);
void dibujarMarcoTurno(int ronda, std::string nombre, int numero, int puntos, int dados, std::string oponente, int dadosOponente);
void dibujarOpcionMenu(std::string numero, std::string nombre, std::string descripcion, int fila);
void dibujarBarraDados(int numero, int cantidad, int x, int fila);
