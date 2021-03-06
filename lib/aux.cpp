/*
 * Inteligencia Artificial - Práctica 3: Problema de empaquetado unidimensional
 *
 * AUTOR: Fernando González López-Peñalver (alu3177)
 * EMAIL: alu0100256543@ull.edu.es
 * FECHA DE ENTREGA: 22 - 11 -12
 *
 * Se incluye repositorio GIT con el desarrollo de la práctica.
 *
 * Enunciado del problema:
 *      Se dispone de una cantidad indeterminada de contenedores de
 *      capacidad C y n objetos de tamaños w1, w2, …, wn. Se trata de
 *      determinar el empaquetado de todos los objetos usando el menor
 *      número posible de contenedores m*. La suma de los tamaños de los
 *      objetos asignados a contenedor no puede sobrepasar la capacidad C.
 *
 *      Implementar los siguientes algoritmos:
 *          - ILS   (Busqueda Local Iterada)
 *          - SA    (Recocido Simulado)
 *          - VNS   (Busqueda por Entorno Variable, basica)
 *          - TS    (Busqueda Tabu)
 *          - GRASP (Procedimiento de Busqueda Adaptativa Aleatoria Voraz)
 *          - AG    (Algoritmo Genetico)
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include "instancia.hpp"
#include "solucion.hpp"

#define EXPLOREDWEIGHT 0    // Marca para indicar que se ha introducido dicho objeto en algun contenedor

using namespace std;

// Resetea 'str' (pone todos sus valores a 0)
void ResetChar (char* str, uint16_t size){
   for (uint16_t i = 0; i < size; i++)
      str[i] = 0;
}

// Comprueba si 'str' está vacia (todos sus valores == 0)
bool IsEmpty (char* str, uint16_t size){
   for (uint16_t i = 0; i < size; i++)
      if (str[i] != 0)
         return false;
   return true;
}

// Comprueba si 'v' ha sido completamente explorado (cada objeto asignado a un contenedor)
bool FullyExplored (vector<uint16_t> &v){
    for (uint16_t i = 0; i < v.size(); i++){
        if (v[i] != EXPLOREDWEIGHT)
            return false;
    }
    return true;
}

// Inicializa 'vin' a ceros
void InitVector (vector<uint16_t> &vin){
    for (uint16_t i = 0; i < vin.size(); i++)
        vin[i] = 0;
}

// Comprueba si 'sol' está en 'vin' segun la funcion objetivo auxiliar
bool InVector (vector<Solucion* > &vin, Solucion* sol, uint16_t capacidad){
    for (uint16_t i = 0; i < vin.size(); i++){
        if (vin[i]->ObjetivoAux(capacidad) == sol->ObjetivoAux(capacidad)){
            return true;
        }
    }
    return false;
}

// Comprueba si 'n' está en 'vin'
bool InVector (vector<uint16_t> &vin, uint16_t n){
    for (uint16_t i = 0; i < vin.size(); i++){
        if (vin[i] == n){
            return true;
        }
    }
    return false;
}

// Reemplaza los valores 'org' por 'newVal' en el vector 'vin'
void Replace(vector<uint16_t> &vin, uint16_t org, uint16_t newVal){
    for (uint16_t i = 0; i < vin.size(); i++)
        if (vin[i] == org)
            vin[i] = newVal;
}

// Deveulve el valor mayor dentro de 'vin'
uint16_t GetMayor(vector<uint16_t> &vin){
    uint16_t result = 0;
    for (uint16_t i = 0; i < vin.size(); i++)
        if (vin[i] > result)
            result = vin[i];
    return result;
}
// Deveulve la posicion del valor mayor dentro de 'vin'
uint16_t GetPosicionMayor(vector<uint16_t> &vin){
    uint16_t mayor = 0;
    uint16_t mayorPos = 0;
    for (uint16_t i = 0; i < vin.size(); i++)
        if (vin[i] > mayor){
            mayor = vin[i];
            mayorPos = i;
        }
    return mayorPos;
}

// Devuelve el valor menor dentro de 'vin'
uint16_t GetMenor(vector<uint16_t> &vin){
    uint16_t result = 9999;
    for (uint16_t i = 0; i < vin.size(); i++)
        if (vin[i] < result){
            result = vin[i];
        }
    return result;
}
// Deveulve la posicion del valor menor dentro de 'vin'
uint16_t GetPosicionMenor(vector<uint16_t> &vin){
    uint16_t menor = 9999;
    uint16_t menorPos = 0;
    for (uint16_t i = 0; i < vin.size(); i++)
        if (vin[i] < menor){
            menor = vin[i];
            menorPos = i;
        }
    return menorPos;
}

// Devuelve el resultado de sumar todos los elementos de 'vin'
uint16_t Sum(vector<uint16_t> &vin){
    uint16_t sum = 0;
    for (uint16_t i = 0; i < vin.size(); i++)
        sum += vin[i];

    return sum;
}
