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

#include <stdlib.h>
#include <string>
#include "aux.cpp"
#include "globalSearches.hpp"
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
        inline Instancia* GetInstancia(uint16_t pos) { return _instances->at(pos); }
        inline uint16_t GetNumeroInstancias() { return _instances->size(); }

        void RunHeuristic(string name, uint16_t ins){
            Solucion* solucion = NULL;
            if (ins < _nInstancias){
                if (name == "ILS"){
                    GlobalSearches* heu = new GlobalSearches(*GetInstancia(ins));
                    cout << C_BLUE << "-- ILS (Busqueda Local Iterada): " << C_DEFAULT << endl;
                    solucion = heu->ILS();
                }else if (name == "SA"){
                    GlobalSearches* heu = new GlobalSearches(*GetInstancia(ins));
                    cout << C_BLUE << "-- SA (Recocido Simulado): " << C_DEFAULT << endl;
                    solucion = heu->SA();
                }else if (name == "VNS"){
                    GlobalSearches* heu = new GlobalSearches(*GetInstancia(ins));
                    cout << C_BLUE << "-- VNS (Busqueda por Entorno Variable, basica): " << C_DEFAULT << endl;
                    solucion = heu->VNS();
                }else if (name == "TS"){
                    GlobalSearches* heu = new GlobalSearches(*GetInstancia(ins));
                    cout << C_BLUE << "-- TS: " << C_DEFAULT << endl;
                    solucion = heu->TS();
                }else if (name == "GRASP"){
                    Constructivas* heu = new Constructivas(*GetInstancia(ins));
                    cout << C_BLUE << "-- GRASP (Procedimiento de Busqueda Adaptativa Aleatoria Voraz): " << C_DEFAULT << endl;
                    solucion = heu->GRASP();
                }else if (name == "AG"){
                    Genetics* heu = new Genetics(*GetInstancia(ins));
                    cout << C_BLUE << "-- AG (Algoritmo Genetico): " << C_DEFAULT << endl;
                    solucion = heu->AG();
                    }else
                        return;  // 'name' INVALIDO
                if (solucion != NULL){
                    cout << *solucion << endl;
                }
            }
        }

        Solucion* RunHeuristicQuiet(string name, uint16_t ins){
            Solucion* solucion = NULL;
            if (ins < _nInstancias){
                if (name == "ILS"){
                    GlobalSearches* heu = new GlobalSearches(*GetInstancia(ins));
                    solucion = heu->ILS();
                }else if (name == "SA"){
                    GlobalSearches* heu = new GlobalSearches(*GetInstancia(ins));
                    solucion = heu->SA();
                }else if (name == "VNS"){
                    GlobalSearches* heu = new GlobalSearches(*GetInstancia(ins));
                    solucion = heu->VNS();
                }else if (name == "TS"){
                    GlobalSearches* heu = new GlobalSearches(*GetInstancia(ins));
                    solucion = heu->TS();
                }else if (name == "GRASP"){
                    Constructivas* heu = new Constructivas(*GetInstancia(ins));
                    solucion = heu->GRASP();
                }else if (name == "AG"){
                    Genetics* heu = new Genetics(*GetInstancia(ins));
                    solucion = heu->AG();
                }else
                    return NULL;  // 'name' INVALIDO
            }
            return solucion;
        }


        // Ejecuta todas las heuristicas para todas la instancia 'ins'
        // Si 'ins' es "-1" se ejecutan todas las instancias
        void RunAllHeuristics(int16_t ins = -1){
            if (ins < 0){ // Todas las instancias
                for (uint16_t i = 0; i < _nInstancias; i++){
                    cout << *_instances->at(i) << endl;
                    RunHeuristic("ILS", i);
                    RunHeuristic("SA", i);
                    RunHeuristic("VNS", i);
                    RunHeuristic("TS", i);
                    RunHeuristic("GRASP", i);
                    RunHeuristic("AG", i);
                }
            }else if (ins < _nInstancias){ // Ejecutar para la instancia 'ins'
                cout << *_instances->at(ins) << endl;
                RunHeuristic("ILS", ins);
                RunHeuristic("SA", ins);
                RunHeuristic("VNS", ins);
                RunHeuristic("TS", ins);
                RunHeuristic("GRASP", ins);
                RunHeuristic("AG", ins);
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
                        }else
                            enInstancia = true;
                        // Creamos nueva instancia con el nombre almacenado en 'buff' menos el primer caracter (SEPARADOR)
                        ins = new Instancia(buff.substr(1, buff.size() - 1));
                    // Línea con los datos de la instancia
                    } else if ((enInstancia) && (!haveData)) {
                        uint16_t j = 0; // Iterador de 'buff2'
                        uint16_t c = 0, n = 0, best = 0; // Datos de la instancia
                        while (i <= buff.size()){
                            // Copiar en Buffer 2
                            // Si no se trata de un SEPARADOR llenamos 'buff2'
                            if ((buff[i] != SEPARADOR)&&(i < buff.size())){
                                buff2[j] = buff[i];
                                j++;
                            // Usar el número leído
                            }else if (!IsEmpty(buff2, BUFFSIZE)){
                                if ( c == 0 ){
                                    c = atoi(buff2);
                                    ins->SetCapacidadC(c);
                                    ResetChar (buff2, BUFFSIZE);
                                    j = 0;
                                }else if ( n == 0 ){
                                    n = atoi(buff2);
                                    ins->SetNumObjetos(n);
                                    ResetChar (buff2, BUFFSIZE);
                                    j = 0;
                                }else if ( best == 0 ){
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
                    }
                }
            }
        }

};
#endif
