#include "instancia.hpp"
#include "solucion.hpp"

#ifndef BASECLASS_H
#define BASECLASS_H

class BaseClass{
    public:
        BaseClass (Instancia ins) : _instance(ins) {}

        inline void Print() { cout << _instance << endl; }  // DEBUG: BORRAME

    protected:
        Instancia _instance;

        // Mueve el objeto en la posición 'obj' (dentro del vector de pesos o el vector solución),
        // al contenedor 'cont'. Refresca todos los valores afectados
        Solucion* MoverMenosEspacio(Solucion* &sIn, uint16_t obj, uint16_t cont){
            // Si cabe, generamos la solución con este movimiento y la añadimos a 'result'
            vector<uint16_t> vSolTmp = sIn->GetVectorSolucion();
            vector<uint16_t> vEspTmp = sIn->GetVectorEspacios();

            vSolTmp[obj] = cont; // Cambiamos el objeto 'i' de contenedor (j)
            vEspTmp[sIn->GetVectorSolucion()[obj]] -= _instance.GetPesos()->at(obj); // Descontamos espacio usado del contenedor del que lo sacamos
            vEspTmp[cont] += _instance.GetPesos()->at(obj); // Añadimos espacio usado al contenedor al que lo movemos
            Solucion* nSol = new Solucion(vSolTmp, vEspTmp, sIn->GetEspacioLibre());

            return nSol;
        }
        // Calcula el conjunto de las soluciones vecinas posibles
        // haciendo un movimiento (al primer contenedor donde deje menos espacio)
        vector<Solucion* > GetVecinasMenosEspacio(Solucion* sIn){
            vector<Solucion* > result;
            // Recorremos el vector de resultados
            for (uint16_t i = 0; i < sIn->GetVectorSolucion().size(); i++){
                // Recorremos los contenedores
                for (uint16_t j = 0; j < sIn->GetNumContenedores(); j++){
                    if (sIn->GetVectorSolucion()[i] != j){ // No comprobamos para el mismo contenedor
                        if (_instance.GetPesos()->at(i) <= _instance.GetCapacidadC() - sIn->GetVectorEspacios()[j]){
                            cout << "Moviendo objeto " << i << "(" << _instance.GetPesos()->at(i) << ") de " << sIn->GetVectorSolucion()[i] << " a " << j << endl; // DEBUG
                            result.push_back(MoverMenosEspacio(sIn, i, j));
                            // TODO: Falta el caso en el que se vacía algún contenedor
                        }
                    }
                }
            }
            return result;
        }

};
/*
class Derivada : public BaseClass{
    public:
        Derivada(Instancia ins) : BaseClass(ins) {}
};
*/
#endif
