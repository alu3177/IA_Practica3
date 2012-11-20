#include <math.h>
#include "localSearches.hpp"

using namespace std;

// Búsqueda Local Iterada
Solucion* LocalSearches::ILS(){
    uint32_t maxIteraciones = 100;
    uint32_t iter = 0;
    Solucion* actual = GeneraSolucionInicialRandom();
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
        actual = GeneraSolucionInicialRandom();
    }
    return mejorSol;
}

// Devuelve un valor de Temperatura basado en el numero de iteración 't'
// y el tamaño de los pasos entre iteraciones.
// Con cada iteracion 't' se reduce 'step * t' el valor de Temperatura
float LocalSearches::GetTemperatura (uint32_t t, float step){
    if ((MAXTEMP - (step * t)) > 0){
        return (MAXTEMP - (step * t));
    }
    return 0;
}

// Simulated Annealing (Recocido simulado)
//      Cuanto menor sea DEFAULTSTEP más ciclos se generaran
//      Cuanto mayor sea MAXTEMP mayores serán las probabilidades de aceptación
Solucion* LocalSearches::SA(){
    uint32_t iter = 1; // Numero de iteraciones
    float Temp = MAXTEMP;     // Temperatura actual
    Solucion* actual = GeneraSolucionInicialRandom();
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
Solucion* LocalSearches::SA2(uint16_t k){
    uint32_t iter = 1; // Numero de iteraciones
    float Temp = MAXTEMP;     // Temperatura actual
    Solucion* actual = GeneraSolucionInicialRandom();
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

Solucion* LocalSearches::VNS(uint16_t kMax){
    Solucion* actual = GeneraSolucionInicialRandom();
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

// Instroduce 'nSol' al final de 'vin'. Si se sobrepasa 'tMax' se extrae el primer elemento de 'vin'
// ** Usado por la Búsqueda Tabú **
void LocalSearches::ActualizaTabu(vector<Solucion* > &vin, Solucion* nSol, uint16_t tMax){
    vin.push_back(nSol);
    if (vin.size() > tMax)
        vin.erase(vin.begin());
}

Solucion* LocalSearches::TS(uint16_t tMax){
    Solucion* actual = GeneraSolucionInicialRandom();
    //Solucion* actual = GeneraSolucionPrimeroQuepa();
    Solucion* mejorSol = actual;
    vector<Solucion* > tabu;
    uint16_t iter = 0;

    while (iter < MAXITERATIONS){
        actual = GeneraMejorVecina(actual, tabu);
        //cout << "Fin While 1" << endl; // DEBUG
        //cout << *actual << " < " << *mejorSol << endl; // DEBUG
        if (actual->ObjetivoAux(_instance.GetCapacidadC()) < mejorSol->ObjetivoAux(_instance.GetCapacidadC())){
            mejorSol = actual;
            cout << C_CYAN << "      MEJORA" << C_DEFAULT << endl; // DEBUG
        }
        //cout << "Fin IF" << endl; // DEBUG
        ActualizaTabu(tabu, actual, tMax);
        //cout << "Fin Update Tabu" << endl; // DEBUG
        iter++;
        //cout << "ITER++" << endl; // DEBUG
    }
    return mejorSol;
}

// Realiza una busqueda voraz
Solucion* LocalSearches::Greedy(Solucion* &sIn){
    Solucion* actual = sIn;
    Solucion* vecina = actual;

    while (1){
        vecina = GeneraMejorVecina(actual);
        if (*vecina < *actual)
            actual = vecina;
        else
            return actual;
    }
}
