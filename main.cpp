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

#include "lib/problema.hpp"

void Menu(Problema* prob){
    uint16_t opcion = 999;  // Opcion del menu seleccionada
    int ins;                // Instancia seleccionada
    string heu;             // Heuristica seleccionada
    uint16_t veces;         // Numero de veces a ejecutar una heuristica
    Solucion* sol;
    while (opcion != 0){
        system("clear");
        cout << "#############################################" << endl;
        cout << "# " << C_RED << "Tarea 3" << C_DEFAULT << "                                   #" << endl;
        cout << "#   Problema de empaquetado unidimensional  #" << endl;
        cout << "#############################################" << endl;
        cout << "#  1. Ejecutar una heuristica en concreto.  #" << endl;
        cout << "#  2. Ejecutar todas las heuristicas.       #" << endl;
        cout << "#  3. Ejecutar una heuristica 'n' veces.    #" << endl;
        cout << "#                                           #" << endl;
        cout << "#  0. SALIR                                 #" << endl;
        cout << "#############################################" << endl << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        switch(opcion){
            case 1:
                system("clear");
                cout << C_RED << "ILS, SA, VNS, TS, GRASP, AG" << C_DEFAULT << endl << "Escriba una heuristica de las posibles (case sensitive): ";
                cin >> heu;
                cout << "Seleccione una instancia [0, " << prob->GetNumeroInstancias() - 1 << "] (-1 para selecionar todas): ";
                cin >> ins;
                if (ins >= 0){
                    cout << *prob->GetInstancia(ins) << endl;
                    prob->RunHeuristic(heu, ins);
                }else{
                    for (uint16_t i = 0; i < prob->GetNumeroInstancias(); i++){
                        cout << *prob->GetInstancia(i) << endl;
                        prob->RunHeuristic(heu, i);
                    }

                }
                cout << "Pulse enter para continuar..." << endl;
                cin.ignore().get();
                break;

            case 2:
                system("clear");
                cout << "Seleccione una instancia [0, " << prob->GetNumeroInstancias() - 1 << "] (-1 para selecionar todas): ";
                cin >> ins;
                prob->RunAllHeuristics(ins);
                cout << "Pulse enter para continuar..." << endl;
                cin.ignore().get();
                break;

            case 3:
                system("clear");
                cout << C_RED << "ILS, SA, VNS, TS, GRASP, AG" << C_DEFAULT << endl << "Escriba una heuristica de las posibles (case sensitive): ";
                cin >> heu;
                cout << "Seleccione una instancia [0, " << prob->GetNumeroInstancias() - 1 << "]: ";
                cin >> ins;
                cout << "Numero de ejecuciones de la heuristica seleccionada: ";
                cin >> veces;
                if ((ins >= 0) && (ins < prob->GetNumeroInstancias())){
                    uint32_t contTotal = 0; // Sumatorio de contenedores
                    uint32_t fSpaceTotal = 0; // Sumatorio de espacio libre
                    for (uint16_t t = 0; t < veces; t++){
                        sol = prob->RunHeuristicQuiet(heu, ins);
                        contTotal += sol->Objetivo();
                        fSpaceTotal += sol->GetEspacioLibre();
                        cout << t << endl;
                    }
                    cout << *prob->GetInstancia(ins) << endl;
                    cout << "Numero medio de contenedores  = " << C_BRED << ((double)contTotal/veces) << C_DEFAULT << endl;
                    cout << "Numero medio de espacio libre = " << C_BRED << ((double)fSpaceTotal/veces) << C_DEFAULT << endl;
                }
                cout << "Pulse enter para continuar..." << endl;
                cin.ignore().get();
                break;

            case 0:
                break;

            default:
                system("clear");
                cout << C_BRED << "OPCION INCORRECTA" << C_DEFAULT << endl;
                cout << "Pulse enter para continuar..." << endl;
                cin.ignore().get();
                break;
        }

    }
}

int main (){

    Problema* prob = new Problema();
    prob->ParseFile("data/BPP10.txt");
    Menu(prob);

    return 0;
}

