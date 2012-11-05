#include "instancia.hpp"

#ifndef BASECLASS_H
#define BASECLASS_H

class BaseClass{
    protected:
        Instancia _instance;

    public:
        BaseClass (Instancia ins) : _instance(ins) {}

        inline void Print() { cout << _instance << endl; }  // DEBUG: BORRAME
};

class Derivada : public BaseClass{
    public:
        Derivada(Instancia ins) : BaseClass(ins) {}
};

#endif
