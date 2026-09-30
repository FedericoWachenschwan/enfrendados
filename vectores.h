// Evita que el archivo se incluya más de una vez en la compilación.
#ifndef VECTORES_H_INCLUDED
#define VECTORES_H_INCLUDED

// Inclusión de bibliotecas necesarias.
#include <iostream> // Para operaciones de entrada/salida como cout y cin.
#include <cstdlib>  // Para funciones como rand y srand.
#include <ctime>    // Para usar time como semilla de números aleatorios.

/// DECLARACIÓN/PROTOTIPO DE FUNCIONES (solo firmas, sin implementación):

// Carga el vector con valores ingresados manualmente por el usuario.
void cargarVector(int v[], int tam);

// Muestra en pantalla el contenido del vector.
void mostrarVector(int v[], int tam);

// Asigna 0 a todas las posiciones del vector.
void ponerCero(int v[], int tam);

// Busca la posición del primer elemento igual a "numero" en el vector.
// Si lo encuentra, retorna su posición; si no, retorna -1.
int posicionNumeroEnVector(int v[], int tam, int numero);

// Cuenta cuántas veces se repite un número dentro del vector.
int contarNumerosRepetidos(int v[], int numero, int tam);

// Devuelve la posición del primer valor máximo encontrado en el vector.
int maximoVector(int v[], int tam);

// Devuelve la posición del primer valor mínimo encontrado en el vector.
int minimoVector(int v[], int tam);

// Carga el vector con números aleatorios en el rango [1, limite].
void cargarAleatorio(int v[], int tam, int limite);

// Ordena el vector de menor a mayor utilizando el método de selección.
void ordenarVector(int v[], int tam);

// Retorna la suma de todos los elementos del vector.
int sumarVector(int v[], int tam);

// Copia el contenido del vector v en el vector v2.
void copiarVector(int v[], int v2[], int tam);

// Compara dos vectores. Devuelve true si son idénticos, false si hay diferencias.
bool compararVectores(int v[], int v2[], int tam);


/// DEFINICIONES DE LAS FUNCIONES:

// Busca la posición del primer número igual a 'numero' en el vector.
int posicionNumeroEnVector(int v[], int tam, int numero) {
    for (int i = 0; i < tam; i++) {
        if (v[i] == numero) return i; // Si encuentra el número, devuelve la posición.
    }
    return -1; // Si no lo encuentra, devuelve -1.
}

// Devuelve la posición del mínimo valor en el vector.
int minimoVector(int v[], int tam) {
    int posMin = 0;
    for (int i = 1; i < tam; i++) {
        if (v[i] < v[posMin]) {
            posMin = i;
        }
    }
    return posMin;
}

// Ordena el vector de menor a mayor usando selección directa.
void ordenarVector(int v[], int tam) {
    int i, j, posmin, aux;
    for (i = 0; i < tam - 1; i++) {
        posmin = i; // Se asume que el mínimo está en la posición i.
        for (j = i + 1; j < tam; j++) {
            if (v[j] < v[posmin]) posmin = j; // Se busca un nuevo mínimo.
        }
        // Se intercambian los valores.
        aux = v[i];
        v[i] = v[posmin];
        v[posmin] = aux;
    }
}

// Devuelve la posición del valor máximo del vector.
int maximoVector(int v[], int tam) {
    int posMax = 0;
    for (int i = 1; i < tam; i++) {
        if (v[i] > v[posMax]) {
            posMax = i;
        }
    }
    return posMax;
}

// Llena el vector con valores aleatorios entre 1 y 'limite'.
void cargarAleatorio(int v[], int tam, int limite) {
    srand(time(NULL)); // Inicializa la semilla aleatoria con la hora actual.
    for (int i = 0; i < tam; i++) {
        v[i] = (rand() % limite) + 1; // Genera número entre 1 y 'limite'.
    }
}

// Muestra todos los elementos del vector separados por tabulación.
void mostrarVector(int v[], int tam) {
    for (int i = 0; i < tam; i++) {
        std::cout << v[i] << " ";
    }
}

// Cuenta cuántas veces aparece un número específico en el vector.
int contarNumerosRepetidos(int v[], int numero, int tam) {
    int cant = 0;
    for (int i = 0; i < tam; i++) {
        if (v[i] == numero) cant++; // Si el número coincide, incrementa el contador.
    }
    return cant;
}

// Asigna el valor 0 a todas las posiciones del vector.
void ponerCero(int v[], int tam) {
    for (int i = 0; i < tam; i++) {
        v[i] = 0;
    }
}

// Suma todos los valores almacenados en el vector.
int sumarVector(int v[], int tam) {
    int suma = 0;
    for (int i = 0; i < tam; i++) {
        suma += v[i];
    }
    return suma;
}

// Copia el contenido de un vector en otro.
void copiarVector(int v[], int v2[], int tam) {
    for (int i = 0; i < tam; i++) {
        v2[i] = v[i];
    }
}

// Compara si dos vectores tienen exactamente los mismos valores.
bool compararVectores(int v[], int v2[], int tam) {
    for (int i = 0; i < tam; i++) {
        if (v2[i] != v[i]) return false; // Si encuentra una diferencia, retorna false.
    }
    return true; // Si no hay diferencias, retorna true.
}

// Carga el vector con valores ingresados por el usuario.
void cargarVector(int v[], int tam) {
    for (int i = 0; i < tam; i++) {
        std::cout << "INGRESE NUMERO: ";
        std::cin >> v[i];
    }
}

// Fin del include guard (protección contra inclusiones múltiples).
#endif // VECTORES_H_INCLUDED
