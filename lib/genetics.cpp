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

#include "genetics.hpp"

// Metodo utilizado para ordenar de menor a mayor (espacio dejado) soluciones en std::sort
bool CompareSolucion (Solucion* s1, Solucion* s2){
    return (s1->Objetivo() < s2->Objetivo());
}

// Algoritmo genetico
// METAHEURISTICA
// 3) GA
Solucion* Genetics::AG(uint16_t maxGen){
    uint16_t t = 0;  // Generacion actual
    vector<Solucion* > poblacion;

    // Generamos la poblacion inicial
    for (uint16_t i = 0; i < POBLACION_SIZE; i++)
        poblacion.push_back(GeneraSolucionInicialRandom());
    // Seleccionamos las BEST_COUNT mejores de la poblacion inicial
    vector<Solucion* > nextGen = SeleccionaMejores(poblacion);
    while (t < maxGen){
        // Selecciona COUPLES_NUMBER, las cruza y muta MUTANTS soluciones hijas
        vector<Solucion* > descendientes = Operar(nextGen);
        // Volvemos a seleccionas las BEST_COUNT mejores (en este caso de las hijas)
        nextGen.clear();
        nextGen = SeleccionaMejores(descendientes);
        t++;
    }
    sort(nextGen.begin(), nextGen.end(), CompareSolucion);
    return nextGen[0];

}

// Selecciona las 'n' mejores soluciones de 'pob'
vector<Solucion* > Genetics::SeleccionaMejores(vector<Solucion* > pob, uint16_t n){
    vector<Solucion* > seleccion;

    sort(pob.begin(), pob.end(), CompareSolucion);
    for (uint16_t i = 0; i < pob.size(); i++){
        if (i < n)
            seleccion.push_back(pob[i]);
        else
            break;
    }
    return seleccion;
}

// Escoge COUPLES_NUMBER parejas de 's' y las cruza. Despues muta MUTANTS
// elementos y devuelve la nueva generacion entera
vector<Solucion* > Genetics::Operar(vector<Solucion* > &s){
    vector<pair<Solucion*, Solucion* > > parejas = SeleccionaParejas(s);
    vector<Solucion* > descendientes;

    for (uint16_t i = 0; i < parejas.size(); i++){
        pair<Solucion*, Solucion* > hijos = Cruzar(parejas[i]);
        descendientes.push_back(hijos.first);
        descendientes.push_back(hijos.second);
    }
    srand ( time(NULL) + rand() ); // Inicializamos la semilla del RANDOM
    for (int16_t m = 0; m < MUTANTS; m++){  // Mutamos MUTANTS soluciones al azar
        uint16_t i = rand() % descendientes.size();
        Solucion mutante = Mutar(descendientes[i]);
        descendientes[i] = &mutante;
    }
    return descendientes;
}

// Construye un vector de 'p' parejas (aleatorias) de soluciones
vector<pair<Solucion*, Solucion* > > Genetics::SeleccionaParejas(vector<Solucion* > pob, uint16_t p){
    vector<pair<Solucion*, Solucion* > > result;

    srand ( time(NULL) + rand() ); // Inicializamos la semilla del RANDOM
    for (uint16_t i = 0; i < p; i++){
        uint16_t s1 = rand() % pob.size();  // Seleccionamos una solucion al azar
        uint16_t s2 = s1;
        while (s1 == s2)
            s2 = rand() % pob.size();  // Seleccionamos otra solucion al azar
        pair<Solucion*, Solucion* > pareja(pob[s1], pob[s2]);
        result.push_back(pareja);
    }
    return result;
}

// Cruza una pareja de soluciones y obtiene (devuelve) dos soluciones resultantes
pair<Solucion*, Solucion* > Genetics::Cruzar(pair<Solucion*, Solucion* > &pareja){
    pair<Solucion*, Solucion* > result;
    vector<uint16_t> vs1 = pareja.first->GetVectorSolucion();
    vector<uint16_t> vs2 = pareja.second->GetVectorSolucion();

    srand ( time(NULL) + rand() ); // Inicializamos la semilla del RANDOM
    uint16_t j = rand() % vs1.size();  // Seleccionamos una posicion al azar dentro del vector solucion
    for (uint16_t i = 0; i < j; i++){  // Realizamos el cruce de vectores solucion
        uint16_t tmp = vs1[i];
        vs1[i] = vs2[i];
        vs2[i] = tmp;
    }
    Solucion* nSol = new Solucion(vs1, _instance.GetCapacidadC(), *_instance.GetPesos());
    result.first = nSol;
    nSol = new Solucion(vs2, _instance.GetCapacidadC(), *_instance.GetPesos());
    result.second = nSol;

    return result;
}

// Realiza una mutacion (cambia un valor al azar del vector solucion)
// Devuelve 'sol' si no es factible
Solucion Genetics::Mutar(Solucion* sol){
    Solucion result = *sol;
    srand ( time(NULL) + rand() ); // Inicializamos la semilla del RANDOM
    if ((result.GetNumObjetos() > 0) && (result.GetNumContenedores() > 0)){
        uint16_t i = rand() % result.GetNumObjetos();  // Objeto aleatorio del vector solucion
        uint16_t j = rand() % result.GetNumContenedores();  // Contenedor aleatorio

        result.SetSolucion(j, i);  // El objeto 'i' muta y pasa al contenedor 'j'
        result.SetEspacio(result.VectorEspacios(j) - _instance.GetPeso(i), j);
        if (result.Factible(_instance.GetCapacidadC(), *_instance.GetPesos()))
            result = *sol;
    }

    return result;
}
