#include "baseClass.hpp"

#ifndef LOCALSEARCHES_H
#define LOCALSEARCHES_H

extern Solucion* GeneraSolucionInicialRandom(Instancia* ins);

class LocalSearches : public BaseClass {
    public:
        LocalSearches(Instancia ins) : BaseClass(ins) {}

        // Búsqueda Local Iterada
        Solucion* ILS(){
            bool seguir = true;
            //while (seguir){
                Solucion* actual = GeneraSolucionInicialRandom(&_instance);
                cout << "Actual: " << endl << *actual << endl; // DEBUG
/*
                vector<uint16_t> vsol;
                vsol.push_back(1);
                vsol.push_back(2);
                vsol.push_back(3);
                vsol.push_back(4);
                vsol.push_back(3);
                vsol.push_back(1);
                vsol.push_back(3);
                vsol.push_back(2);
                vsol.push_back(3);
                vsol.push_back(0);
                vector<uint16_t> vEsp;
                vEsp.push_back(10);
                vEsp.push_back(10);
                vEsp.push_back(10);
                vEsp.push_back(9);
                vEsp.push_back(1);
                
                Solucion* test = new Solucion(vsol, vEsp, 10);
                cout << "Actual: " << *test << endl; // DEBUG
                vector<Solucion* > sols = GetVecinasMenosEspacio(test);
*/
                vector<Solucion* > sols = GetVecinasMenosEspacio(actual);
                uint16_t menorEspacio = actual->GetEspacioLibre();
                Solucion* mejorSol;
                for (uint16_t i = 0; i < sols.size(); i++){
                    //cout << *sols[i] << endl;
                    vector<Solucion* > solss = GetVecinasMenosEspacio(sols[i]);
                    for (uint16_t i = 0; i < solss.size(); i++){
                        if (solss[i]->GetEspacioLibre() <= menorEspacio){
                            menorEspacio = solss[i]->GetEspacioLibre();
                            mejorSol = solss[i];
                        }
                    }
                
                }
                cout << "Mejor Solución:" << endl << *mejorSol << endl;
            //}
        }
};

#endif
