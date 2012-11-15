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
        inline void SetEspacioLibre(uint16_t s) { _espacioLibre = s; }

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
            /*
            o << "Vector Solucion: [ ";
            for (uint16_t i = 0; i < sol.GetVectorSolucion().size(); i++)
                o << sol.GetVectorSolucion()[i] << " ";
            o << "]" << endl;
            o << "Vector de Espacios: [ ";
            for (uint16_t i = 0; i < sol.GetVectorEspacios().size(); i++)
                o << sol.GetVectorEspacios()[i] << " ";
            o << "]" << endl;
            */
            o << "Numero de contenedores: " << C_BRED << sol.GetNumContenedores() << endl;
            o << C_DEFAULT << "Espacio Libre: " << C_BRED << sol.GetEspacioLibre() << C_DEFAULT << endl;
            return o;
        }

        bool operator  < (Solucion &sol){
            //cout << _espacioLibre << " < " << sol.GetEspacioLibre() << endl; // DEBUG
            return ( (_espacioLibre < sol.GetEspacioLibre()) );
        }

        bool operator  <= (Solucion &sol){
            //cout << _espacioLibre << " < " << sol.GetEspacioLibre() << endl; // DEBUG
            return ( (_espacioLibre <= sol.GetEspacioLibre()) );
        }

        bool operator  == (Solucion &sol){
            //return ( (_espacioLibre == sol.GetEspacioLibre()) );
            /*
            if ( (_vectorSolucion.size() != sol.GetVectorEspacios().size()) && (_vectorEspacios.size() != sol.GetVectorEspacios().size()) ){
                
                for (uint16_t i = 0; i < _vectorSolucion.size(); i++){
                    if (_vectorSolucion[i] != sol.GetVectorSolucion()[i])
                        return false;
                }
                for (uint16_t i = 0; i < _vectorEspacios.size(); i++){
                    if (_vectorEspacios[i] != sol.GetVectorEspacios()[i])
                        return false;
                }             
            }else
                return false;
            return true;
            */
            if ( (_nContenedores == sol.GetNumContenedores()) && (_espacioLibre == sol.GetEspacioLibre()) )
                return true;
            return false;
        }

        int operator  - (Solucion &sol){
            //cout << _nContenedores << " - " <<  sol.GetNumContenedores() << endl;  // DEBUG
            return ( (_nContenedores - sol.GetNumContenedores()) );
        }
};
#endif
