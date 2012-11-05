#include <stdint.h>
#include <vector>
#include <time.h>

using namespace std;

#ifndef GENETICS_H
#define GENETICS_H


struct Contenedor{
    uint16_t id;    // ID del contendor
    uint16_t c;
    uint16_t usedSpace;  // Espacio usado
};

class Solucion{
    protected:
        vector<uint16_t> _vectorSolucion;
        vector<uint16_t> _vectorEspacios;   // El espacio ocupado en cada contenedor (El contenedor 'i' tendrá '_vectorEspacios[i]' de espacio ocupado)
        uint16_t _nContenedores;
        uint16_t _espacioLibre;

    public:
        Solucion(vector<uint16_t> vSol, vector<Contenedor* > vSpa, uint16_t c = 0, uint16_t e = 0) : _nContenedores(c), _espacioLibre(e) {
            if (vSol.size() > 0)
                SetVectorSolucion (vSol);
            if (vSpa.size() > 0)
                SetVectorEspacios(vSpa);
        }

        // GETTERS
        inline vector<uint16_t> GetVectorSolucion() { return _vectorSolucion; }
        inline vector<uint16_t> GetVectorEspacios() { return _vectorEspacios; }
        inline uint16_t GetNumContenedores() { return _nContenedores; }

        // SETTERS
        void SetVectorSolucion(vector<uint16_t> &v) {
            _vectorSolucion.clear();
            for (uint16_t i = 0; i < v.size(); i++){
                //cout << "Metiendo en vector " << v[i] << endl; // DEBUG
                _vectorSolucion.push_back(v[i]); 
            }
        }
        
        void SetVectorEspacios(vector<uint16_t> &v) {
            _vectorEspacios.clear();
            for (uint16_t i = 0; i < v.size(); i++){
                //cout << "Metiendo en vector " << v[i] << endl; // DEBUG
                _vectorEspacios.push_back(v[i]); 
            }
        }
        
        void SetVectorEspacios(vector<Contenedor* > &v) {
            _vectorEspacios.clear();
            for (uint16_t i = 0; i < v.size(); i++){
                //cout << "Metiendo en vector " << v[i] << endl; // DEBUG
                _vectorEspacios.push_back(v[i]->usedSpace); 
            }
        }
        
        
        inline void SetNumContenedores(uint16_t n) { _nContenedores = n; }

        // SobreCarga de Operadores
        friend ostream& operator << (ostream &o, Solucion &sol){
            o << "Vector Solucion: [ ";
            for (uint16_t i = 0; i < sol.GetVectorSolucion().size(); i++)
                o << sol.GetVectorSolucion()[i] << " ";
            o << "]" << endl;
            o << "Vector de Espacios: [ ";
            for (uint16_t i = 0; i < sol.GetVectorEspacios().size(); i++)
                o << sol.GetVectorEspacios()[i] << " ";
            o << "]" << endl;
            o << "Numero de contenedores: " << sol.GetNumContenedores() << endl;
            return o;
        }
};

/*
 *
 * GENETICS
 *
 */

#define EXPLOREDWEIGHT 0    // Marca para indicar que se ha introducido dicho objeto en algun contenedor


// Comprueba si 'v' ha sido completamente explorado (cada objeto asignado a un contenedor)
bool FullyExplored (vector<uint16_t> &v){
    for (uint16_t i = 0; i < v.size(); i++){
        if (v[i] != EXPLOREDWEIGHT)
            return false;
    }
    return true;
}

/* BUENAS SOLUCIONES */

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

// Instroduce cada objeto en el primer contenedor donde quepa
Solucion* GeneraSolucionPrimeroQuepa(Instancia* ins, bool ordena = false){
    vector<Contenedor* > contenedores;
    Contenedor* cont;
    vector<uint16_t> pesos = *ins->GetPesos();
    vector<uint16_t> v_solucion;
    bool insertado;  // Indica si se ha insertado el objeto 'i'

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
    Solucion* result = new Solucion(v_solucion, contenedores, contenedores.size());
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
                    pesos[i] = EXPLOREDWEIGHT;  // "Sacamos" del vector de pesos al objeto 'i'
                    //i = EXPLOREDWEIGHT;
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
    Solucion* result = new Solucion(solucion, contenedores, contenedores.size());
    //cout << "Creo solucion y salgo" << endl;
    return result;
}

#define DEFAULT_MAXGENERATIONS 200
class Genetics{
    protected:
        Instancia* _instance;
        vector<Solucion* >* _cjtoSoluciones;
        uint16_t _maxGeneraciones; // Número máximo de generaciones

    public:
        Genetics(Instancia* ins = NULL, vector<Solucion* >* sols = NULL, uint16_t mGen = DEFAULT_MAXGENERATIONS) : _maxGeneraciones(mGen) {
            if (ins == NULL)
                _instance = new Instancia();
            else
                _instance = ins;

            if (sols == NULL)
                _cjtoSoluciones = new vector<Solucion* >;
            else
                _cjtoSoluciones = sols;
        }

};

#endif
