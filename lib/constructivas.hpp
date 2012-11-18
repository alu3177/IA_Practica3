#include <algorithm>
#include "baseClass.hpp"

using namespace std;

extern bool FullyExplored (vector<uint16_t>&);

#ifndef CONSTRUCTIVAS_H
#define CONSTRUCTIVAS_H

#define EXPLOREDWEIGHT 0    // Marca para indicar que se ha introducido dicho objeto en algun contenedor

const uint16_t GRASP_T = 60;

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
        Solucion* GRASP(uint16_t t = GRASP_T);

    private:
        vector<Candidato> GetListaRestringidaCandidatos(vector<uint16_t> &pesos, vector<Contenedor* > &contenedores, uint16_t t);
};

#endif
