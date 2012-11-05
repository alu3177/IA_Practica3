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
                cout << "Actual: " << *actual << endl; // DEBUG
                vector<Solucion* > sols = GetVecinasMenosEspacio(actual);
                for (uint16_t i = 0; i < sols.size(); i++)
                    cout << *sols[i] << endl;
            //}
        }
};

#endif
