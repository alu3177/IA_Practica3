/*
 * Inteligencia Artificial - Práctica 3: Problema de empaquetado unidimensional
 *
 * AUTOR: Fernando González López-Peñalver (alu3177)
 * EMAIL: alu0100256543@ull.edu.es
 * FECHA DE ENTREGA: 22 - 11 -12
 *
 * Se incluye repositorio GIT con el desarrollo de la práctica.
 *
 * Enunciado del problema:
 *      Se dispone de una cantidad indeterminada de contenedores de
 *      capacidad C y n objetos de tamaños w1, w2, …, wn. Se trata de
 *      determinar el empaquetado de todos los objetos usando el menor
 *      número posible de contenedores m*. La suma de los tamaños de los
 *      objetos asignados a contenedor no puede sobrepasar la capacidad C.
 *
 *      Implementar los siguientes algoritmos:
 *          - ILS   (Busqueda Local Iterada)
 *          - SA    (Recocido Simulado)
 *          - VNS   (Busqueda por Entorno Variable, basica)
 *          - TS    (Busqueda Tabu)
 *          - GRASP (Procedimiento de Busqueda Adaptativa Aleatoria Voraz)
 *          - AG    (Algoritmo Genetico)
 *
 */

#include <stdint.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <math.h>
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
        // Realiza un procedimiento voraz para generarla
        Solucion(vector<uint16_t> vSol, uint16_t capacidad, vector<uint16_t> pesos){
            if (vSol.size() > 0){
                SetVectorSolucion (vSol);
            }
            // Obtenemos el numero de contenedores
            _nContenedores = 0;
            _espacioLibre = 0;
            vector<uint16_t> contenedores;  // Vector con las IDs de los contenedores extraidas de vSol
            for (uint16_t i = 0; i < GetNumObjetos(); i++){
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
            for (uint16_t j = 0; j < contenedores.size(); j++){ // Recorremos contenedores
                uint16_t usedSpaceCont = 0;  // Espacio usado en el contenedor actual 'j'
                for (uint16_t i = 0; i < _vectorSolucion.size(); i++){  // Recorremos objetos
                    if (_vectorSolucion[i] == j){  // Objeto 'i' esta en contenedor 'j'?
                        usedSpaceCont += pesos[i];
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
            _espacioLibre = 0;
            for (uint16_t j = 0; j < GetNumContenedores(); j++){  // Recorremos contenedores (en el vector de espacios)
                if(_vectorEspacios[j] > capacidad){  // Se produce SobreCarga del contenedor 'j'
                    uint16_t nCap = 0;  // Nueva capacidad del contenedor
                    for (uint16_t i = 0; i < GetNumObjetos(); i++){  // Recorremos el vector solucion buscando los asociados a 'j'
                        if (_vectorSolucion[i] == j){  // 'i' esta en 'j'
                            if ((nCap + pesos[i]) <= capacidad){  // Objeto 'i' cabe en 'j'
                                nCap += pesos[i];
                            }else{  // Objeto 'i' no cabe en 'j'
                                if ((capacidad - GetMenor(_vectorEspacios)) >= pesos[i]){  // ¿Cabe en algun otro contenedor?
                                    _vectorSolucion[i] = GetPosicionMenor(_vectorEspacios);
                                    _vectorEspacios[GetPosicionMenor(_vectorEspacios)] += pesos[i];
                                    _vectorEspacios[j] -= pesos[i];

                                }else{ // No cabe en ningun contenedor => Creamos uno
                                    _vectorEspacios.push_back(pesos[i]);  // Añadimos el dato al vector de espacios
                                    _vectorSolucion[i] = _vectorEspacios.size() - 1;  // El ID del ultimo contenedor creado es igual al tamaño del vector - 1
                                    _vectorEspacios[j] -= pesos[i];
                                    _nContenedores++;
                                }
                            }
                        }
                    }
                }
                _espacioLibre += capacidad - _vectorEspacios[j];
            }
        }

        // Comprueba si la solucion es factible
        bool Factible(uint16_t capacidad){
            if (_nContenedores != _vectorEspacios.size()) // '_nContenedores' concuerda
                return false;
            for (uint16_t j = 0; j < GetNumContenedores(); j++){ //  Ningun contenedor sobrepasa 'capacidad'
                if (_vectorEspacios[j] > capacidad)
                    return false;
            }
            return true;
        }

        // Comprueba si la solucion es factible y si el vector solucion es valido
        bool Factible(uint16_t capacidad, vector<uint16_t> pesos){
            vector<uint16_t> vEspTmp(_vectorEspacios.size() + 1);
            InitVector(vEspTmp);

            if (this->Factible(capacidad)){
                for (uint16_t i = 0; i < GetNumObjetos(); i++){
                    uint16_t j = _vectorSolucion[i];  // Contenedor donde esta 'i'
                    vEspTmp[j] += pesos[i];
                    if (vEspTmp[j] > capacidad){
                        return false;
                    }
                }
                return true;
            }
            return false;

        }

        // Valor objetivo
        uint16_t Objetivo(){
            return GetNumContenedores();
        }

        // Valor objetivo auxiliar. Promedio de los cuadrados de los huecos de los contenedores
        double ObjetivoAux(uint16_t capacidad){
            if (_nContenedores > 0){
                vector<uint16_t> espacios;  // Vector con los espacios libres en cada contenedor

                for (uint16_t i = 0; i < _vectorEspacios.size(); i++){
                    espacios.push_back(capacidad - _vectorEspacios[i]);
                }
                return (pow(Sum(espacios), 2) / _nContenedores);
            }
            return 0.0;
        }

        // GETTERS
        inline vector<uint16_t> GetVectorSolucion() { return _vectorSolucion; }
        inline uint16_t VectorSolucion(uint16_t i) { return _vectorSolucion[i]; }
        inline vector<uint16_t> GetVectorEspacios() { return _vectorEspacios; }
        inline uint16_t VectorEspacios(uint16_t j) { return _vectorEspacios[j]; }
        inline uint16_t GetNumContenedores() { return _vectorEspacios.size(); }
        inline uint16_t GetNumObjetos() { return _vectorSolucion.size(); }
        inline uint16_t GetEspacioLibre() { return _espacioLibre; }
        // SETTERS
        inline void SetEspacioLibre(uint16_t s) { _espacioLibre = s; }
        inline void SetNumContenedores(uint16_t n) { _nContenedores = n; }
        inline void SetSolucion(uint16_t sol, uint16_t pos) { _vectorSolucion[pos] = sol; }
        inline void SetEspacio(uint16_t esp, uint16_t pos) { _vectorEspacios[pos] = esp; }

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
        int operator - (Solucion &sol){ return ( (_nContenedores - sol.GetNumContenedores()) ); }
};
#endif
