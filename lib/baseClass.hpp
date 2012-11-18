#include <stdlib.h>
#include <vector>
#include "instancia.hpp"
#include "solucion.hpp"

using namespace std;

extern bool InVector (vector<Solucion* >&, Solucion*);

#ifndef BASECLASS_H
#define BASECLASS_H

/*
 * Clase base utilizada para implementar las clases que aglutinan las
 * diferentes heurísticas.
*/
class BaseClass{
    public:
        BaseClass (Instancia &ins) : _instance(ins) {}

    protected:
        Instancia _instance;

        // Devuelve la mejor solucion vecina genrada
        Solucion* GeneraMejorVecina (Solucion* sIn);

        // Devuelve la mejor solucion vecina genrada teniendo en cuenta la lista 'tabu'
        Solucion* GeneraMejorVecina (Solucion* sIn, vector<Solucion* > tabu);

        // Devuelve una solucion vecina (a profundidad k) escogida al azar entre las posibles
        Solucion* GetVecinaRandom(Solucion* sIn, uint16_t k = 1);

        // Calcula el conjunto de las soluciones vecinas posibles
        // haciendo un movimiento (al primer contenedor donde deje menos espacio)
        vector<Solucion* >* GetVecinasMenosEspacio(Solucion* sIn);

    private:
        // Mueve el objeto en la posición 'obj' (dentro del vector de pesos o el vector solución),
        // al contenedor 'cont'. Refresca todos los valores afectados
        Solucion* MoverMenosEspacio(Solucion* &sIn, uint16_t obj, uint16_t cont);

        // Borra el contenedor 'pos' del vector de Espacios 'vEsp' y
        // propaga los cambios al vector solucion 'vSol'
        void BorrarContenedor(vector<uint16_t> &vSol, vector<uint16_t> &vEsp, uint16_t pos);
};
#endif
