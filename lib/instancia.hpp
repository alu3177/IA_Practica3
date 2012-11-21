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
#include <iostream>
#include <string>
#include <vector>
#include "solucion.hpp"

using namespace std;

#ifndef INSTANCIA_H
#define INSTANCIA_H

class Instancia {
    protected:
        string _name;            // Nombre de la instancia
        uint16_t _capacidadC;    // Capacidad de cada contenedor (C)
        uint16_t _nObjetos;      // Número de objetos (n)
        uint16_t _mejorNumCont;  // Mejor número de contenedores
        vector<uint16_t>* _pesos; // Puntero a, vector con los pesos de los elementos (El elemento 'i' pesa '_pesos[i]')
    public:
        /*
         * CONSTRUCTOR
         */
        Instancia(string nam = "", uint16_t c = 0, uint16_t n = 0, uint16_t best = 0, vector<uint16_t>* w = NULL) : _name(nam), _capacidadC(c), _nObjetos(n), _mejorNumCont(best) {
            if (w != NULL)
                _pesos = w;
            else
                _pesos = new vector<uint16_t>;
        }

        /*
         * GETTERS & SETTERS
         */
        inline string GetName() { return _name; }
        inline uint16_t GetCapacidadC() { return _capacidadC; }
        inline uint16_t GetNumObjetos() { return _nObjetos; }
        inline uint16_t GetMejorNumContenedores() { return _mejorNumCont; }
        inline vector<uint16_t>* GetPesos() { return _pesos; }
        inline uint16_t GetPeso(uint16_t i) { return _pesos->at(i); }

        inline void SetName (string n) { _name = n; }
        inline void SetCapacidadC (uint16_t c) { _capacidadC = c; }
        inline void SetNumObjetos (uint16_t n) { _nObjetos = n; }
        inline void SetMejorNumContenedores (uint16_t best) { _mejorNumCont = best; }
        inline void SetPesos(vector<uint16_t>* w) { _pesos = w; }
        inline void SetPesos(vector<uint16_t> w) {
            _pesos->clear();
            for (uint16_t i = 0; i < w.size(); i++)
                _pesos->push_back(w[i]);
        }

        /*
         * METODOS
         */
        inline void AddPeso(uint16_t w) { _pesos->push_back(w); }

        /*
         * SOBRECARGA DE OPERADORES
         */
        friend ostream& operator << (ostream &o, Instancia &ins){
            o << "Instancia:                     " << C_BGREEN << ins.GetName() << C_DEFAULT << endl;
            o << "Capacidad de los contenedores: " << ins.GetCapacidadC() << endl;
            o << "Número de objetos:             " << ins.GetNumObjetos() << endl;
            o << "Mejor número de contenedores:  " << C_GREEN << ins.GetMejorNumContenedores() << C_DEFAULT << endl;
            if (VERBOSE){
                for (uint16_t i = 0; i < ins.GetPesos()->size(); i++){
                    o << "[" << i << "]" << C_CYAN << ins.GetPesos()->at(i) << C_DEFAULT;
                    if (i < ins.GetPesos()->size() - 1)
                        o << ", ";
                }
            }
            return o;
        }
};
#endif
