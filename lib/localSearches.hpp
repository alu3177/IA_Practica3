#include "baseClass.hpp"

using namespace std;

#ifndef LOCALSEARCHES_H
#define LOCALSEARCHES_H

const float MAXTEMP = 975.35;   // Temperatura maxima inicial para el SA
const float DEFAULTSTEP = 0.133;  // Con 0.333 se obtienen valores mediocres en un tiempo aceptable
                                  // Con 0.133 se obtienen valores aceptables en un tiempo mediocre
const uint16_t MAXITERATIONS = 150;  // Numero maximo de iteraciones (Usado en TS)
const uint16_t VNS_K = 8;
const uint16_t TS_T = 5;

class LocalSearches : public BaseClass {
    public:
        LocalSearches(Instancia &ins) : BaseClass(ins) {}

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

        // Realiza una busqueda voraz
        Solucion* Greedy(Solucion* &sIn);
};
#endif
