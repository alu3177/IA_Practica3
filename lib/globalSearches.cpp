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

#include <math.h>
#include "globalSearches.hpp"

using namespace std;

// Búsqueda Local Iterada
// METAHEURISTICA
// 1) ILS
Solucion* GlobalSearches::ILS(){
    uint32_t iter = 0;
    Solucion* actual = GeneraSolucionInicialRandom();
    Solucion* vecina = NULL;
    Solucion* mejorSol = actual;

    while (iter < MAXITERATIONS){
        while (1){
            vecina = GeneraMejorVecina(actual);
            if (vecina->Objetivo() < mejorSol->Objetivo()){
                mejorSol = vecina;
            }else
                break;
        }
        iter++;
        actual = GeneraSolucionInicialRandom();
    }
    return mejorSol;
}

// Devuelve un valor de Temperatura basado en el numero de iteración 't'
// y el tamaño de los pasos entre iteraciones.
// Con cada iteracion 't' se reduce 'step * t' el valor de Temperatura
float GlobalSearches::GetTemperatura (uint32_t t, float step){
    if ((MAXTEMP - (step * t)) > 0){
        return (MAXTEMP - (step * t));
    }
    return 0;
}

// Simulated Annealing (Recocido simulado)
//      Cuanto menor sea DEFAULTSTEP más ciclos se generaran
//      Cuanto mayor sea MAXTEMP mayores serán las probabilidades de aceptación
// METAHEURISTICA
// 2) SA
Solucion* GlobalSearches::SA(){
    uint32_t iter = 1; // Numero de iteraciones
    float Temp = MAXTEMP;     // Temperatura actual
    Solucion* actual = GeneraSolucionInicialRandom();
    Solucion* siguiente;
    srand ( time(NULL) + rand() );   // Inicializamos la semilla del RANDOM
    do{
        Temp = GetTemperatura(iter);
        if (Temp == 0)
            return actual;

        siguiente = GetVecinaRandom(actual);
        int delta = siguiente->Objetivo() - actual->Objetivo();  // Gradiente entre siguiente y actual
        if (delta >= 0){
            actual = siguiente;
        }else{
            float probabilidad = exp((float)((float)delta/Temp));    // Probabilidad de aceptar 'siguiente' => e^(delta/Temp)
            float aceptacion = (float)rand()/(float)RAND_MAX; // Genera un numero entre 0.0 y 1.0
            // Aceptaremos la solución si ' aceptacion <= probabilidad'
            if (aceptacion <= probabilidad){
                actual = siguiente;
            }
        }
        iter++;
    } while (Temp > 0);

    return actual;
}

// Busqueda por ENTORNO VARIABLE básica
// METAHEURISTICA
// 6) VNS
Solucion* GlobalSearches::VNS(uint16_t kMax){
    Solucion* actual = GeneraSolucionInicialRandom();
    Solucion* vecina = actual;
    uint16_t k = 1;

    do{
        vecina = GetVecinaRandom(actual, k);
        vecina = Greedy(vecina);
        if (vecina->Objetivo() < actual->Objetivo()){
            actual = vecina;
            k = 1;
        }else{
            k++;
        }
    }while (k <= kMax);
    return actual;
}

// Busqueda TABU
// MODIFICACION 1
Solucion* GlobalSearches::TS(uint16_t tMax){
    Solucion* actual = GeneraSolucionInicialRandom();
    Solucion* mejorSol = actual;
    vector<uint16_t> tabu;
    uint16_t iter = 0;

    while (iter < MAXITERATIONS){
        actual = GeneraMejorVecina(actual, tabu, tMax); // Genera la vecina teniendo en cuenta la lista Tabu. Ademas actualiza la lista
        if (actual->ObjetivoAux(_instance.GetCapacidadC()) < mejorSol->ObjetivoAux(_instance.GetCapacidadC())){
            mejorSol = actual;
        }
        iter++;
    }
    return mejorSol;
}
