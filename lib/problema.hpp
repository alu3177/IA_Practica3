#include <fstream>
#include <stdlib.h>

#include "localSearches.hpp"

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
        inline Instancia* GetInstancia(uint16_t pos) { return _instances->at(pos); }

        // Métodos
        void ParseFile (const char* path); // Lee el fichero de entrada y carga los datos del problema
        //inline void AddInstancia(Instancia* i) { _instances->push_back(i); }


        // LLAMADAS A LOS DIFERENTES ALGORITMOS (que están encapsulados en clases)
        // 'ins' permite ejecutar los algoritmos para una sola instancia.
        void BusquedasLocales(int32_t ins = -1){
            // Ejecutar para todas las instancias cargadas
            if (ins < 0){
                //cout << "Numero de instancias: " << _nInstancias << endl;     // DEBUG
                LocalSearches* local;
                Solucion* solucion;
                for (uint16_t i = 0; i < _nInstancias; i++){
                    //cout << "Instancia " << i << "          (" << C_GREEN << _instances->at(i)->GetMejorNumContenedores() << C_DEFAULT << ")" << endl;
                    cout << *_instances->at(i) << endl;
                    local = new LocalSearches(*_instances->at(i));

                    solucion = local->ILS();
                    cout << "-- ILS 1: " << endl << *solucion;

                    solucion = local->ILS2();
                    cout << "-- ILS 2: " << endl << *solucion;

                    solucion = local->SA();
                    cout << "-- SA 1: " << endl << *solucion;

                    solucion = local->SA2();
                    cout << "-- SA 2: " << endl << *solucion;

                    solucion = local->VNS();
                    cout << "-- VNS 1: " << endl << *solucion << endl;

                }
            }else{
                LocalSearches* local = new LocalSearches(*_instances->at(ins));
                Solucion* solucion = local->ILS();
                cout << "Instancia " << ins << ", solución: " << endl << solucion << endl;
            }
        }

};

#endif
