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

/*
    // PRUEBAS CANDIDATOS
    #include <algorithm>
    // Representa cada uno de los pasos candidatos
    struct Candidato{
        uint16_t oID;  // ID del objeto candidato
        uint16_t cID;  // ID del contendor candidato
        uint16_t freeSpace;  // Espacio libre en 'cID' tras insertar 'oID'
    };
    // Metodo utilizado para ordenar candidatos con mediante std::sort
    bool CompareCandidato (Candidato c1, Candidato c2){
        return (c1.freeSpace > c2.freeSpace);
    }
*/

#include "lib/constructivas.hpp"

int main (){
/*
    // PRUEBAS CANDIDATOS
    vector<Candidato> cands;
    for (uint16_t i = 0; i < 15; i++){
        Candidato c;
        c.oID = i;
        c.cID = i+10;
        c.freeSpace = rand() % 275;
        cands.push_back(c);

        cout << "[" << i << "] - " << c.freeSpace << endl;
    }
    sort(cands.begin(), cands.end(), CompareCandidato);
    cout << endl << endl;
    for (uint16_t i = 0; i < cands.size(); i++){
        cout << "[" << cands[i].oID << "] - " << cands[i].freeSpace << endl;
    }
*/


    Problema* prob = new Problema();
    prob->ParseFile("data/BPP10.txt");
    prob->BusquedasLocales();
    //prob->HeuristicasConstructivas();


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

    Constructivas* cons = new Constructivas(inn);
    cout << *cons->GRASP(15) << endl;
*/
/*
    cout << *GeneraSolucionPrimeroQuepa(&inn) << endl;
    cout << *GeneraSolucionPrimeroQuepa(&inn, true) << endl;
    cout << *GeneraSolucionInicialRandom(&inn) << endl;
*/



    return 0;
}

