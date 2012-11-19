#include "genetics.hpp"

// Algoritmo genetico
// TODO: Usar AVERAGE para detener el proceso cuando no se mejora más
Solucion* Genetics::AG(uint16_t maxGen){
    uint16_t t = 0;  // Generacion actual
    vector<Solucion* > poblacion;
    //float average = 0.0; // Promedio de huecos de la poblacion actual

    for (uint16_t i = 0; i < POBLACION_SIZE; i++)
        poblacion.push_back(GeneraSolucionInicialRandom(&_instance));

    //average = EvaluaPoblacion(poblacion);

    vector<Solucion* > seleccion = SeleccionaMejores(poblacion);
    vector<Solucion* > descendientes;
    while (t < maxGen){
        descendientes = Operar(seleccion);
        //average = EvaluaPoblacion(descendientes);
        descendientes = SeleccionaMejores(poblacion);
        t++;
    }

    sort(descendientes.begin(), descendientes.end(), CompareSolucion);
    return descendientes[0];

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

    //cout << " punto de cruce = " << j << endl; // DEBUG

    for (uint16_t i = 0; i < j; i++){
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
    if (result.GetVectorSolucion().size() > 0){
        uint16_t i = rand() % result.GetVectorSolucion().size();  // Objeto aleatorio del vector solucion
        uint16_t j = rand() % result.GetNumContenedores();  // Contenedor aleatorio
        //cout << "MuTANDO: " << i << " pasa a " << j << endl; // DEBUG
        result.SetSolucion(j, i);
        result.SetEspacio(result.GetVectorEspacios()[j] - _instance.GetPeso(i), j);

        //cout << result << endl;  // DEBUG
        if (!result.Factible(_instance.GetCapacidadC())){
            result = *sol;
            //cout << "NO FACTIBLE:" << endl << result << endl;  // DEBUG
        }
    }

    return result;
}
