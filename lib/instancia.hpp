#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>

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
        // Constructor
        Instancia(string nam = "", uint16_t c = 0, uint16_t n = 0, uint16_t best = 0, vector<uint16_t>* w = NULL) : _name(nam), _capacidadC(c), _nObjetos(n), _mejorNumCont(best) {
            if (w != NULL)
                _pesos = w;
            else
                _pesos = new vector<uint16_t>;
        }

        // Métodos
        inline void AddPeso(uint16_t w) { _pesos->push_back(w); }

        // GETTERS y SETTERS
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

        // SobreCarga de Operadores
        friend ostream& operator << (ostream &o, Instancia &ins){
            o << "Instancia:                     " << ins.GetName() << endl;
            o << "Capacidad de los contenedores: " << ins.GetCapacidadC() << endl;
            o << "Número de objetos:             " << ins.GetNumObjetos() << endl;
            o << "Mejor número de contenedores:  " << ins.GetMejorNumContenedores() << endl;
            for (uint16_t i = 0; i < ins.GetPesos()->size(); i++){
                o << "[" << i << "] " << ins.GetPesos()->at(i) << endl;
            }
            return o;
        }

        friend bool operator == (Instancia &p, Instancia &p2){
            return p2.GetName() == p.GetName() && p2.GetCapacidadC() == p.GetCapacidadC() && p2.GetMejorNumContenedores() == p.GetMejorNumContenedores() && p2.GetMejorNumContenedores() == p.GetMejorNumContenedores();
        }

        Instancia& operator = (Instancia &p){
            this->_name = p.GetName();
            this->_capacidadC = p.GetCapacidadC();
            this->_nObjetos = p.GetNumObjetos();
            this->_mejorNumCont = p.GetMejorNumContenedores();
            this->_pesos = p.GetPesos();
            return *this;
        }   

};

#endif
