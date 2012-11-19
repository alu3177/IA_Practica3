#include <stdlib.h>
#include "aux.cpp"
#include "localSearches.hpp"
#include "constructivas.hpp"
#include "genetics.hpp"

using namespace std;

#ifndef PROBLEMA_H
#define PROBLEMA_H

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

        // LLAMADAS A LOS DIFERENTES ALGORITMOS (que están encapsulados en clases)
        // 'ins' permite ejecutar los algoritmos para una sola instancia.
        void BusquedasLocales(int16_t ins = -1){
            LocalSearches* local;
            Solucion* solucion;
            // Ejecutar para todas las instancias cargadas
            if (ins < 0){
                //cout << "Numero de instancias: " << _nInstancias << endl;     // DEBUG
                for (uint16_t i = 0; i < _nInstancias; i++){
                    cout << *_instances->at(i) << endl;
                    local = new LocalSearches(*_instances->at(i));

                    cout << "-- ILS 1: " << endl;
                    solucion = local->ILS();
                    cout << *solucion;

                    cout << "-- ILS 2: " << endl;
                    solucion = local->ILS2();
                    cout << *solucion;

                    cout << "-- SA 1: " << endl;
                    solucion = local->SA();
                    cout << *solucion;

                    cout << "-- SA 2: " << endl;
                    solucion = local->SA2();
                    cout << *solucion;

                    cout << "-- VNS 1: " << endl;
                    solucion = local->VNS();
                    cout << *solucion;

                    cout << "-- TS 1: " << endl;
                    solucion = local->TS(50);
                    cout << *solucion << endl;

                }
            }else{
                local = new LocalSearches(*_instances->at(ins));
                cout << *_instances->at(ins) << endl;

                    cout << "-- ILS 1: " << endl;
                    solucion = local->ILS();
                    cout << *solucion;

                    cout << "-- ILS 2: " << endl;
                    solucion = local->ILS2();
                    cout << *solucion;

                    cout << "-- SA 1: " << endl;
                    solucion = local->SA();
                    cout << *solucion;

                    cout << "-- SA 2: " << endl;
                    solucion = local->SA2();
                    cout << *solucion;

                    cout << "-- VNS 1: " << endl;
                    solucion = local->VNS();
                    cout << *solucion;

                    cout << "-- TS 1: " << endl;
                    solucion = local->TS(25);
                    cout << *solucion << endl;
            }
        }

        void HeuristicasConstructivas(int16_t ins = -1){
            Constructivas* cons;
            Solucion* solucion;
            // Ejecutar para todas las instancias cargadas
            if (ins < 0){
                for (uint16_t i = 0; i < _nInstancias; i++){
                    cout << *_instances->at(i) << endl;
                    cons = new Constructivas(*_instances->at(i));

                    cout << "-- GRASP: " << endl;
                    solucion = cons->GRASP();
                    cout << *solucion << endl;
                }
            }else{
                cons = new Constructivas(*_instances->at(ins));

                cout << "-- GRASP: " << endl;
                solucion = cons->GRASP();
                cout << *solucion;
            }
        }

        void Geneticas(int16_t ins = -1){
            Genetics* cons;
            Solucion* solucion;
            // Ejecutar para todas las instancias cargadas
            if (ins < 0){
                for (uint16_t i = 0; i < _nInstancias; i++){
                    cout << *_instances->at(i) << endl;
                    cons = new Genetics(*_instances->at(i));

                    cout << "-- AG: " << endl;
                    solucion = cons->AG();
                    cout << *solucion << endl;
                }
            }else{
                cons = new Genetics(*_instances->at(ins));

                cout << "-- AG: " << endl;
                solucion = cons->AG();
                cout << *solucion;
            }
        }

        void RunAllHeuristics(int16_t ins = -1){
            LocalSearches* local;
            Constructivas* cons;
            Genetics* gen;
            Solucion* solucion;
            // Ejecutar para todas las instancias cargadas
            if (ins < 0){
                for (uint16_t i = 0; i < _nInstancias; i++){
                    cout << *_instances->at(i) << endl;
                    local = new LocalSearches(*_instances->at(i));
                    cons = new Constructivas(*_instances->at(i));
                    gen = new Genetics(*_instances->at(i));

                    cout << C_BLUE << "-- ILS 1: " << C_DEFAULT << endl;
                    solucion = local->ILS();
                    cout << *solucion;
                    delete(solucion);

                    cout << C_BLUE << "-- ILS 2: " << C_DEFAULT << endl;
                    solucion = local->ILS2();
                    cout << *solucion;
                    delete(solucion);

                    cout << C_BLUE << "-- SA 1: " << C_DEFAULT << endl;
                    solucion = local->SA();
                    cout << *solucion;
                    delete(solucion);

                    cout << C_BLUE << "-- SA 2: " << C_DEFAULT << endl;
                    solucion = local->SA2();
                    cout << *solucion;
                    delete(solucion);

                    cout << C_BLUE << "-- VNS 1: " << C_DEFAULT << endl;
                    solucion = local->VNS();
                    cout << *solucion;
                    delete(solucion);

                    cout << C_BLUE << "-- TS 1: " << C_DEFAULT << endl;
                    solucion = local->TS();
                    cout << *solucion;
                    delete(solucion);

                    cout << C_BLUE << "-- GRASP: " << C_DEFAULT << endl;
                    solucion = cons->GRASP();
                    cout << *solucion;
                    delete(solucion);

                    cout << C_BLUE << "-- AG: " << C_DEFAULT << endl;
                    solucion = gen->AG();
                    cout << *solucion << endl;
                    delete(solucion);

                    delete(local);
                    delete(cons);
                    delete(gen);

                }
            }else{
                cout << *_instances->at(ins) << endl;
                local = new LocalSearches(*_instances->at(ins));
                cons = new Constructivas(*_instances->at(ins));
                gen = new Genetics(*_instances->at(ins));

                cout << C_BLUE << "-- ILS 1: " << C_DEFAULT << endl;
                solucion = local->ILS();
                cout << *solucion;

                cout << C_BLUE << "-- ILS 2: " << C_DEFAULT << endl;
                solucion = local->ILS2();
                cout << *solucion;

                cout << C_BLUE << "-- SA 1: " << C_DEFAULT << endl;
                solucion = local->SA();
                cout << *solucion;

                cout << C_BLUE << "-- SA 2: " << C_DEFAULT << endl;
                solucion = local->SA2();
                cout << *solucion;

                cout << C_BLUE << "-- VNS 1: " << C_DEFAULT << endl;
                solucion = local->VNS();
                cout << *solucion;

                cout << C_BLUE << "-- TS 1: " << C_DEFAULT << endl;
                solucion = local->TS();
                cout << *solucion;

                cout << C_BLUE << "-- GRASP: " << C_DEFAULT << endl;
                solucion = cons->GRASP();
                cout << *solucion;

                cout << C_BLUE << "-- AG: " << C_DEFAULT << endl;
                solucion = gen->AG();
                cout << *solucion << endl;
            }
        }

        // Carga los todos los datos necesarios desde el fichero situado en 'path'
        void ParseFile (const char* path){
            fstream file;
            char *buff2 = new char[BUFFSIZE];
            string buff;
            bool haveInstances = false; // Indica si ya hemos leido el numero de instancias que tiene el fichero
            bool enInstancia = false;   // Indica si estamos leyendo datos "dentro de una instancia"
            bool haveData = false;      // Indica si hemos leido los datos de la instancia actual
            Instancia* ins;     // Puntero usado para crear (y añadir al problema) nuevas instancias

            file.open(path, fstream::in);
            if (file.is_open()){
                // Iteramos por el fichero mientras podamos
                while (file.good()){
                    uint16_t i = 0;    // Iterador del Buffer 'buff'
                    getline(file, buff);

                    // Obtención del numero de instancias
                    if (!haveInstances){
                        while (i < buff.size()){
                            buff2[i] = buff[i];
                            i++;
                        }
                        _nInstancias = atoi(buff2);

                        //cout << "_nInstancias = " << _nInstancias << endl;   // DEBUG
                        //cout << "buff = " << buff << endl;   // DEBUG

                        haveInstances = true;
                        buff.clear();
                        ResetChar(buff2, BUFFSIZE);
                        //i = 0;

                    // Línea de nueva Instancia
                    } else if (buff[0] == SEPARADOR) {
                        if (enInstancia){
                            // Si ya estábamos leyendo otra instancia, la almacenamos antes de crear una nueva
                            _instances->push_back(ins);
                            haveData = false; // No disponemos de los datos de la nueva instancia
                            //cout << *ins << endl;   // DEBUG (mostramos la instancia generada)
                        }else
                            enInstancia = true;
                        // Creamos nueva instancia con el nombre almacenado en 'buff' menos el primer caracter (SEPARADOR)
                        ins = new Instancia(buff.substr(1, buff.size() - 1));
                        //cout << "ins->GetName() = " << ins->GetName() << endl;  // DEBUG
                        //cout << "Instancia: " << buff << endl;  // DEBUG

                    // Línea con los datos de la instancia
                    } else if ((enInstancia) && (!haveData)) {
                        //uint16_t i = 0; // Iterador de 'buff'
                        uint16_t j = 0; // Iterador de 'buff2'
                        uint16_t c = 0, n = 0, best = 0; // Datos de la instancia
                        while (i <= buff.size()){
                            // Copiar en Buffer 2
                            // Si no se trata de un SEPARADOR llenamos 'buff2'
                            if ((buff[i] != SEPARADOR)&&(i < buff.size())){
                                buff2[j] = buff[i];
                                j++;
                            // Usar el número leído
                            }else if ((!IsEmpty(buff2, BUFFSIZE))/* || (i >= buff.size() - 1)*/){
                                //cout << "buff2 : " << buff2 << endl;  // DEBUG
                                if ( c == 0 ){
                                    //cout << " c << " << buff2 << endl;   // DEBUG
                                    c = atoi(buff2);
                                    ins->SetCapacidadC(c);
                                    ResetChar (buff2, BUFFSIZE);
                                    j = 0;
                                }else if ( n == 0 ){
                                    //cout << " n << " << buff2 << endl;   // DEBUG
                                    n = atoi(buff2);
                                    ins->SetNumObjetos(n);
                                    ResetChar (buff2, BUFFSIZE);
                                    j = 0;
                                }else if ( best == 0 ){
                                    //cout << " best << " << buff2 << endl;   // DEBUG
                                    best = atoi(buff2);
                                    ins->SetMejorNumContenedores(best);
                                    ResetChar (buff2, BUFFSIZE);
                                    j = 0;
                                }
                            }
                            i++;
                        }
                        haveData = true;

                    // Línea con el peso de un componente
                    } else if ((enInstancia) && (haveData)) {
                        // Copiamos a 'buff2'
                        while (i < buff.size()){
                            buff2[i] = buff[i];
                            i++;
                        }
                        ins->AddPeso(atoi(buff2));
                        //cout << "w = " << atoi(buff2) << endl;  // DEBUG
                    }
                }
                //cout << *ins << endl;   // DEBUG (mostramos la última instancia generada)
            }
        }

};
#endif
