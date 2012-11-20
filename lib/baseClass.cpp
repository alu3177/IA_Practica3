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
        //cout << "SORRRRRT" << endl << *ins << endl; // DEBUG
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
            //cout << "Seleccionado " << i << endl; // DEBUG
            for (uint16_t j = 0; j < contenedores.size(); j++){ // Recorremos vector de contenedores
                if ( pesos[i] <= (contenedores[j]->c - contenedores[j]->usedSpace) ){ // El objeto 'i' cabe en el contenedor 'j'
                    //cout << "INSERTANDO objeto " << i << " en contenedor " << j << endl; // DEBUG
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
                //cout << "CREO contenedor " << cont->id << endl;  // DEBUG
            }
        }
    }
    uint16_t freeSpace = (contenedores.size() * _instance.GetCapacidadC()) - sumPesos; // Establecemos el espacio libre (Total - sumPesos)
    Solucion* result = new Solucion(solucion, contenedores, freeSpace);
    //cout << "Creo solucion y salgo" << endl;
    return result;
}


    /* MANEJO DE VECINAS */
/*
// Devuelve la mejor solucion vecina genrada
Solucion* BaseClass::GeneraMejorVecina (Solucion* sIn){
    vector<Solucion* > vecinas = *GetVecinasMenosEspacio(sIn);
    Solucion* bestSol = sIn;

    for (uint16_t i = 0; i < vecinas.size(); i++){
        if (*vecinas[i] < *bestSol){
            bestSol = vecinas[i];
        }
    }
    return bestSol;
}
*/
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
                    if (mejor->Objetivo() < prevObj){  // Si se ha eliminado un contenedor implica que es la mejor solucion
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

/*
// Devuelve la mejor solucion vecina genrada teniendo en cuenta la lista 'tabu'
// ** Usado por la Búsqueda Tabú **
Solucion* BaseClass::GeneraMejorVecina (Solucion* sIn, vector<Solucion* > tabu){
    vector<Solucion* > vecinas = *GetVecinasMenosEspacio(sIn);
    Solucion* bestSol = sIn;


    cout << "Nº vecinas = " << vecinas.size() << endl; // DEBUG
    for (uint16_t i = 0; i < vecinas.size(); i++){
        if (vecinas[i]->ObjetivoAux(_instance.GetCapacidadC()) <= bestSol->ObjetivoAux(_instance.GetCapacidadC())){
            //cout << "MejorVecina, Mejora" << endl; // DEBUG
            if (!InVector(tabu, vecinas[i], _instance.GetCapacidadC())){
                bestSol = vecinas[i];
                cout << "Cojo uno" << *bestSol << endl; // DEBUG
            } //else
              //  cout << "Descarto" << endl; // DEBUG
        }
    }

    return bestSol;

}
*/
// Devuelve una solucion vecina genrada al azar teniendo en cuenta la lista 'tabu'
// ** Usado por la Búsqueda Tabú **
Solucion* BaseClass::GeneraVecinaRandom (Solucion* sIn, vector<Solucion* > tabu){
    //vector<Solucion* > vecinas = *GetVecinasMenosEspacio(sIn);
    Solucion* bestSol = GetVecinaRandom(sIn);
    uint16_t maxIt = 50;


    //cout << "Nº vecinas = " << vecinas.size() << endl; // DEBUG
    //for (uint16_t i = 0; i < vecinas.size(); i++){
        //if (vecinas[i]->ObjetivoAux(_instance.GetCapacidadC()) <= bestSol->ObjetivoAux(_instance.GetCapacidadC())){
            //cout << "MejorVecina, Mejora" << endl; // DEBUG
            uint16_t i = 0;
            while ((InVector(tabu, bestSol, _instance.GetCapacidadC())) && (i < maxIt)){
                bestSol = GetVecinaRandom(sIn);
                i++;
            } //else
              //  cout << "Descarto" << endl; // DEBUG
        //}
    //}

    return bestSol;

}

// Devuelve una solucion vecina escogida al azar entre las posibles
Solucion* BaseClass::GetVecinaRandom(Solucion* sIn){
    Solucion* result = NULL;
    float chance = 0.5;  // Probabilidad de aceptar (y reparar) una "no factible"

    srand ( time(NULL) + rand() );   // Inicializamos la semilla del RANDOM
    while (1){
        uint16_t obj = rand() % sIn->GetNumObjetos();  // Objeto seleccionado al azar
        uint16_t cont = rand() % sIn->GetNumContenedores(); // Contenedor seleccionado al azar
        result = MoverObjeto(sIn, obj, cont);
        if (result->Factible(_instance.GetCapacidadC(), *_instance.GetPesos())){
            //cout << "FACTIBLE !! :)" << endl; // DEBUG
            return result;
        }else{
            float prob = (float)rand()/(float)RAND_MAX; // Genera un numero entre 0.0 y 1.0
            if (prob <= chance){
                //cout << "Reparando !!" << endl; // DEBUG
                result->RepairOverLoad(_instance.GetCapacidadC(), *_instance.GetPesos());
                //cout << C_GREEN << *result << endl; // DEBUG
                if (result->Factible(_instance.GetCapacidadC(), *_instance.GetPesos()))
                    return result;
            }
        }
    }
}

// Devuelve una solucion vecina (a profundidad k) escogida al azar entre las posibles
// K = 0 => Vecina directa
Solucion* BaseClass::GetVecinaRandom(Solucion* sIn, uint16_t k){
    uint16_t profundidad = 0;
    Solucion* sol = GetVecinaRandom(sIn);
    while (profundidad < k){
        sol = GetVecinaRandom(sol);
        profundidad++;
    }
    //cout << "Prof : "  << profundidad << endl; // DEBUG
    return sol;
}
/*
// Calcula el conjunto de las soluciones vecinas posibles
// haciendo un movimiento (al primer contenedor donde deje menos espacio)
vector<Solucion* >* BaseClass::GetVecinasMenosEspacio(Solucion* sIn){
    //cout << "GetVecinasMenosEspacio" << endl; // DEBUG
    vector<Solucion* >* result = new vector<Solucion* >;
    uint16_t mejor; // Mejor valor (de menos espacio dejado tras insertar) encontrado
    int bestCont;   // ID del mejor contenedor encontrado
    // Recorremos el vector de resultados
    for (uint16_t i = 0; i < sIn->GetVectorSolucion().size(); i++){
        bestCont = -1;
        mejor = _instance.GetCapacidadC(); // Iniciamos al maximo valor posible (peor caso) para cada objeto
        // Recorremos los contenedores
        for (uint16_t j = 0; j < sIn->GetNumContenedores(); j++){
            if (sIn->GetVectorSolucion()[i] != j){ // No comprobamos para el mismo contenedor
                uint16_t libre = _instance.GetCapacidadC() - sIn->GetVectorEspacios()[j]; // Espacio libre en el contenedor 'j'
                //cout << "[" << i <<  "]libre " << j << " : " << libre << endl;
                if (_instance.GetPeso(i) < libre){ // ¿Cabe?
                    //cout << "Moviendo objeto " << i << "(" << _instance.GetPeso(i) << ") de " << sIn->GetVectorSolucion()[i] << " a " << j << endl; // DEBUG

                    if (libre - _instance.GetPeso(i) < mejor){  // Si deja menos espacio libre que el anterior mejor resultado...
                        mejor = libre - _instance.GetPeso(i);
                        bestCont = j;
                    }

                    //result->push_back(MoverObjeto(sIn, i, j));  // Insertamos 'i' en 'bestCont' (j)
                }
            }
        }
        if ((bestCont > 0) && (mejor != _instance.GetCapacidadC()))
            result->push_back(MoverObjeto(sIn, i, bestCont));  // Insertamos 'i' en 'bestCont'
    }
    //cout << "[FIN]  GetVecinasMenosEspacio" << endl; // DEBUG
    return result;
}
*/
// Mueve el objeto en la posición 'obj' (dentro del vector solución),
// al contenedor 'cont'. Refresca todos los valores afectados
// Es decir, sIn->Solucion[obj] = cont
Solucion* BaseClass::MoverObjeto(Solucion* &sIn, uint16_t obj, uint16_t cont){
    //cout << "MoverObjeto" << endl; // DEBUG
    vector<uint16_t> vSolTmp = sIn->GetVectorSolucion();
    vector<uint16_t> vEspTmp = sIn->GetVectorEspacios();
    uint16_t nEspacioLibre = sIn->GetEspacioLibre(); // Nuevo espacio libre
    if ((obj < vSolTmp.size()) && (cont < vEspTmp.size())){

        int prevCont = vSolTmp[obj];
        if (prevCont < vEspTmp.size()){

            /*
            cout << C_BRED << "MOviendo " << obj << " a " << cont << endl; // DEBUG
            cout << C_CYAN << *sIn << endl; // DEBUG
            cout << "vEspTmp[" << prevCont << "]("; // DEBUG
            cout  << vEspTmp[prevCont] <<  ") -= " << _instance.GetPeso(obj) << endl; //DEBUG
            */
            if (vEspTmp[prevCont] >= _instance.GetPeso(obj)){
                vEspTmp[prevCont] -= _instance.GetPeso(obj); // Descontamos espacio usado del contenedor del que lo sacamos
                vSolTmp[obj] = cont; // Cambiamos el objeto 'obj' de contenedor (cont)
                if (vEspTmp[prevCont] <= 0){  // Si se queda a cero, lo borramos
                    //cout << "BORRANDO CONTENEDOR " << prevCont << endl;  // DEBUG
                    //vEspTmp = BorrarElemento(vEspTmp, prevCont);
                    //vEspTmp.erase(vEspTmp.begin() + prevCont);
                    BorrarContenedor(vSolTmp, vEspTmp, prevCont);
                    nEspacioLibre -= _instance.GetCapacidadC();
                    //cout << "BORRANDO !!!!" << endl;  // DEBUG
                }
                vEspTmp[cont] += _instance.GetPeso(obj); // Añadimos espacio usado al contenedor al que lo movemos
            }


            Solucion* nSol = new Solucion(vSolTmp, vEspTmp, nEspacioLibre);

            //cout << "[FIN]  MoverObjeto" << endl << *nSol << endl; // DEBUG

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
    // Restamos uno a todos los IDs de contenedor mayores que el que hemos borrado
    for (uint16_t i = 0; i < vSol.size(); i++){
        if (vSol[i] > pos)
            vSol[i]--;
    }
}
