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

#include "baseClass.hpp"

using namespace std;

#ifndef GLOBALSEARCHES_H
#define GLOBALSEARCHES_H

const float MAXTEMP = 5000;   // Temperatura maxima inicial para el SA
const float DEFAULTSTEP = 0.333;  // Tamaño de los pasos para calcular la temeperatura en el SA
const uint16_t MAXITERATIONS = 150;  // Numero maximo de iteraciones (Usado en TS e ILS)
const uint16_t VNS_K = 8;
const uint16_t TS_T = 5;

class GlobalSearches : public BaseClass {
    public:
        GlobalSearches(Instancia &ins) : BaseClass(ins) {}

        // Búsqueda Local Iterada
        Solucion* ILS();

        // Simulated Annealing (Recocido simulado)
        //      Cuanto menor sea DEFAULTSTEP más ciclos se generaran
        //      Cuanto mayor sea MAXTEMP mayores serán las probabilidades de aceptación
        Solucion* SA();

        // Simulated Annealing 2 (Recocido simulado modificado)
        // Obtiene la vecina aleatoria a 'k' pasos de la actual
        //      Cuanto menor sea DEFAULTSTEP más ciclos se generaran
        //      Cuanto mayor sea MAXTEMP mayores serán las probabilidades de aceptación
        Solucion* SA2(uint16_t k = 3);

        // Busqueda por Entorno Variable
        Solucion* VNS(uint16_t kMax = VNS_K);

        // Busqueda Tabu
        Solucion* TS(uint16_t tMax = TS_T);
    private:
        // Devuelve un valor de Temperatura basado en el numero de iteración 't'
        // y el tamaño de los pasos entre iteraciones.
        // Con cada iteracion 't' se reduce 'step * t' el valor de Temperatura
        // ** Usado por el Recocido Simulado **
        float GetTemperatura (uint32_t t, float step = DEFAULTSTEP);

        // Instroduce 'nSol' al final de 'vin'. Si se sobrepasa 'tMax' se extrae el primer elemento de 'vin'
        // ** Usado por la Búsqueda Tabú **
        void ActualizaTabu (vector<Solucion* > &vin, Solucion* nSol, uint16_t tMax = TS_T);
};
#endif
