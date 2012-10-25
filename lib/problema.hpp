#include <fstream>
#include <stdlib.h>

#include "instancia.hpp"

#ifndef PROBLEMA_H
#define PROBLEMA_H

using namespace std;

#define BUFFSIZE 16     // Tamaño del buffer de lectura de ficheros
#define SEPARADOR ' '   // Separador de argumentos en el fichero de entrada

class Problema {
    protected:
        uint16_t _nInstancias;            // Número de instancias
        vector<Instancia* >* _instances; // Puntero a, vector con punteros a las distintas instancias

    public:
        // Constructor
        Problema(uint16_t nI = 0, vector<Instancia* >* ins = NULL) : _nInstancias(nI) {
            if (ins != NULL)
                _instances = ins;
            else
                _instances = new vector<Instancia* >;
        }

        // Getters y Setters

        //inline void SetNumInstancias (uint16_t nI) { _nInstancias = nI; }

        // Métodos
        void ParseFile (const char* path); // Lee el fichero de entrada y carga los datos del problema
        //inline void AddInstancia(Instancia* i) { _instances->push_back(i); }

};

#endif
