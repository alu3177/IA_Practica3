#ifndef SOLUCION_H
#define SOLUCION_H

struct Contenedor{
    uint16_t id;    // ID del contendor
    uint16_t c;
    uint16_t usedSpace;  // Espacio usado
};

class Solucion{
    protected:
        vector<uint16_t> _vectorSolucion;
        vector<uint16_t> _vectorEspacios;   // El espacio ocupado en cada contenedor (El contenedor 'i' tendrá '_vectorEspacios[i]' de espacio ocupado)
        uint16_t _nContenedores;
        uint16_t _espacioLibre;

    public:
        Solucion(vector<uint16_t> vSol, vector<Contenedor* > vSpa, uint16_t e, uint16_t c = 0) : _nContenedores(c), _espacioLibre(e) {
            if (vSol.size() > 0)
                SetVectorSolucion (vSol);
            if (vSpa.size() > 0)
                SetVectorEspacios(vSpa);
            if (c == 0)
                _nContenedores = _vectorEspacios.size();
        }

        Solucion(vector<uint16_t> vSol, vector<uint16_t> vSpa, uint16_t e, uint16_t c = 0) : _nContenedores(c), _espacioLibre(e) {
            if (vSol.size() > 0)
                SetVectorSolucion (vSol);
            if (vSpa.size() > 0)
                SetVectorEspacios(vSpa);
            if (c == 0)
                _nContenedores = _vectorEspacios.size();
        }

        // GETTERS
        inline vector<uint16_t> GetVectorSolucion() { return _vectorSolucion; }
        inline vector<uint16_t> GetVectorEspacios() { return _vectorEspacios; }
        inline uint16_t GetNumContenedores() { return _nContenedores; }
        inline uint16_t GetEspacioLibre() { return _espacioLibre; }

        // SETTERS
        void SetVectorSolucion(vector<uint16_t> &v) {
            _vectorSolucion.clear();
            for (uint16_t i = 0; i < v.size(); i++){
                //cout << "Metiendo en vector " << v[i] << endl; // DEBUG
                _vectorSolucion.push_back(v[i]);
            }
        }

        void SetVectorEspacios(vector<uint16_t> &v) {
            _vectorEspacios.clear();
            for (uint16_t i = 0; i < v.size(); i++){
                _vectorEspacios.push_back(v[i]);
            }
        }

        void SetVectorEspacios(vector<Contenedor* > &v) {
            _vectorEspacios.clear();
            for (uint16_t i = 0; i < v.size(); i++){
                _vectorEspacios.push_back(v[i]->usedSpace);
            }
        }
        
        inline void SetNumContenedores(uint16_t n) { _nContenedores = n; }

        // SobreCarga de Operadores
        friend ostream& operator << (ostream &o, Solucion &sol){
            o << "Vector Solucion: [ ";
            for (uint16_t i = 0; i < sol.GetVectorSolucion().size(); i++)
                o << sol.GetVectorSolucion()[i] << " ";
            o << "]" << endl;
            o << "Vector de Espacios: [ ";
            for (uint16_t i = 0; i < sol.GetVectorEspacios().size(); i++)
                o << sol.GetVectorEspacios()[i] << " ";
            o << "]" << endl;
            o << "Numero de contenedores: " << sol.GetNumContenedores() << endl;
            o << "Espacio Libre: " << sol.GetEspacioLibre() << endl;
            return o;
        }
};
#endif
