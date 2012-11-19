#include "baseClass.hpp"

using namespace std;

#ifndef LOCALSEARCHES_H
#define LOCALSEARCHES_H

const uint16_t MAXTEMP = 1000;   // Temperatura maxima inicial para el SA
const float DEFAULTSTEP = 0.133;  // Con 0.333 se obtienen valores mediocres en un tiempo aceptable
                                  // Con 0.133 se obtienen valores aceptables en un tiempo mediocre
const uint16_t MAXITERATIONS = 350;  // Numero maximo de iteraciones (Usado en TS)
const uint16_t VNS_K = 8;
const uint16_t TS_T = 5;

extern Solucion* GeneraSolucionInicialRandom(Instancia* ins);
extern Solucion* GeneraSolucionPrimeroQuepa(Instancia* ins, bool ordena = false);

class LocalSearches : public BaseClass {
    public:
        LocalSearches(Instancia &ins) : BaseClass(ins) {}

        // Búsqueda Local Iterada
        Solucion* ILS();
        // Búsqueda Local Iterada
        Solucion* ILS2();

        // Simulated Annealing (Recocido simulado)
        //      Cuanto menor sea DEFAULTSTEP más ciclos se generaran
        //      Cuanto mayor sea MAXTEMP mayores serán las probabilidades de aceptación
        Solucion* SA();
        // Simulated Annealing 2 (Recocido simulado modificado)
        // Obtiene la vecina aleatoria a 'k' pasos de la actual
        //      Cuanto menor sea DEFAULTSTEP más ciclos se generaran
        //      Cuanto mayor sea MAXTEMP mayores serán las probabilidades de aceptación
        Solucion* SA2(uint16_t k = 3);

        Solucion* VNS(uint16_t kMax = VNS_K);
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
