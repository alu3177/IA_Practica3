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


        // Borra el contenedor 'pos' del vector de Espacios 'vEsp' y
        // propaga los cambios al vector solucion 'vSol'
        void BorrarContenedor(vector<uint16_t> &vSol, vector<uint16_t> &vEsp, uint16_t pos){
            vector<uint16_t> vEspTmp;

            // Construimos el nuevo vector de pesos (result)
            for (uint16_t i = 0; i < vEsp.size(); i++){
                if (i != pos)
                    vEspTmp.push_back(vEsp[i]);
            }
            vEsp = vEspTmp;
            // Actualizamos los IDs de contenedor del vector solucion
            // Restamos uno a todos los IDs de contenedor mayores que el que hemos borrado
            for (uint16_t i = 0; i < vSol.size(); i++){
                if (vSol[i] > pos)
                    vSol[i]--;
            }
        }

        // Mueve el objeto en la posición 'obj' (dentro del vector de pesos o el vector solución),
        // al contenedor 'cont'. Refresca todos los valores afectados
        Solucion* MoverMenosEspacio(Solucion* &sIn, uint16_t obj, uint16_t cont){
            //cout << "MoverMenosEspacio" << endl; // DEBUG

            // Si cabe, generamos la solución con este movimiento y la añadimos a 'result'
            vector<uint16_t> vSolTmp = sIn->GetVectorSolucion();
            vector<uint16_t> vEspTmp = sIn->GetVectorEspacios();
            uint16_t nEspacioLibre = sIn->GetEspacioLibre(); // Nuevo espacio libre


            int prevCont = sIn->GetVectorSolucion()[obj];
            //cout << "vEspTmp[" << prevCont << "](";
            //cout  << vEspTmp[prevCont] <<  ") -= " << _instance.GetPeso(obj) << endl; //DEBUG
            vEspTmp[prevCont] -= _instance.GetPeso(obj); // Descontamos espacio usado del contenedor del que lo sacamos
            vSolTmp[obj] = cont; // Cambiamos el objeto 'obj' de contenedor (cont)
            if (vEspTmp[prevCont] <= 0){  // Si se queda a cero, lo borramos
                //cout << "BORRANDO CONTENEDOR " << prevCont << endl;  // DEBUG
                //vEspTmp = BorrarElemento(vEspTmp, prevCont);
                //vEspTmp.erase(vEspTmp.begin() + prevCont);
                BorrarContenedor(vSolTmp, vEspTmp, prevCont);
                nEspacioLibre -= _instance.GetCapacidadC();
                //cout << "BORRANDO !!!!" << endl;  // DEBUG
            }
            vEspTmp[cont] += _instance.GetPeso(obj); // Añadimos espacio usado al contenedor al que lo movemos


            Solucion* nSol = new Solucion(vSolTmp, vEspTmp, nEspacioLibre);

            //cout << "[FIN]  MoverMenosEspacio" << endl; // DEBUG

            return nSol;
        }

        // Calcula el conjunto de las soluciones vecinas posibles
        // haciendo un movimiento (al primer contenedor donde deje menos espacio)
        vector<Solucion* >* GetVecinasMenosEspacio(Solucion* sIn){
            //cout << "GetVecinasMenosEspacio" << endl; // DEBUG
            vector<Solucion* >* result = new vector<Solucion* >;
            uint16_t mejor; // Mejor valor (de menos espacio dejado tras insertar) encontrado
            int bestCont;   // ID del mejor contenedor encontrado
            // Recorremos el vector de resultados
            for (uint16_t i = 0; i < sIn->GetVectorSolucion().size(); i++){
                bestCont = -1;
                mejor = _instance.GetCapacidadC(); // Iniciamos al maximo valor posible (peor caso) para cada objeto
                // Recorremos los contenedores
                for (uint16_t j = 0; j < sIn->GetNumContenedores(); j++){
                    if (sIn->GetVectorSolucion()[i] != j){ // No comprobamos para el mismo contenedor
                        uint16_t libre = _instance.GetCapacidadC() - sIn->GetVectorEspacios()[j]; // Espacio libre en el contenedor 'j'
                        //cout << "[" << i <<  "]libre " << j << " : " << libre << endl;
                        if (_instance.GetPeso(i) < libre){ // ¿Cabe?
                            //cout << "Moviendo objeto " << i << "(" << _instance.GetPeso(i) << ") de " << sIn->GetVectorSolucion()[i] << " a " << j << endl; // DEBUG
                            /*
                            if (libre - _instance.GetPeso(i) < mejor){  // Si deja menos espacio libre que el anterior mejor resultado...
                                mejor = libre - _instance.GetPeso(i);
                                bestCont = j;
                            }
                            */
                            result->push_back(MoverMenosEspacio(sIn, i, j));  // Insertamos 'i' en 'bestCont' (j)
                        }
                    }
                }
                //if ((bestCont > 0) && (mejor != _instance.GetCapacidadC()))
                //    result->push_back(MoverMenosEspacio(sIn, i, bestCont));  // Insertamos 'i' en 'bestCont'
            }
            //cout << "[FIN]  GetVecinasMenosEspacio" << endl; // DEBUG
            return result;
        }

        // Devuelve una solucion vecina escogida al azar entre las posibles
        Solucion* GetVecinaRandom(Solucion* sIn){
            srand ( time(NULL) + rand() );   // Inicializamos la semilla del RANDOM
            vector<Solucion* > vecinas = *GetVecinasMenosEspacio(sIn);

            //cout << "Devolviendo vecina Random: " << endl; // DEBUG
            if (vecinas.size() > 0)
                return vecinas[rand() % vecinas.size()];
            return sIn;
        }

};
/*
class Derivada : public BaseClass{
    public:
        Derivada(Instancia ins) : BaseClass(ins) {}
};
*/
#endif
