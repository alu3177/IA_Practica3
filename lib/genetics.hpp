#include <vector>
#include "solucion.hpp"
#include "baseClass.hpp"

#ifndef GENETICS_H
#define GENETICS_H

#define MAXGENERATIONS 70     // Numero maximo de generaciones
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
/*
        // Calcula el promedio de huecos de la poblacion 'pob'
        float EvaluaPoblacion(vector<Solucion* > &pob){
            uint32_t huecos = 0;
            for (uint16_t i = 0; i < pob.size(); i++)
                huecos += pob[i]->GetEspacioLibre();
            return (huecos / pob.size());
        }
*/
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
