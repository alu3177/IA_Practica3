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

#include "constructivas.hpp"

// Metodo utilizado para ordenar de menor a mayor (espacio dejado) candidatos en std::sort
bool CompareCandidato (Candidato c1, Candidato c2){
    return (c1.freeSpace < c2.freeSpace);
}

Solucion* Constructivas::GRASP (uint16_t t){
    vector<Contenedor* > contenedores;
    vector<uint16_t> pesos = *_instance.GetPesos();
    vector<uint16_t> solucion(_instance.GetNumObjetos()); // Vector solucion inicializado al numero de objetos de la instancia
    vector<Candidato> lrc;
    uint16_t sumPesos = 0;

    srand ( time(NULL) + rand() );   // Inicializamos la semilla del RANDOM
    while (!FullyExplored(pesos)){
        lrc = GetListaRestringidaCandidatos(pesos, contenedores, t);
        if (lrc.size() > 0){
            // Elegimos un elemento aleatorio de LRC
            uint16_t i = rand() % lrc.size();
            Candidato paso = lrc[i];
            solucion[paso.oID] = paso.cID;
            contenedores[paso.cID]->usedSpace += pesos[paso.oID];
            sumPesos += pesos[paso.oID]; // Incrementamos el peso usado
            pesos[paso.oID] = EXPLOREDWEIGHT;  // "Sacamos" del vector de pesos al objeto 'paso.oID'
        }else{  // LRC vacía, significa que no hubo hueco suficiente para realizar ningun paso => Creamos contenedor
            Contenedor *cont = new Contenedor;
            cont->id = contenedores.size();
            cont->c = _instance.GetCapacidadC();
            cont->usedSpace = 0;
            contenedores.push_back(cont);
        }
    }

    uint16_t freeSpace = (contenedores.size() * _instance.GetCapacidadC()) - sumPesos; // Establecemos el espacio libre (Total - sumPesos)
    Solucion* result = new Solucion(solucion, contenedores, freeSpace);

    return result;
}

// Obtiene la Lista Restringida de Candidatos
// Devuelve una lista con los 't' mejores pasos candidatos
vector<Candidato> Constructivas::GetListaRestringidaCandidatos(vector<uint16_t> &pesos, vector<Contenedor* > &contenedores, uint16_t t){
    vector<Candidato> candidatos;  // Vector con todos los pasos candidatos

    // Construccion de candidatos para todo 'i' != 'EXPLOREDWEIGHT'
    for (uint16_t i = 0; i < pesos.size(); i++){  // Recorremos objetos
        bool insertado = false;  // Indica si se ha insertado en un contenedor
        Candidato bestCandidato;  // Mejor paso candidato para el objeto 'i'
        bestCandidato.oID = i;
        bestCandidato.freeSpace = _instance.GetCapacidadC();
        if (pesos[i] != EXPLOREDWEIGHT){  // Nos aseguramos de comprobar sólo para objetos sin contenedor asociado
            for (uint16_t j = 0; j < contenedores.size(); j++){ // Recorremos contenedores
                uint16_t freespace = contenedores[j]->c - contenedores[j]->usedSpace; // Espacio libre en 'j' actualmente
                if (pesos[i] <= freespace){  // Paso factible ('i', cabe en 'j')
                    if ((pesos[i] - freespace) < bestCandidato.freeSpace){  // Mejoramos el mejor paso anterior
                        bestCandidato.cID = j;
                        bestCandidato.freeSpace = freespace - pesos[i];
                        insertado = true;
                    }
                }
            }
            if (insertado){  // Si se encontró un contenedor donde mover a 'i' ...
                candidatos.push_back(bestCandidato);
            }
        }
    }
    // Ordenamos de menor a mayor y nos quedamos con los 't' primeros
    sort(candidatos.begin(), candidatos.end(), CompareCandidato);
    vector<Candidato> result;
    for (uint16_t i = 0; i < candidatos.size(); i++){
        if (i < t)
            result.push_back(candidatos[i]);
        else
            break;
    }
    return result;
}
