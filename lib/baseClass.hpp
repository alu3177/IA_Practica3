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

#include <stdlib.h>
#include <vector>
#include "instancia.hpp"
#include "solucion.hpp"

using namespace std;

extern bool InVector (vector<Solucion* > &vin, Solucion* sol, uint16_t capacidad);
extern bool InVector (vector<uint16_t> &vin, uint16_t n);
extern bool FullyExplored (vector<uint16_t> &v);
extern uint16_t GetMayor(vector<uint16_t> &vin);
extern uint16_t Sum(vector<uint16_t> &vin);

#ifndef BASECLASS_H
#define BASECLASS_H

const float CHANCE = 0.5;  // Probabilidad de aceptar (y reparar) una "no factible" al generar vecinas random

#define EXPLOREDWEIGHT 0    // Marca para indicar que se ha introducido dicho objeto en algun contenedor
#define MAXITER        1200 // Numero maximo de iteraciones permitidas

/*
 * Clase base utilizada para implementar las clases que aglutinan las
 * diferentes heurísticas.
*/
class BaseClass{
    public:
        /*
         * CONSTRUCTOR
         */
        BaseClass (Instancia &ins) : _instance(ins) {}

    protected:
        Instancia _instance;

        /*
         * METODOS (Coleccion de heuristicas)
         */

            /* GENERADORES DE SOLUCIONES */
        // Instroduce cada objeto en el primer contenedor donde quepa
        Solucion* GeneraSolucionPrimeroQuepa(bool ordena = false);

        // Mete un objeto aleatorio en el primer contenedor que cabe
        Solucion* GeneraSolucionInicialRandom();

            /* MANEJO DE VECINAS */
        // Devuelve la mejor solucion vecina genrada
        Solucion* GeneraMejorVecina (Solucion* sIn);

        // Devuelve la mejor solucion vecina genrada teniendo en cuenta la lista 'tabu'
        Solucion* GeneraMejorVecina (Solucion* sIn, vector<uint16_t> &tabu, uint16_t tMax);

        // Devuelve una solucion vecina escogida al azar entre las posibles
        Solucion* GetVecinaRandom(Solucion* sIn);

        // Devuelve una solucion vecina (a profundidad k) escogida al azar entre las posibles
        Solucion* GetVecinaRandom(Solucion* sIn, uint16_t k);

        // Mueve el objeto en la posición 'obj' (dentro del vector de pesos o el vector solución),
        // al contenedor 'cont'. Refresca todos los valores afectados
        Solucion* MoverObjeto(Solucion* &sIn, uint16_t obj, uint16_t cont);

        // Realiza una busqueda voraz
        Solucion* Greedy(Solucion* &sIn);

    private:
        // Borra el contenedor 'pos' del vector de Espacios 'vEsp' y
        // propaga los cambios al vector solucion 'vSol'
        void BorrarContenedor(vector<uint16_t> &vSol, vector<uint16_t> &vEsp, uint16_t pos);
};
#endif
