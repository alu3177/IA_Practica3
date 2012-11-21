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

#include "baseClass.hpp"

// Metodo utilizado para ordenar de mayor a menor numeros en std::sort
bool MayorAmenor (uint16_t i, uint16_t j){
    return (i > j);
}

    /* GENERADORES DE SOLUCIONES */

// Instroduce cada objeto en el primer contenedor donde quepa
Solucion* BaseClass::GeneraSolucionPrimeroQuepa(bool ordena){
    vector<Contenedor* > contenedores;
    Contenedor* cont;
    vector<uint16_t> pesos = *_instance.GetPesos();
    vector<uint16_t> v_solucion;
    bool insertado;  // Indica si se ha insertado el objeto 'i'
    uint16_t sumPesos = 0; // Suma de los pesos insertados

    if (ordena){
        sort(pesos.begin(), pesos.end(), MayorAmenor);
        _instance.SetPesos(pesos); // Persistimos los cambios en la instacia, para mantener la coherencia
    }

    for (int i = 0; i < pesos.size(); i++){
        insertado = false;
        for (uint16_t j = 0; j < contenedores.size(); j++){ // Recorremos vector de contenedores
            if ( pesos[i] <= (_instance.GetCapacidadC() - contenedores[j]->usedSpace) ){ // El objeto 'i' cabe en el contenedor 'j'
                v_solucion.push_back(contenedores[j]->id);
                contenedores[j]->usedSpace += pesos[i];
                sumPesos += pesos[i]; // Restamos espacio libre
                insertado = true;
                 break;
             }
         }
         if (!insertado){   // No lo hemos insertado (no cabe en ningun contenedor)
             cont = new Contenedor;
             cont->id = contenedores.size();
             cont->c = _instance.GetCapacidadC();
             cont->usedSpace = 0;
             contenedores.push_back(cont);
             i--;    // Evitamos avanzar de objeto (para introducirlo)
        }
    }
    uint16_t freeSpace = (contenedores.size() * _instance.GetCapacidadC()) - sumPesos; // Establecemos el espacio libre (Total - sumPesos)
    Solucion* result = new Solucion(v_solucion, contenedores, freeSpace);
    return result;
}

// Mete un objeto aleatorio en el primer contenedor que cabe
Solucion* BaseClass::GeneraSolucionInicialRandom(){
    vector<Contenedor* > contenedores;
    Contenedor* cont;
    vector<uint16_t> pesos = *_instance.GetPesos();
    vector<uint16_t> solucion(_instance.GetNumObjetos());
    bool insertado;  // Indica si se ha insertado el objeto 'i'
    uint16_t sumPesos = 0; // Suma de los pesos insertados
    uint16_t i;     // Representa a cada objeto en el vector de pesos (el objeto 'i' tiene un peso 'pesos[i]')

    srand ( time(NULL) + rand() );   // Inicializamos la semilla del RANDOM
    while (!FullyExplored(pesos)){
        insertado = false;
        i = rand() % _instance.GetNumObjetos();    // Seleccionamos un elemento al azar
        if (pesos[i] != EXPLOREDWEIGHT){   // Hemos seleccionado un elemento no asignado
            for (uint16_t j = 0; j < contenedores.size(); j++){ // Recorremos vector de contenedores
                if ( pesos[i] <= (contenedores[j]->c - contenedores[j]->usedSpace) ){ // El objeto 'i' cabe en el contenedor 'j'
                    solucion[i] = contenedores[j]->id;
                    contenedores[j]->usedSpace += pesos[i];
                    sumPesos += pesos[i]; // Incrementamos el peso usado
                    pesos[i] = EXPLOREDWEIGHT;  // "Sacamos" del vector de pesos al objeto 'i'
                    insertado = true;
                    break;
                }
            }
            if (!insertado){   // No lo hemos insertado (no cabe en ningun contenedor)
                cont = new Contenedor;
                cont->id = contenedores.size();
                cont->c = _instance.GetCapacidadC();
                cont->usedSpace = 0;
                contenedores.push_back(cont);
            }
        }
    }
    uint16_t freeSpace = (contenedores.size() * _instance.GetCapacidadC()) - sumPesos; // Establecemos el espacio libre (Total - sumPesos)
    Solucion* result = new Solucion(solucion, contenedores, freeSpace);
    return result;
}


    /* MANEJO DE VECINAS */

// Devuelve la mejor solucion vecina genrada
Solucion* BaseClass::GeneraMejorVecina (Solucion* sIn){
    Solucion* mejor = sIn;
    for (uint16_t i = 0; i < sIn->GetNumObjetos(); i++){  // Recorremos el vector solucion
        uint16_t bestSpace = _instance.GetCapacidadC(); // Mejor "espacio libre" encontrado
        for (uint16_t j = 0; j < sIn->GetNumContenedores(); j++){  // Recorremos el vector de espacios
            uint16_t nSpace = sIn->VectorEspacios(j) + _instance.GetPeso(i);  // Espacio ocupado si se mueve 'i' al contenedor 'j'
            if (nSpace <= _instance.GetCapacidadC()){  // Si 'i' cabe en 'j' lo comparamos con el mejor resultado
                if (nSpace < bestSpace){  // Si es mejor, actualizamos el mejor resultado
                    bestSpace = nSpace;
                    uint16_t prevObj = mejor->Objetivo();  // Numero de contenedores de la anterior mejor solucion
                    mejor = MoverObjeto(sIn, i, j);
                    if (mejor->Objetivo() < prevObj){  // Si se ha eliminado un contenedor implica que no se puede mejorar mas
                        return mejor;
                    }
                }
            }
        }
    }
    return mejor;
}

// Devuelve la mejor solucion vecina genrada teniendo en cuenta la lista 'tabu'
// ** Usado por la Búsqueda Tabú **
Solucion* BaseClass::GeneraMejorVecina (Solucion* sIn, vector<Solucion* > tabu){
    Solucion* mejor = sIn;
    for (uint16_t i = 0; i < sIn->GetNumObjetos(); i++){  // Recorremos el vector solucion
        uint16_t bestSpace = _instance.GetCapacidadC(); // Mejor "espacio libre" encontrado
        for (uint16_t j = 0; j < sIn->GetNumContenedores(); j++){  // Recorremos el vector de espacios
            uint16_t nSpace = sIn->VectorEspacios(j) + _instance.GetPeso(i);  // Espacio ocupado si se mueve 'i' al contenedor 'j'
            if (nSpace <= _instance.GetCapacidadC()){  // Si 'i' cabe en 'j' lo comparamos con el mejor resultado
                if (nSpace < bestSpace){  // Si es mejor ...
                    Solucion* sTemp = MoverObjeto(sIn, i, j);
                    uint16_t capacidad = _instance.GetCapacidadC();
                    if (!InVector(tabu, sTemp, capacidad)){
                        bestSpace = nSpace;
                        uint16_t prevObj = mejor->ObjetivoAux(capacidad);  // Numero de contenedores de la anterior mejor solucion
                        mejor = sTemp;
                        if (mejor->ObjetivoAux(capacidad) < prevObj){  // Si se ha eliminado un contenedor implica que es la mejor solucion
                            return mejor;
                        }
                    }
                }
            }
        }
    }
    return mejor;
}

// Devuelve una solucion vecina escogida al azar entre las posibles
// CHANCE es la probabilidad de aceptar (y reparar) una "no factible"
Solucion* BaseClass::GetVecinaRandom(Solucion* sIn){
    Solucion* result = sIn;
    float chance;
    uint32_t iter = 0;

    if ((CHANCE >= 0) && (CHANCE <= 1))
        chance = CHANCE;
    else if (CHANCE < 0)
        chance = 0;
    else if (CHANCE > 1)
        chance = 1;
    srand ( time(NULL) + rand() );   // Inicializamos la semilla del RANDOM
    while (iter < MAXITER){
        uint16_t obj = rand() % sIn->GetNumObjetos();  // Objeto seleccionado al azar
        uint16_t cont = rand() % sIn->GetNumContenedores(); // Contenedor seleccionado al azar
        result = MoverObjeto(sIn, obj, cont);
        if (result->Factible(_instance.GetCapacidadC(), *_instance.GetPesos())){
            // El movimiento es valido
            return result;
        }else{
            // El movimiento es invalido. Probamos si lo aceptamos (y mejoramos) en funcion de 'chance'
            float prob = (float)rand()/(float)RAND_MAX; // Genera un numero entre 0.0 y 1.0
            if (prob <= chance){
                result->RepairOverLoad(_instance.GetCapacidadC(), *_instance.GetPesos());
                if (result->Factible(_instance.GetCapacidadC(), *_instance.GetPesos()))
                    return result;
            }
        }
        iter++;
    }
    return result;
}

// Devuelve una solucion vecina (a profundidad k) escogida al azar entre las posibles
// K = 1 => Vecina directa
Solucion* BaseClass::GetVecinaRandom(Solucion* sIn, uint16_t k){
    uint16_t profundidad = 1;  // El primer nivel de vecinas empieza en 1
    Solucion* sol = GetVecinaRandom(sIn);
    while (profundidad < k){
        sol = GetVecinaRandom(sol);
        profundidad++;
    }
    return sol;
}

// Mueve el objeto en la posición 'obj' (dentro del vector solución),
// al contenedor 'cont'. Refresca todos los valores afectados
// Es decir, sIn->Solucion[obj] = cont
// No tiene en cuenta si produce o no sobrecarga del contenedor al que se mueve
Solucion* BaseClass::MoverObjeto(Solucion* &sIn, uint16_t obj, uint16_t cont){
    vector<uint16_t> vSolTmp = sIn->GetVectorSolucion();
    vector<uint16_t> vEspTmp = sIn->GetVectorEspacios();
    uint16_t nEspacioLibre = sIn->GetEspacioLibre(); // Nuevo espacio libre
    if ((obj < sIn->GetNumObjetos()) && (cont < sIn->GetNumContenedores())){  // 'obj' y 'cont' deben ser validos
        uint16_t prevCont = vSolTmp[obj];
        if (prevCont < vEspTmp.size()){ // Contenedor valido
            if (vEspTmp[prevCont] >= _instance.GetPeso(obj)){
                vEspTmp[prevCont] -= _instance.GetPeso(obj); // Descontamos espacio usado del contenedor del que lo sacamos
                vSolTmp[obj] = cont; // Cambiamos el objeto 'obj' de contenedor (cont)
                if (vEspTmp[prevCont] <= 0){  // Si se queda a cero, lo borramos
                    BorrarContenedor(vSolTmp, vEspTmp, prevCont);
                    nEspacioLibre -= _instance.GetCapacidadC();
                }
                vEspTmp[cont] += _instance.GetPeso(obj); // Añadimos espacio usado al contenedor al que lo movemos
            }
            Solucion* nSol = new Solucion(vSolTmp, vEspTmp, nEspacioLibre);
            return nSol;
        }
    }
    return sIn;
}

// Borra el contenedor 'pos' del vector de Espacios 'vEsp' y
// propaga los cambios al vector solucion 'vSol'
void BaseClass::BorrarContenedor(vector<uint16_t> &vSol, vector<uint16_t> &vEsp, uint16_t pos){
    vector<uint16_t> vEspTmp;

    // Construimos el nuevo vector de espacios
    for (uint16_t i = 0; i < vEsp.size(); i++){
        if (i != pos)
            vEspTmp.push_back(vEsp[i]);
    }
    vEsp = vEspTmp;
    // Actualizamos los IDs de contenedor del vector solucion
    // Restamos, uno a uno, todos los IDs de contenedor mayores que el que hemos borrado
    for (uint16_t i = 0; i < vSol.size(); i++){
        if (vSol[i] > pos)
            vSol[i]--;
    }
}


// Realiza una busqueda voraz
Solucion* BaseClass::Greedy(Solucion* &sIn){
    Solucion* actual = sIn;
    Solucion* vecina = sIn;

    uint32_t iter = 0;
    while (iter < MAXITER){
        vecina = GeneraMejorVecina(actual);
        if (vecina->Objetivo() < actual->Objetivo())
            actual = vecina;
        else
            return actual;
        iter++;
    }
    return actual;
}
