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

#include <algorithm>
#include "baseClass.hpp"

using namespace std;

extern bool FullyExplored (vector<uint16_t>&);

#ifndef CONSTRUCTIVAS_H
#define CONSTRUCTIVAS_H

#define EXPLOREDWEIGHT 0    // Marca para indicar que se ha introducido dicho objeto en algun contenedor
#define GRASP_T        77   // Tamaño por defecto de la LRC

// Representa cada uno de los pasos candidatos
struct Candidato{
    uint16_t oID;  // ID del objeto candidato
    uint16_t cID;  // ID del contendor candidato
    uint16_t freeSpace;  // Espacio libre en 'cID' tras insertar 'oID'
};

class Constructivas : public BaseClass {
    public:
        // Constructor
        Constructivas(Instancia &ins) : BaseClass(ins) {}
        // Heurísticas
        Solucion* GRASP(uint16_t t = GRASP_T);

    private:
        vector<Candidato> GetListaRestringidaCandidatos(vector<uint16_t> &pesos, vector<Contenedor* > &contenedores, uint16_t t);
};
#endif
