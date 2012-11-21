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

#include <vector>
#include "solucion.hpp"
#include "baseClass.hpp"

#ifndef GENETICS_H
#define GENETICS_H

#define MAXGENERATIONS 50     // Numero maximo de generaciones
#define POBLACION_SIZE 100    // Tamaño de la poblacion inicial
#define BEST_COUNT     30     // Se escogeran las BEST_COUNT en cada generacion
#define COUPLES_NUMBER 100    // Numero de parejas formadas en cada generacion
#define MUTANTS        10     // Numero de descendientes que mutan
class Genetics : public BaseClass {
    public:
        // Constructor
        Genetics(Instancia &ins) : BaseClass(ins) {}

        // Algoritmo genetico
        Solucion* AG(uint16_t maxGen = MAXGENERATIONS);

    private:
        // Selecciona las 'n' mejores soluciones de 'pob'
        vector<Solucion* > SeleccionaMejores(vector<Solucion* > pob, uint16_t n = BEST_COUNT);

        // Escoge COUPLES_NUMBER parejas de 's' y las cruza. Despues muta MUTANTS
        // elementos y devuelve la nueva generacion entera
        vector<Solucion* > Operar(vector<Solucion* > &s);

        // Construye un vector de 'p' parejas (aleatorias) de soluciones
        vector<pair<Solucion*, Solucion* > > SeleccionaParejas(vector<Solucion* > pob, uint16_t p = COUPLES_NUMBER);

        // Cruza una pareja de soluciones y obtiene (devuelve) dos soluciones resultantes
        pair<Solucion*, Solucion* > Cruzar(pair<Solucion*, Solucion* > &pareja);

        // Realiza una mutacion (cambia un valor al azar del vector solucion)
        // Devuelve 'sol' si no es factible
        Solucion Mutar(Solucion* sol);

};

#endif
