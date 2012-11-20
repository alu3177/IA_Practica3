#include "genetics.hpp"

// Metodo utilizado para ordenar de menor a mayor (espacio dejado) soluciones en std::sort
bool CompareSolucion (Solucion* s1, Solucion* s2){
    return (s1->Objetivo() < s2->Objetivo());
}

// Algoritmo genetico
// TODO: Usar AVERAGE para detener el proceso cuando no se mejora más
Solucion* Genetics::AG(uint16_t maxGen){
    uint16_t t = 0;  // Generacion actual
    vector<Solucion* > poblacion;
    //float average = 0.0; // Promedio de huecos de la poblacion actual

    // Generamos la poblacion inicial
    for (uint16_t i = 0; i < POBLACION_SIZE; i++)
        poblacion.push_back(GeneraSolucionInicialRandom());

    //average = EvaluaPoblacion(poblacion);

    // Seleccionamos las BEST_COUNT mejores de la poblacion inicial
    vector<Solucion* > nextGen = SeleccionaMejores(poblacion);
    while (t < maxGen){
        // Selecciona COUPLES_NUMBER, las cruza y muta MUTANTS soluciones hijas
        //cout << "Gen: " << t << endl; // DEBUG
        vector<Solucion* > descendientes = Operar(nextGen);

        //cout << "Desendencia: " << descendientes.size() << endl; // DEBUG
        //average = EvaluaPoblacion(descendientes);
        // Volvemos a seleccionas las BEST_COUNT mejores (en este caso de las hijas)
        nextGen.clear();
        nextGen = SeleccionaMejores(descendientes);
        t++;
    }
    sort(nextGen.begin(), nextGen.end(), CompareSolucion);
    //cout << "Ultima generacion: " << nextGen.size() << endl; // DEBUG
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

    //cout << "Descendientes tras cruce: " << descendientes.size() << endl; // DEBUG

    // DEBUG
    for (uint16_t i = 0; i < descendientes.size(); i++){
        //cout << "[" << i << "] " << *descendientes[i] << endl;
    }
    // DEBUG

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

    //cout << "CRUZANDO:" << endl << endl << *pareja.first << endl << " con " << endl << endl << *pareja.second << endl;

    srand ( time(NULL) + rand() ); // Inicializamos la semilla del RANDOM
    uint16_t j = rand() % vs1.size();  // Seleccionamos una posicion al azar dentro del vector solucion

    //cout << " punto de cruce = " << j << endl; // DEBUG

    for (uint16_t i = 0; i < j; i++){
        uint16_t tmp = vs1[i];
        vs1[i] = vs2[i];
        vs2[i] = tmp;
    }
    //cout << vs1.size() << ", " << _instance.GetCapacidadC() << endl; // DEBUG
    Solucion* nSol = new Solucion(vs1, _instance.GetCapacidadC(), *_instance.GetPesos());
    //cout << "Primer CRUCE:" << *nSol << endl; // DEBUG
    result.first = nSol;
    nSol = new Solucion(vs2, _instance.GetCapacidadC(), *_instance.GetPesos());
    //cout << "Segundo CRUCE:" << *nSol << endl; // DEBUG
    result.first = nSol;
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
        //cout << "MuTANDO: " << i << " pasa a " << j << endl; // DEBUG
        result.SetSolucion(j, i);  // El objeto 'i' muta y pasa al contenedor 'j'
        result.SetEspacio(result.VectorEspacios(j) - _instance.GetPeso(i), j);

        //cout << result << endl;  // DEBUG
        if (result.Factible(_instance.GetCapacidadC(), *_instance.GetPesos())){
            result = *sol;
            //cout << "FACTIBLE:" << endl << result << endl;  // DEBUG
        }
    }

    return result;
}
