/*
 * Inteligencia Artificial - Práctica 3: Problema de empaquetado unidimensional
 *
 * AUTOR: Fernando González López-Peñalver (alu3177)
 * EMAIL: alu0100256543@ull.edu.es
 * ACCESO WEB AL REPOSITORIO DEL DESARROLLO: https://github.com/alu3177/IA_Practica3
 *
 * Fecha de entrega:
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
#include "lib/localSearches.hpp"
#include "lib/generadores.hpp"  // Contiene los métodos generadores de soluciones.

using namespace std;

int main (){

    Problema* prob = new Problema();
    prob->ParseFile("data/BPP10.txt");
    prob->BusquedasLocales();


    //cout << *GeneraSolucionPrimeroQuepa(prob->GetInstancia(1)) << endl;
    //cout << *GeneraSolucionPrimeroQuepa(prob->GetInstancia(1), true) << endl;
    //cout << *GeneraSolucionInicialRandom(prob->GetInstancia(1)) << endl;


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
    local->ILS();
*/
/*
    cout << *GeneraSolucionPrimeroQuepa(&inn) << endl;
    cout << *GeneraSolucionPrimeroQuepa(&inn, true) << endl;
    cout << *GeneraSolucionInicialRandom(&inn) << endl;
*/
    return 0;
}

