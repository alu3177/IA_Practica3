#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include "instancia.hpp"
#include "solucion.hpp"

using namespace std;

#define EXPLOREDWEIGHT 0    // Marca para indicar que se ha introducido dicho objeto en algun contenedor

// Metodo utilizado para ordenar de menor a mayor (espacio dejado) soluciones en std::sort
bool CompareSolucion (Solucion* s1, Solucion* s2){
    return (s1->GetEspacioLibre() < s2->GetEspacioLibre());
}

/* FUNCIONES AUXILIARES de manejo de char* */
void ResetChar (char* str, uint16_t size){
   for (uint16_t i = 0; i < size; i++)
      str[i] = 0;
}

bool IsEmpty (char* str, uint16_t size){
   for (uint16_t i = 0; i < size; i++)
      if (str[i] != 0)
         return false;
   return true;
}

// Comprueba si 'v' ha sido completamente explorado (cada objeto asignado a un contenedor)
bool FullyExplored (vector<uint16_t> &v){
    for (uint16_t i = 0; i < v.size(); i++){
        if (v[i] != EXPLOREDWEIGHT)
            return false;
    }
    return true;
}

// Ordena de mayor a menor
void BubbleSort(vector<uint16_t> &v){
    for (uint16_t i = 1; i < v.size(); i++){
        for (uint16_t j = 0; j < v.size() - 1; j++){
            if (v[j] < v[j+1]){
                uint16_t tmp = v[j];
                v[j] = v[j+1];
                v[j+1] = tmp;
            }
        }
    }
}

// Comprueba si 'sol' está en 'vin'
bool InVector (vector<Solucion* > &vin, Solucion* sol){
    for (uint16_t i = 0; i < vin.size(); i++){
        if (*vin[i] == *sol){
            //cout << *vin[i] << " == " << *sol << endl; // DEBUG
            //cout << C_CYAN << "     IGUALL !!" << C_DEFAULT << endl; // DEBUG
            return true;
        }
    }
    //cout << "No estaba ..." << endl; // DEBUG
    return false;
}

// Comprueba si 'n' está en 'vin'
bool InVector (vector<uint16_t> &vin, uint16_t n){
    for (uint16_t i = 0; i < vin.size(); i++){
        if (vin[i] == n){
            return true;
        }
    }
    return false;
}

// Reemplaza los valores 'org' por 'newVal' en el vector 'vin'
void Replace(vector<uint16_t> &vin, uint16_t org, uint16_t newVal){
    for (uint16_t i = 0; i < vin.size(); i++)
        if (vin[i] == org)
            vin[i] = newVal;
}
// Deveulve el valor mayor dentro de 'vin'
uint16_t GetMayor(vector<uint16_t> &vin){
    uint16_t result = 0;
    for (uint16_t i = 0; i < vin.size(); i++)
        if (vin[i] > result)
            result = vin[i];
    return result;
}
// Deveulve la posicion del valor mayor dentro de 'vin'
uint16_t GetPosicionMayor(vector<uint16_t> &vin){
    uint16_t mayor = 0;
    uint16_t mayorPos = 0;
    for (uint16_t i = 0; i < vin.size(); i++)
        if (vin[i] > mayor){
            mayor = vin[i];
            mayorPos = i;
        }
    return mayorPos;
}
// Deveulve el valor menor dentro de 'vin'
uint16_t GetMenor(vector<uint16_t> &vin){
    uint16_t result = 9999;
    for (uint16_t i = 0; i < vin.size(); i++)
        if (vin[i] < result){
            //cout << vin[i] << "<" << result << endl; // DEBUG
            result = vin[i];
        }
    return result;
}
// Deveulve la posicion del valor menor dentro de 'vin'
uint16_t GetPosicionMenor(vector<uint16_t> &vin){
    uint16_t menor = 9999;
    uint16_t menorPos = 0;
    for (uint16_t i = 0; i < vin.size(); i++)
        if (vin[i] < menor){
            menor = vin[i];
            menorPos = i;
        }
    return menorPos;
}

// Instroduce cada objeto en el primer contenedor donde quepa
Solucion* GeneraSolucionPrimeroQuepa(Instancia* ins, bool ordena){
    vector<Contenedor* > contenedores;
    Contenedor* cont;
    vector<uint16_t> pesos = *ins->GetPesos();
    vector<uint16_t> v_solucion;
    bool insertado;  // Indica si se ha insertado el objeto 'i'
    uint16_t sumPesos = 0; // Suma de los pesos insertados

    if (ordena){
        BubbleSort(pesos);
        ins->SetPesos(pesos); // Persistimos los cambios en la instacia, para mantener la coherencia
    }

    for (int i = 0; i < pesos.size(); i++){
        insertado = false;
        for (uint16_t j = 0; j < contenedores.size(); j++){ // Recorremos vector de contenedores
            if ( pesos[i] <= (ins->GetCapacidadC() - contenedores[j]->usedSpace) ){ // El objeto 'i' cabe en el contenedor 'j'
                v_solucion.push_back(contenedores[j]->id);
                contenedores[j]->usedSpace += pesos[i];
                sumPesos += pesos[i]; // Restamos espacio libre
                insertado = true;
                 break;
             }
         }
         if (!insertado){   // No lo hemos insertado (no cabe en ningun contenedor)
             cont = new Contenedor;
             cont->id = contenedores.size();
             cont->c = ins->GetCapacidadC();
             cont->usedSpace = 0;
             contenedores.push_back(cont);
             i--;    // Evitamos avanzar de objeto (para introducirlo)
        }
    }
    uint16_t freeSpace = (contenedores.size() * ins->GetCapacidadC()) - sumPesos; // Establecemos el espacio libre (Total - sumPesos)
    Solucion* result = new Solucion(v_solucion, contenedores, freeSpace);
    return result;
}

/* SOLUCIONES INICIALES */
// Mete un objeto aleatorio en el primer contenedor que cabe
Solucion* GeneraSolucionInicialRandom(Instancia* ins){
    vector<Contenedor* > contenedores;
    Contenedor* cont;// = new Contenedor;
    vector<uint16_t> pesos = *ins->GetPesos();
    vector<uint16_t> solucion(ins->GetNumObjetos());
    bool insertado;  // Indica si se ha insertado el objeto 'i'
    uint16_t sumPesos = 0; // Suma de los pesos insertados

    srand ( time(NULL) + rand() );   // Inicializamos la semilla del RANDOM

    uint16_t i;     // Representa a cada objeto en el vector de pesos (el objeto 'i' tiene un peso 'pesos[i]')
    while (!FullyExplored(pesos)){
        insertado = false;
        i = rand() % ins->GetNumObjetos();    // Seleccionamos un elemento al azar
        if (pesos[i] != EXPLOREDWEIGHT){   // Hemos seleccionado un elemento no asignado
            //cout << "Seleccionado " << i << endl; // DEBUG
            for (uint16_t j = 0; j < contenedores.size(); j++){ // Recorremos vector de contenedores
                if ( pesos[i] <= (contenedores[j]->c - contenedores[j]->usedSpace) ){ // El objeto 'i' cabe en el contenedor 'j'
                    //cout << "INSERTANDO objeto " << i << " en contenedor " << j << endl; // DEBUG
                    solucion[i] = contenedores[j]->id;
                    contenedores[j]->usedSpace += pesos[i];
                    sumPesos += pesos[i]; // Incrementamos el peso usado
                    pesos[i] = EXPLOREDWEIGHT;  // "Sacamos" del vector de pesos al objeto 'i'
                    insertado = true;
                    break;
                }
            }
            if (!insertado){   // No lo hemos insertado (no cabe en ningun contenedor)
                cont = new Contenedor;
                cont->id = contenedores.size();
                cont->c = ins->GetCapacidadC();
                cont->usedSpace = 0;
                contenedores.push_back(cont);
                //cout << "CREO contenedor " << cont->id << endl;  // DEBUG
            }
        }
    }
    uint16_t freeSpace = (contenedores.size() * ins->GetCapacidadC()) - sumPesos; // Establecemos el espacio libre (Total - sumPesos)
    Solucion* result = new Solucion(solucion, contenedores, freeSpace);
    //cout << "Creo solucion y salgo" << endl;
    return result;
}
