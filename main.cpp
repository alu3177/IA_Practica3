/*
 * Inteligencia Artificial - Práctica 3: Problema de empaquetado unidimensional
 *
 * AUTOR: Fernando González López-Peñalver (alu3177)
 * EMAIL: alu0100256543@ull.edu.es
 * ACCESO WEB AL REPOSITORIO DEL DESARROLLO: https://github.com/alu3177/IA_Practica3
 *
 * Fecha de entrega: 22-11-2012
 *
 * Enunciado del problema:
 *      Se dispone de una cantidad indeterminada de contenedores de
 *      capacidad C y n objetos de tamaños w1, w2, …, wn. Se trata de
 *      determinar el empaquetado de todos los objetos usando el menor
 *      número posible de contenedores m*. La suma de los tamaños de los
 *      objetos asignados a contenedor no puede sobrepasar la capacidad C.
 *
 */
#include "lib/problema.hpp"

int main (){

    Problema* prob = new Problema();
    prob->ParseFile("data/BPP10.txt");
    //prob->BusquedasLocales();
    //prob->HeuristicasConstructivas();
    //prob->Geneticas();
    prob->RunAllHeuristics();

/*
    // Creamos el problema de las transparencias:
    vector<uint16_t>* w = new vector<uint16_t>;

    w->push_back(3);
    w->push_back(6);
    w->push_back(2);
    w->push_back(1);
    w->push_back(5);
    w->push_back(7);
    w->push_back(2);
    w->push_back(4);
    w->push_back(1);
    w->push_back(9);

    Instancia* ins = new Instancia("Transparencias", 10, 10, 0, w);
    cout << *ins << endl;
    Instancia inn = *ins;
    cout << inn << endl;

    LocalSearches* local = new LocalSearches(inn);
    cout << *local->SA() << endl;
*/

    return 0;
}

