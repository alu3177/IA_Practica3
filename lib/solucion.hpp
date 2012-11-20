#include <stdint.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
#include "../definitions.h"

using namespace std;

#ifndef SOLUCION_H
#define SOLUCION_H

extern bool InVector (vector<uint16_t> &vin, uint16_t n);
extern void Replace(vector<uint16_t> &vin, uint16_t org, uint16_t newVal);
extern uint16_t GetMayor(vector<uint16_t> &vin);
extern uint16_t GetPosicionMayor(vector<uint16_t> &vin);
extern uint16_t GetMenor(vector<uint16_t> &vin);
extern uint16_t GetPosicionMenor(vector<uint16_t> &vin);
extern void InitVector (vector<uint16_t> &vin);
extern uint16_t Sum(vector<uint16_t> &vin);

struct Contenedor{
    uint16_t id;    // ID del contendor
    uint16_t c;
    uint16_t usedSpace;  // Espacio usado (Si está lleno => usedSpace == c)
};

class Solucion{
    protected:
        vector<uint16_t> _vectorSolucion;
        vector<uint16_t> _vectorEspacios;   // El espacio ocupado en cada contenedor (El contenedor 'i' tendrá '_vectorEspacios[i]' de espacio ocupado)
        uint16_t _nContenedores;
        uint16_t _espacioLibre;

    public:
        // CONSTRUCTORES
        Solucion(vector<uint16_t> vSol, vector<Contenedor* > vSpa, uint16_t e, uint16_t c = 0) : _nContenedores(c), _espacioLibre(e) {
            if (vSol.size() > 0)
                SetVectorSolucion (vSol);
            if (vSpa.size() > 0)
                SetVectorEspacios(vSpa);
            if (c == 0)
                _nContenedores = _vectorEspacios.size();
        }

        Solucion(vector<uint16_t> vSol, vector<uint16_t> vSpa, uint16_t e, uint16_t c = 0) : _nContenedores(c), _espacioLibre(e) {
            if (vSol.size() > 0)
                SetVectorSolucion (vSol);
            if (vSpa.size() > 0)
                SetVectorEspacios(vSpa);
            if (c == 0)
                _nContenedores = _vectorEspacios.size();
        }

        // Constructor a partir de un vector solucion, el vector de pesos y la capacidad de los contenedores
        Solucion(vector<uint16_t> vSol, uint16_t capacidad, vector<uint16_t> pesos){
            if (vSol.size() > 0){
                SetVectorSolucion (vSol);
            }
            // Obtenemos el numero de contenedores
            _nContenedores = 0;
            _espacioLibre = 0;
            vector<uint16_t> contenedores;  // Vector con las IDs de los contenedores extraidas de vSol
            for (uint16_t i = 0; i < _vectorSolucion.size(); i++){
                if (!InVector(contenedores, _vectorSolucion[i])){
                    _nContenedores++;
                    contenedores.push_back(_vectorSolucion[i]);
                }
            }

            sort(contenedores.begin(), contenedores.end());
            // Comprobamos que los IDs de los contenedores sean correctos
            // (Los Ids de contenedores del vector solucion pueden no ser consecutivos)
            // El valor 'contenedores[i]' debe ser igual a 'i' tras haberlos ordenado
            for (uint16_t i = 0; i < contenedores.size(); i++){
                if (contenedores[i] != i){
                    Replace(_vectorSolucion, contenedores[i], i);
                    contenedores[i] = i;
                }
            }

            // Calculamos los valores de '_vectorEspacios'
            for (uint16_t j = 0; j < _nContenedores; j++){ // Recorremos contenedores
                uint16_t usedSpaceCont = 0;  // Espacio usado en el contenedor actual 'j'
                for (uint16_t i = 0; i < _vectorSolucion.size(); i++){  // Recorremos objetos
                    if (_vectorSolucion[i] == j){  // Objeto 'i' esta en contenedor 'j'?
                        usedSpaceCont += pesos[i];
                        //cout << "Objeto " << i << " en contenedor " << j << endl; // DEBUG
                    }
                }
                _vectorEspacios.push_back(usedSpaceCont);
                _espacioLibre += capacidad - usedSpaceCont;
            }

            // Comprobamos si algun contenedor excede su capacidad maxima
            if (GetMayor(_vectorEspacios) > capacidad){
                this->RepairOverLoad(capacidad, pesos);
            }
        }

        // Repara la solucion para evitar la sobrecarga de capacidad de contenedores
        void RepairOverLoad(uint16_t capacidad, vector<uint16_t> pesos){
            //cout << " OVERLOAD !!" << endl; // DEBUG
            //cout << C_CYAN << *this << endl; // DEBUG
            //while (!this->Factible(capacidad)){
            _espacioLibre = 0;
            for (uint16_t j = 0; j < _nContenedores; j++){  // Recorremos contenedores (en el vector de espacios)
                if(_vectorEspacios[j] > capacidad){  // Se produce SobreCarga del contenedor 'j'
                    uint16_t nCap = 0;  // Nueva capacidad del contenedor
                    for (uint16_t i = 0; i < _vectorSolucion.size(); i++){  // Recorremos el vector solucion buscando los asociados a 'j'
                        if (_vectorSolucion[i] == j){  // 'i' esta en 'j'
                            if ((nCap + pesos[i]) <= capacidad){  // Objeto 'i' cabe en 'j'
                                nCap += pesos[i];
                            }else/* if (_vectorEspacios[j] >= pesos[i])*/{  // Objeto 'i' no cabe en 'j'
                                //cout << i << " [" << pesos[i] << "] no cabe en " << j << " -- (" << GetMenor(_vectorEspacios) << ", " << GetPosicionMenor(_vectorEspacios) << ")" << endl; // DEBUG
                                //cout << (capacidad - GetMenor(_vectorEspacios)) <<  ">=" << pesos[i] << "  ?" << endl; // DEBUG
                                if ((capacidad - GetMenor(_vectorEspacios)) >= pesos[i]){  // ¿Cabe en algun otro contenedor?
                                    //cout << (capacidad - GetMenor(_vectorEspacios)) <<  ">=" << pesos[i] << "  !!" << endl; // DEBUG
                                    //cout << "Moviendo " << i << " a " << GetPosicionMenor(_vectorEspacios) << endl; // DEBUG
                                    _vectorSolucion[i] = GetPosicionMenor(_vectorEspacios);
                                    _vectorEspacios[GetPosicionMenor(_vectorEspacios)] += pesos[i];
                                    _vectorEspacios[j] -= pesos[i];

                                }else{ // No cabe en ningun contenedor => Creamos uno
                                    //cout << "_vectorSolucion[" << i << "] " << _vectorSolucion[i] << " == " << j << "      ?       ";
                                    //cout << "_vectorEspacios[" << j << "] (" << _vectorEspacios[j] << ") -= " << "pesos[" << i << "] (" << pesos[i] << ")" << endl; // DEBUG

                                    _vectorEspacios.push_back(pesos[i]);  // Añadimos el dato al vector de espacios
                                    _vectorSolucion[i] = _vectorEspacios.size() - 1;  // El ID del ultimo contenedor creado es igual al tamaño del vector - 1
                                    _vectorEspacios[j] -= pesos[i];
                                    //cout << "Creado nuevo contenedor : " << _vectorEspacios.size() << endl; // DEBUG
                                    _nContenedores++;
                                }
                            }
                        }
                    }
                }
                _espacioLibre += capacidad - _vectorEspacios[j];
            }
            //}
        }

        // Comprueba si la solucion es factible
        bool Factible(uint16_t capacidad){
            if (_nContenedores != _vectorEspacios.size())
                return false;
            for (uint16_t j = 0; j < _vectorEspacios.size(); j++){
                if (_vectorEspacios[j] > capacidad)
                    return false;
            }

            return true;
        }

        // Comprueba si la solucion es factible y si el vector solucion es valido
        bool Factible(uint16_t capacidad, vector<uint16_t> pesos){
            vector<uint16_t> vEspTmp(_vectorEspacios.size() + 1);
            InitVector(vEspTmp);

            //cout << "Factibleeeeee" << endl; // DEBUG
            if (this->Factible(capacidad)){
                for (uint16_t i = 0; i < GetNumObjetos(); i++){
                    uint16_t j = _vectorSolucion[i];  // Contenedor donde esta 'i'
                    vEspTmp[j] += pesos[i];
                    if (vEspTmp[j] > capacidad){
                        //cout << vEspTmp[j] << " > " << capacidad << endl; // DEBUG
                        return false;
                    }
                }
                return true;
            }
            return false;

        }

        // Valor objetivo auxiliar. Promedio de los cuadrados de los huecos de los contenedores
        float ObjetivoAux(uint16_t capacidad){
            if (_nContenedores > 0){
                vector<uint16_t> espacios;  // Vector con los espacios libres en cada contenedor

                for (uint16_t i = 0; i < _vectorEspacios.size(); i++)
                    espacios.push_back(capacidad - _vectorEspacios[i]);

                return Sum(espacios) / _nContenedores;
            }
            return 0.0;
        }

        // GETTERS
        inline vector<uint16_t> GetVectorSolucion() { return _vectorSolucion; }
        inline vector<uint16_t> GetVectorEspacios() { return _vectorEspacios; }
        inline uint16_t GetNumContenedores() { return _nContenedores; }
        inline uint16_t GetNumObjetos() { return _vectorSolucion.size(); }
        inline uint16_t GetEspacioLibre() { return _espacioLibre; }
        // SETTERS
        inline void SetEspacioLibre(uint16_t s) { _espacioLibre = s; }

        void SetVectorSolucion(vector<uint16_t> &v) {
            _vectorSolucion.clear();
            for (uint16_t i = 0; i < v.size(); i++){
                _vectorSolucion.push_back(v[i]);
            }
        }

        void SetVectorEspacios(vector<uint16_t> &v) {
            _vectorEspacios.clear();
            for (uint16_t i = 0; i < v.size(); i++){
                _vectorEspacios.push_back(v[i]);
            }
        }

        void SetVectorEspacios(vector<Contenedor* > &v) {
            _vectorEspacios.clear();
            for (uint16_t i = 0; i < v.size(); i++){
                _vectorEspacios.push_back(v[i]->usedSpace);
            }
        }

        inline void SetNumContenedores(uint16_t n) { _nContenedores = n; }
        inline void SetSolucion(uint16_t sol, uint16_t pos) { _vectorSolucion[pos] = sol; }
        inline void SetEspacio(uint16_t esp, uint16_t pos) { _vectorEspacios[pos] = esp; }

        // SOBRECARGA DE OPERADORES
        friend ostream& operator << (ostream &o, Solucion &sol){
            if (VERBOSE){
                o << "Vector Solucion: [ ";
                for (uint16_t i = 0; i < sol.GetVectorSolucion().size(); i++)
                    o << sol.GetVectorSolucion()[i] << " ";
                o << "]" << endl;
                o << "Vector de Espacios: [ ";
                for (uint16_t i = 0; i < sol.GetVectorEspacios().size(); i++)
                    o << sol.GetVectorEspacios()[i] << " ";
                o << "]" << endl;
            }

            o << "Numero de contenedores: " << C_BRED << sol.GetNumContenedores() << endl;
            o << C_DEFAULT << "Espacio Libre: " << C_BRED << sol.GetEspacioLibre() << C_DEFAULT << endl;
            return o;
        }
        bool operator < (Solucion &sol){ return ( (_nContenedores < sol.GetNumContenedores()) ); }
        bool operator <= (Solucion &sol){ return ( (_nContenedores <= sol.GetNumContenedores()) ); }
        bool operator == (Solucion &sol){
        /*
             // Método estrictamente correcto de hacer el ==
            if ( (_nContenedores == sol.GetNumContenedores()) && (_espacioLibre == sol.GetEspacioLibre()) \
                && (_vectorSolucion.size() == sol.GetVectorEspacios().size()) && (_vectorEspacios.size() == sol.GetVectorEspacios().size()) ){

                    for (uint16_t i = 0; i < _vectorSolucion.size(); i++){
                        if (_vectorSolucion[i] != sol.GetVectorSolucion()[i])
                            return false;
                    }
                    for (uint16_t i = 0; i < _vectorEspacios.size(); i++){
                        if (_vectorEspacios[i] != sol.GetVectorEspacios()[i])
                            return false;
                    }

                    return true;
                }
            return false;
            */
            /*
            if ( (_nContenedores == sol.GetNumContenedores()) && (_espacioLibre == sol.GetEspacioLibre()) \
                && (_vectorSolucion.size() == sol.GetVectorEspacios().size()) && (_vectorEspacios.size() == sol.GetVectorEspacios().size()) ){
                    return true;
                }
                return false;
            */

            if (_nContenedores == sol.GetNumContenedores())
                return true;
            return false;

        }
        int operator - (Solucion &sol){ return ( (_nContenedores - sol.GetNumContenedores()) ); }
};
#endif
