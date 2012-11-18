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

/*
// Borra el contenedor 'pos' del vector de Espacios 'vEsp' y
// propaga los cambios al vector solucion 'vSol'
void BorrarContenedor(vector<uint16_t> &vSol, vector<uint16_t> &vEsp, uint16_t pos){
    vector<uint16_t> vEspTmp;

    // Construimos el nuevo vector de pesos (result)
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

// Mueve el objeto en la posición 'obj' (dentro del vector de pesos o el vector solución),
// al contenedor 'cont'. Refresca todos los valores afectados
Solucion* MoverMenosEspacio(Solucion* &sIn, uint16_t obj, uint16_t cont, Instancia _instance){
    //cout << "MoverMenosEspacio" << endl; // DEBUG

    // Si cabe, generamos la solución con este movimiento y la añadimos a 'result'
    vector<uint16_t> vSolTmp = sIn->GetVectorSolucion();
    vector<uint16_t> vEspTmp = sIn->GetVectorEspacios();
    uint16_t nEspacioLibre = sIn->GetEspacioLibre(); // Nuevo espacio libre


    int prevCont = sIn->GetVectorSolucion()[obj];
    //cout << "vEspTmp[" << prevCont << "](";
    //cout  << vEspTmp[prevCont] <<  ") -= " << _instance.GetPeso(obj) << endl; //DEBUG
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


    Solucion* nSol = new Solucion(vSolTmp, vEspTmp, nEspacioLibre);

    //cout << "[FIN]  MoverMenosEspacio" << endl; // DEBUG

    return nSol;
}

// Calcula el conjunto de las soluciones vecinas posibles
// haciendo un movimiento (al primer contenedor donde deje menos espacio)
vector<Solucion* >* GetVecinasMenosEspacio(Solucion* sIn, Instancia _instance){
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

                    //result->push_back(MoverMenosEspacio(sIn, i, j));  // Insertamos 'i' en 'bestCont' (j)
                }
            }
        }
        if ((bestCont > 0) && (mejor != _instance.GetCapacidadC()))
            result->push_back(MoverMenosEspacio(sIn, i, bestCont, _instance));  // Insertamos 'i' en 'bestCont'
    }
    //cout << "[FIN]  GetVecinasMenosEspacio" << endl; // DEBUG
    return result;
}

// Devuelve la mejor solucion vecina genrada
Solucion* GeneraMejorVecina (Solucion* sIn, Instancia i){
    vector<Solucion* > vecinas = *GetVecinasMenosEspacio(sIn, i);
    Solucion* bestSol = sIn;

    for (uint16_t i = 0; i < vecinas.size(); i++){
        if (*vecinas[i] < *bestSol){
            bestSol = vecinas[i];
        }
    }
    return bestSol;
}

Solucion* Greedy(Solucion* &sIn, Instancia i){
    Solucion* actual = sIn;
    Solucion* vecina = actual;

    while (1){
        vecina = GeneraMejorVecina(actual, i);
        if (*vecina < *actual)
            actual = vecina;
        else
            return actual;
    }
}
*/
int main (){

    Problema* prob = new Problema();
    prob->ParseFile("data/BPP10.txt");
    //prob->BusquedasLocales();
    //prob->HeuristicasConstructivas();
    prob->Geneticas();

/*
    vector<uint16_t> vsol;
    vsol.push_back(0);
    vsol.push_back(0);
    vsol.push_back(1);
    vsol.push_back(0);
    vsol.push_back(1);
    vsol.push_back(2);
    vsol.push_back(1);
    vsol.push_back(3);
    vsol.push_back(1);
    vsol.push_back(4);

    vector<uint16_t> pesos;
    pesos.push_back(3);
    pesos.push_back(6);
    pesos.push_back(2);
    pesos.push_back(1);
    pesos.push_back(5);
    pesos.push_back(7);
    pesos.push_back(2);
    pesos.push_back(4);
    pesos.push_back(1);
    pesos.push_back(9);

    uint16_t c = 10;

 //   Solucion* sol1 = new Solucion(vsol, c, pesos);
    Instancia* ins = new Instancia("I_1", 10, 10, 0, &pesos);
 //   Solucion* sol2 = GeneraSolucionInicialRandom(ins);

 //   cout << *sol1 << endl;
//    cout << *sol2 << endl;

 //   pair<Solucion*, Solucion* > par(sol1, sol2);
 //   pair<Solucion*, Solucion* > hijas;

    Genetics* gen = new Genetics(*ins);
//    hijas = gen->Cruzar(par);

 //   cout << *hijas.first << endl;
//    cout << *hijas.second << endl;

    cout << " -- AG : " << *gen->AG() << endl;
*/
    return 0;
}

