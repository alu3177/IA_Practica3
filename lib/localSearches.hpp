#include <math.h>
#include "baseClass.hpp"


#ifndef LOCALSEARCHES_H
#define LOCALSEARCHES_H

const uint32_t MAXTEMP = 99;   // Temperatura maxima inicial para el SA
const float DEFAULTSTEP = 0.333;  // Con 0.333 se obtienen valores mediocres en un tiempo aceptable
                                  // Con 0.133 se obtienen valores aceptables en un tiempo mediocre

extern Solucion* GeneraSolucionInicialRandom(Instancia* ins);
extern Solucion* GeneraSolucionPrimeroQuepa(Instancia* ins, bool ordena = false);

class LocalSearches : public BaseClass {
    public:
        LocalSearches(Instancia ins) : BaseClass(ins) {}
/*
        // Búsqueda Local Iterada
        Solucion* ILS(){
            bool seguir = true;
            uint32_t maxIteraciones = 150; // TODO: make me a constant
            uint32_t iter = 0;
            Solucion* actual = GeneraSolucionInicialRandom(&_instance);
            Solucion* mejorSol = actual;

            while (iter < maxIteraciones){
                vector<Solucion* >* sols = GetVecinasMenosEspacio(actual);
                for (uint16_t i = 0; i < sols->size(); i++){
                    if (*sols->at(i) <= *mejorSol){
                        mejorSol = sols->at(i);
                    } else break;
                }
                iter++;
                actual = GeneraSolucionInicialRandom(&_instance);
            }
            //cout << "Mejor Solución:" << endl << mejorSol << endl;
            return mejorSol;

        }
*/

        // Búsqueda Local Iterada
        Solucion* ILS(){
            uint32_t maxIteraciones = 100;
            uint32_t iter = 0;
            Solucion* actual = GeneraSolucionInicialRandom(&_instance);
            Solucion* vecina = NULL;
            Solucion* mejorSol = actual;

            while (iter < maxIteraciones){
                while (1){
                    vecina = GeneraMejorVecina(actual);
                    if (*vecina < *mejorSol){
                        mejorSol = vecina;
                    }else
                        break;
                }
                iter++;
                actual = GeneraSolucionInicialRandom(&_instance);
            }
            return mejorSol;
        }

        // Búsqueda Local Iterada
        Solucion* ILS2(){
            bool seguir = true;
            uint32_t maxIteraciones = 2; // Usamos 2 puesto que observando el algoritmo se dedujo que no se producirán mas de dos conjuntos de vecinas distintos.
            uint32_t iter = 0;
            Solucion* actual = GeneraSolucionPrimeroQuepa(&_instance, true);
            Solucion* mejorSol = actual;

            while (iter < maxIteraciones){
                vector<Solucion* >* sols = GetVecinasMenosEspacio(actual);
                //cout << "sols.size = " << sols->size() << endl; // DEBUG
                for (uint16_t i = 0; i < sols->size(); i++){
                    if (*sols->at(i) <= *mejorSol){
                        actual = sols->at(i);
                        mejorSol = actual;
                    } else break;
                }
                iter++;
            }
            return mejorSol;

        }


        // Devuelve un valor de Temperatura basado en el numero de iteración 't'
        // y el tamaño de los pasos entre iteraciones.
        // Con cada iteracion 't' se reduce 'step * t' el valor de Temperatura
        float GetTemperatura (uint32_t t, float step = DEFAULTSTEP){
            if ((MAXTEMP - (step * t)) > 0){
                return (MAXTEMP - (step * t));
            }
            return 0;
        }

        // Simulated Annealing (Recocido simulado)
        //      Cuanto menor sea DEFAULTSTEP más ciclos se generaran
        //      Cuanto mayor sea MAXTEMP mayores serán las probabilidades de aceptación
        Solucion* SA(){
            uint32_t iter = 1; // Numero de iteraciones
            float Temp = MAXTEMP;     // Temperatura actual
            Solucion* actual = GeneraSolucionInicialRandom(&_instance);
            Solucion* siguiente;
            srand ( time(NULL) + rand() );   // Inicializamos la semilla del RANDOM
            do{
                //cout << "Init" << endl; // DEBUG
                Temp = GetTemperatura(iter);
                //cout << "Temp = " << Temp << endl; //DEBUG
                if (Temp == 0)
                    return actual;

                siguiente = GetVecinaRandom(actual);
                //cout << "Siguiente" << endl; // DEBUG
                int delta = *siguiente - *actual;  // Gradiente entre siguiente y actual
                //cout << "Definido delta" << endl; // DEBUG
                if (delta >= 0){
                    //cout << "Delta >= 0" << endl; // DEBUG
                    actual = siguiente;
                }else{
                    float probabilidad = exp((float)((float)delta/Temp));    // Probabilidad de aceptar 'siguiente' => e^(delta/Temp)
                    float aceptacion = (float)rand()/(float)RAND_MAX; // Genera un numero entre 0.0 y 1.0
                    // Aceptaremos la solución si ' aceptacion <= probabilidad'
                    if (aceptacion <= probabilidad){
                        //cout << "Delta(" << delta << "): " << aceptacion <<  " <= " << probabilidad << endl;  // DEBUG
                        actual = siguiente;
                        //cout << "CHECHECHE" << endl;// DEBUG
                    }
                }
                iter++;
                //cout << "iter++" << endl; // DEBUG
            } while (Temp > 0);

            return actual;
        }

        // Simulated Annealing 2 (Recocido simulado modificado)
        // Obtiene la vecina aleatoria a 'k' pasos de la actual
        //      Cuanto menor sea DEFAULTSTEP más ciclos se generaran
        //      Cuanto mayor sea MAXTEMP mayores serán las probabilidades de aceptación
        Solucion* SA2(uint16_t k = 2){
            uint32_t iter = 1; // Numero de iteraciones
            float Temp = MAXTEMP;     // Temperatura actual
            Solucion* actual = GeneraSolucionInicialRandom(&_instance);
            Solucion* siguiente;
            srand ( time(NULL) + rand() );   // Inicializamos la semilla del RANDOM
            do{
                //cout << "Init" << endl; // DEBUG
                Temp = GetTemperatura(iter);
                //cout << "Temp = " << Temp << endl; //DEBUG
                if (Temp == 0)
                    return actual;

                // En esta ocasion 'siguiente' será una solucion vecina a k movimientos (inspirado en VNS)
                siguiente = GetVecinaRandom(actual, k);
                /*
                if (k > 1){
                    uint16_t deep = 1; // En este paso ya hemos generado la primera solucion (linea superior)
                    while (deep < k){
                        siguiente = GetVecinaRandom(siguiente);
                        deep++;
                    }
                }
                */
                //cout << "Siguiente" << endl; // DEBUG
                int delta = *siguiente - *actual;  // Gradiente entre siguiente y actual
                //cout << "Definido delta" << endl; // DEBUG
                if (delta >= 0){
                    //cout << "Delta >= 0" << endl; // DEBUG
                    actual = siguiente;
                }else{
                    float probabilidad = exp((float)((float)delta/Temp));    // Probabilidad de aceptar 'siguiente' => e^(delta/Temp)
                    float aceptacion = (float)rand()/(float)RAND_MAX; // Genera un numero entre 0.0 y 1.0
                    // Aceptaremos la solución si ' aceptacion <= probabilidad'
                    if (aceptacion <= probabilidad){
                        //cout << "Delta(" << delta << "): " << aceptacion <<  " <= " << probabilidad << endl;  // DEBUG
                        actual = siguiente;
                        //cout << "CHECHECHE" << endl;// DEBUG
                    }
                }
                iter++;
                //cout << "iter++" << endl; // DEBUG
            } while (Temp > 0);

            return actual;
        }

        Solucion* VNS(uint16_t kMax = 5){
            Solucion* actual = GeneraSolucionInicialRandom(&_instance);
            Solucion* vecina = actual;
            uint16_t k = 1;
            do{
                vecina = GetVecinaRandom(actual, k);
                vecina = Greedy(vecina);
                if (*vecina < *actual){
                    //cout << "VNS GOOT" << endl; // DEBUG
                    actual = vecina;
                    k = 1;
                }else{
                    k++;
                    //cout << "VNS BAAD" << endl; // DEBUG
                }
            }while (k <= kMax);
            return actual;
        }


};

#endif
