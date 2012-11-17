#include <algorithm>
#include "baseClass.hpp"

#ifndef CONSTRUCTIVAS_H
#define CONSTRUCTIVAS_H

extern bool FullyExplored (vector<uint16_t>&);
#define EXPLOREDWEIGHT 0    // Marca para indicar que se ha introducido dicho objeto en algun contenedor

// Representa cada uno de los pasos candidatos
struct Candidato{
    uint16_t oID;  // ID del objeto candidato
    uint16_t cID;  // ID del contendor candidato
    uint16_t freeSpace;  // Espacio libre en 'cID' tras insertar 'oID'
};

class Constructivas : public BaseClass {
    public:
        // Constructor
        Constructivas(Instancia &ins) : BaseClass(ins) {}
        // Heurísticas
        Solucion* GRASP(uint16_t t = 30);

    private:
        vector<Candidato> GetListaRestringidaCandidatos(vector<uint16_t> &pesos, vector<Contenedor* > &contenedores, uint16_t t);
};

#endif
