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
 *          - GA    (Algoritmo Genetico)
 *
 */

#include <math.h>
#include "globalSearches.hpp"

using namespace std;

// Búsqueda Local Iterada
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

// Simulated Annealing 2 (Recocido simulado modificado)
// Obtiene la vecina aleatoria a 'k' pasos de la actual
//      Cuanto menor sea DEFAULTSTEP más ciclos se generaran
//      Cuanto mayor sea MAXTEMP mayores serán las probabilidades de aceptación
Solucion* GlobalSearches::SA2(uint16_t k){
    uint32_t iter = 1; // Numero de iteraciones
    float Temp = MAXTEMP;     // Temperatura actual
    Solucion* actual = GeneraSolucionInicialRandom();
    Solucion* siguiente;
    srand ( time(NULL) + rand() );   // Inicializamos la semilla del RANDOM
    do{
        Temp = GetTemperatura(iter);
        if (Temp == 0)
            return actual;
        // En esta ocasion 'siguiente' será una solucion vecina a k movimientos (inspirado en VNS)
        siguiente = GetVecinaRandom(actual, k);
        int delta = *siguiente - *actual;  // Gradiente entre siguiente y actual
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

// Instroduce 'nSol' al final de 'vin'. Si se sobrepasa 'tMax' se extrae el primer elemento de 'vin'
// ** Usado por la Búsqueda Tabú **
void GlobalSearches::ActualizaTabu(vector<Solucion* > &vin, Solucion* nSol, uint16_t tMax){
    vin.push_back(nSol);
    if (vin.size() > tMax)
        vin.erase(vin.begin());
}

// Busqueda TABU
Solucion* GlobalSearches::TS(uint16_t tMax){
    Solucion* actual = GeneraSolucionInicialRandom();
    //Solucion* actual = GeneraSolucionPrimeroQuepa();
    Solucion* mejorSol = actual;
    vector<Solucion* > tabu;
    uint16_t iter = 0;

    while (iter < MAXITERATIONS){
        actual = GeneraMejorVecina(actual, tabu);
        if (actual->ObjetivoAux(_instance.GetCapacidadC()) < mejorSol->ObjetivoAux(_instance.GetCapacidadC())){
            mejorSol = actual;
        }
        ActualizaTabu(tabu, actual, tMax);
        iter++;
    }
    return mejorSol;
}
