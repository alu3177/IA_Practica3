#include "instancia.hpp"
#include "solucion.hpp"

#ifndef BASECLASS_H
#define BASECLASS_H

extern uint16_t GetMayor (vector<uint16_t> &v);

class BaseClass{
    public:
        BaseClass (Instancia ins) : _instance(ins) {}

        inline void Print() { cout << _instance << endl; }  // DEBUG: BORRAME

    protected:
        Instancia _instance;
/*        
        Solucion* GeneraMejorVecina (Solucion* sIn){
            cout << GetMayor(*_instance.GetPesos()) << endl;
            for (uint16_t i = 0; i < sIn->GetVectorEspacios().size(); i++){
                uint16_t pesado = GetMayor(*_instance.GetPesos());
                if ((sIn->GetVectorEspacios()[i] + _instance.GetPeso(pesado)) <= _instance.GetCapacidadC()){
                    cout << "Moviendo objeto " << pesado << "(" << _instance.GetPeso(pesado) << ") de " << sIn->GetVectorSolucion()[pesado] << " a " << i << endl; // DEBUG
                }
            }
        }
*/
        // Mueve el objeto en la posición 'obj' (dentro del vector de pesos o el vector solución),
        // al contenedor 'cont'. Refresca todos los valores afectados
        Solucion* MoverMenosEspacio(Solucion* &sIn, uint16_t obj, uint16_t cont){
            // Si cabe, generamos la solución con este movimiento y la añadimos a 'result'
            vector<uint16_t> vSolTmp = sIn->GetVectorSolucion();
            vector<uint16_t> vEspTmp = sIn->GetVectorEspacios();
            uint16_t nEspacioLibre = sIn->GetEspacioLibre(); // Nuevo espacio libre

            vSolTmp[obj] = cont; // Cambiamos el objeto 'obj' de contenedor (cont)
            vEspTmp[sIn->GetVectorSolucion()[obj]] -= _instance.GetPeso(obj); // Descontamos espacio usado del contenedor del que lo sacamos
            if (vEspTmp[sIn->GetVectorSolucion()[obj]] <= 0){  // Si se queda a cero, lo borramos
                vEspTmp.erase(vEspTmp.begin() + sIn->GetVectorSolucion()[obj]);
                nEspacioLibre -= _instance.GetCapacidadC();
                //cout << "BORRANDO CONTENEDOR " << sIn->GetVectorSolucion()[obj] << endl;  // DEBUG
            }
            vEspTmp[cont] += _instance.GetPesos()->at(obj); // Añadimos espacio usado al contenedor al que lo movemos

            
            Solucion* nSol = new Solucion(vSolTmp, vEspTmp, nEspacioLibre);

            return nSol;
        }
/*        
        // Calcula el conjunto de las soluciones vecinas posibles
        // haciendo un movimiento (al primer contenedor donde deje menos espacio)
        vector<Solucion* > GetVecinasMenosEspacio(Solucion* sIn){
            vector<Solucion* > result;
            uint16_t mejor; // Mejor valor (de menos espacio dejado) encontrado
            uint16_t bestCont; // ID del mejor contenedor encontrado
            // Recorremos el vector de resultados
            for (uint16_t i = 0; i < sIn->GetVectorSolucion().size(); i++){
                mejor = _instance.GetCapacidadC(); // Iniciamos al maximo valor posible (peor caso) para cada objeto
                // Recorremos los contenedores
                for (uint16_t j = 0; j < sIn->GetNumContenedores(); j++){
                    if (sIn->GetVectorSolucion()[i] != j){ // No comprobamos para el mismo contenedor
                        uint16_t libre = _instance.GetCapacidadC() - sIn->GetVectorEspacios()[j]; // Espacio libre en el contenedor 'j'
                        //cout << "[" << i <<  "]libre " << j << " : " << libre << endl;
                        if (_instance.GetPesos()->at(i) <= libre){ // ¿Cabe?
                            if (mejor > libre){  // Deja menos espacio 'libre' que 'mejor'
                                mejor = libre;
                                bestCont = j;
                            }
                        }
                    }
                }
                if (mejor != _instance.GetCapacidadC()){
                    //cout << "Moviendo objeto " << i << "(" << _instance.GetPesos()->at(i) << ") de " << sIn->GetVectorSolucion()[i] << " a " << bestCont << endl; // DEBUG
                    result.push_back(MoverMenosEspacio(sIn, i, bestCont));  // Insertamos 'i' en 'bestCont'
                }
            }
            return result;
        }
*/

        // Calcula el conjunto de las soluciones vecinas posibles
        // haciendo un movimiento (al primer contenedor donde deje menos espacio)
        vector<Solucion* > GetVecinasMenosEspacio(Solucion* sIn){
            vector<Solucion* > result;
            uint16_t mejor; // Mejor valor (de menos espacio dejado) encontrado
            uint16_t bestCont; // ID del mejor contenedor encontrado
            // Recorremos el vector de resultados
            for (uint16_t i = 0; i < sIn->GetVectorSolucion().size(); i++){
                mejor = _instance.GetCapacidadC(); // Iniciamos al maximo valor posible (peor caso) para cada objeto
                // Recorremos los contenedores
                for (uint16_t j = 0; j < sIn->GetNumContenedores(); j++){
                    if (sIn->GetVectorSolucion()[i] != j){ // No comprobamos para el mismo contenedor
                        uint16_t libre = _instance.GetCapacidadC() - sIn->GetVectorEspacios()[j]; // Espacio libre en el contenedor 'j'
                        //cout << "[" << i <<  "]libre " << j << " : " << libre << endl;
                        if (_instance.GetPeso(i) < libre){ // ¿Cabe?
                            //cout << "Moviendo objeto " << i << "(" << _instance.GetPeso(i) << ") de " << sIn->GetVectorSolucion()[i] << " a " << j << endl; // DEBUG
                            result.push_back(MoverMenosEspacio(sIn, i, j));  // Insertamos 'i' en 'bestCont'
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
