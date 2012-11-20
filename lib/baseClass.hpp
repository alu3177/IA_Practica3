#include <stdlib.h>
#include <vector>
#include "instancia.hpp"
#include "solucion.hpp"

using namespace std;

extern bool InVector (vector<Solucion* > &vin, Solucion* sol, uint16_t capacidad);
extern bool FullyExplored (vector<uint16_t> &v);
extern uint16_t GetMayor(vector<uint16_t> &vin);
extern uint16_t Sum(vector<uint16_t> &vin);

#ifndef BASECLASS_H
#define BASECLASS_H

#define EXPLOREDWEIGHT 0    // Marca para indicar que se ha introducido dicho objeto en algun contenedor

/*
 * Clase base utilizada para implementar las clases que aglutinan las
 * diferentes heurísticas.
*/
class BaseClass{
    public:
        BaseClass (Instancia &ins) : _instance(ins) {}

    protected:
        Instancia _instance;

            /* GENERADORES DE SOLUCIONES */
        // Instroduce cada objeto en el primer contenedor donde quepa
        Solucion* GeneraSolucionPrimeroQuepa(bool ordena = false);

        // Mete un objeto aleatorio en el primer contenedor que cabe
        Solucion* GeneraSolucionInicialRandom();

            /* MANEJO DE VECINAS */
        // Devuelve la mejor solucion vecina genrada
        Solucion* GeneraMejorVecina (Solucion* sIn);

        // Devuelve la mejor solucion vecina genrada teniendo en cuenta la lista 'tabu'
        Solucion* GeneraMejorVecina (Solucion* sIn, vector<Solucion* > tabu);

        // Devuelve una solucion vecina genrada al azar teniendo en cuenta la lista 'tabu'
        // ** Usado por la Búsqueda Tabú **
        Solucion* GeneraVecinaRandom (Solucion* sIn, vector<Solucion* > tabu);

        // Devuelve una solucion vecina escogida al azar entre las posibles
        Solucion* GetVecinaRandom(Solucion* sIn);

        // Devuelve una solucion vecina (a profundidad k) escogida al azar entre las posibles
        Solucion* GetVecinaRandom(Solucion* sIn, uint16_t k);

        // Calcula el conjunto de las soluciones vecinas posibles
        // haciendo un movimiento (al primer contenedor donde deje menos espacio)
        //vector<Solucion* >* GetVecinasMenosEspacio(Solucion* sIn);

    private:
        // Mueve el objeto en la posición 'obj' (dentro del vector de pesos o el vector solución),
        // al contenedor 'cont'. Refresca todos los valores afectados
        Solucion* MoverObjeto(Solucion* &sIn, uint16_t obj, uint16_t cont);

        // Borra el contenedor 'pos' del vector de Espacios 'vEsp' y
        // propaga los cambios al vector solucion 'vSol'
        void BorrarContenedor(vector<uint16_t> &vSol, vector<uint16_t> &vEsp, uint16_t pos);
};
#endif
