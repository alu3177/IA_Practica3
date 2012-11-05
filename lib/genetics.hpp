#include <stdint.h>
#include <vector>
#include <time.h>

using namespace std;

#ifndef GENETICS_H
#define GENETICS_H

#define DEFAULT_MAXGENERATIONS 200
class Genetics{
    protected:
        Instancia* _instance;
        vector<Solucion* >* _cjtoSoluciones;
        uint16_t _maxGeneraciones; // Número máximo de generaciones

    public:
        Genetics(Instancia* ins = NULL, vector<Solucion* >* sols = NULL, uint16_t mGen = DEFAULT_MAXGENERATIONS) : _maxGeneraciones(mGen) {
            if (ins == NULL)
                _instance = new Instancia();
            else
                _instance = ins;

            if (sols == NULL)
                _cjtoSoluciones = new vector<Solucion* >;
            else
                _cjtoSoluciones = sols;
        }

};

#endif
