/*
 * Inteligencia Artificial - Práctica 3: Problema de empaquetado unidimensional
 *
 * AUTOR: Fernando González López-Peñalver (alu3177)
 * EMAIL: alu0100256543@ull.edu.es
 *
 * Fecha de entrega:
 *
 * Enunciado del problema:
 *      Se dispone de una cantidad indeterminada de contenedores de
 *      capacidad C y n objetos de tamaños w1, w2, …, wn. Se trata de
 *      determinar el empaquetado de todos los objetos usando el menor
 *      número posible de contenedores m*. La suma de los tamaños de los
 *      objetos asignados a contenedor no puede sobrepasar la capacidad C.
 *
 */
#include <stdlib.h>
#include <stdint.h>
#include <iostream>   // cout, cin
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Instancia {
    protected:
        string _name;            // Nombre de la instancia
        uint16_t _capacidadC;    // Capacidad de cada contenedor (C)
        uint16_t _nObjetos;      // Número de objetos (n)
        uint16_t _mejorNumCont;  // Mejor número de contenedores
        vector<uint16_t>* _pesos; // Puntero a, vector con los pesos de los elementos (El elemento 'i' pesa '_pesos[i]')
    public:
        // Constructor
        Instancia(string nam = "", uint16_t c = 0, uint16_t n = 0, uint16_t best = 0, vector<uint16_t>* w = NULL) : _name(nam), _capacidadC(c), _nObjetos(n), _mejorNumCont(best) {
            if (w != NULL)
                _pesos = w;
            else
                _pesos = new vector<uint16_t>;
        }

        // Métodos
        inline void AddPeso(uint16_t w) { _pesos->push_back(w); }

        // GETTERS y SETTERS
        inline string GetName() { return _name; }
        inline uint16_t GetCapacidadC() { return _capacidadC; }
        inline uint16_t GetNumObjetos() { return _nObjetos; }
        inline uint16_t GetMejorNumContenedores() { return _mejorNumCont; }
        inline vector<uint16_t>* GetPesos() { return _pesos; }

        inline void SetName (string n) { _name = n; }
        inline void SetCapacidadC (uint16_t c) { _capacidadC = c; }
        inline void SetNumObjetos (uint16_t n) { _nObjetos = n; }
        inline void SetMejorNumContenedores (uint16_t best) { _mejorNumCont = best; }
        inline void SetPesos(vector<uint16_t>* w) { _pesos = w; }

        // SobreCarga de Operadores
        friend ostream& operator << (ostream &o, Instancia &ins){
            o << "Instancia:                     " << ins.GetName() << endl;
            o << "Capacidad de los contenedores: " << ins.GetCapacidadC() << endl;
            o << "Número de objetos:             " << ins.GetNumObjetos() << endl;
            o << "Mejor número de contenedores:  " << ins.GetMejorNumContenedores() << endl;
            for (uint16_t i = 0; i < ins.GetPesos()->size(); i++){
                o << "[" << i << "] " << ins.GetPesos()->at(i) << endl;
            }
            return o;
        }

};

class Problema {
    protected:
        uint16_t _nInstancias;            // Número de instancias
        vector<Instancia* >* _instances; // Puntero a, vector con punteros a las distintas instancias

    public:
        // Constructor
        Problema(uint16_t nI = 0, vector<Instancia* >* ins = NULL) : _nInstancias(nI) {
            if (ins != NULL)
                _instances = ins;
            else
                _instances = new vector<Instancia* >;
        }

        // Getters y Setters

        //inline void SetNumInstancias (uint16_t nI) { _nInstancias = nI; }

        // Métodos
        void ParseFile (const char* path); // Lee el fichero de entrada y carga los datos del problema
        //inline void AddInstancia(Instancia* i) { _instances->push_back(i); }

};


/* FUNCIONES AUXILIARES de manejo de char* */
void ResetChar (char* str, uint16_t size){
   for (uint16_t i = 0; i < size; i++)
      str[i] = 0;
}

bool IsEmpty (char* str, uint16_t size){
   for (uint16_t i = 0; i < size; i++)
      if (str[i] != 0)
         return false;
   return true;
}

#define BUFFSIZE 16
#define SEPARADOR ' '

void Problema::ParseFile (const char* path){
    fstream file;
    char *buff2 = new char[BUFFSIZE];
    string buff;
    bool haveInstances = false; // Indica si ya hemos leido el numero de instancias que tiene el fichero
    bool enInstancia = false;   // Indica si estamos leyendo datos "dentro de una instancia"
    bool haveData = false;      // Indica si hemos leido los datos de la instancia actual
    Instancia* ins;     // Puntero usado para crear (y añadir al problema) nuevas instancias

    file.open(path, fstream::in);
    if (file.is_open()){
        // Iteramos por el fichero mientras podamos
        while (file.good()){
            uint16_t i = 0;    // Iterador del Buffer 'buff'
            getline(file, buff);

            // Obtención del numero de instancias
            if (!haveInstances){
                while (i < buff.size()){
                    buff2[i] = buff[i];
                    i++;
                }
                _nInstancias = atoi(buff2);

                cout << "_nInstancias = " << _nInstancias << endl;   // DEBUG
                cout << "buff = " << buff << endl;   // DEBUG

                haveInstances = true;
                buff.clear();
                ResetChar(buff2, BUFFSIZE);
                //i = 0;

            // Línea de nueva Instancia
            } else if (buff[0] == SEPARADOR) {
                if (enInstancia){
                    // Si ya estábamos leyendo otra instancia, la almacenamos antes de crear una nueva
                    _instances->push_back(ins);
                    haveData = false; // No disponemos de los datos de la nueva instancia
                    cout << *ins << endl;   // DEBUG (mostramos la instancia generada)
                }else
                    enInstancia = true;
                // Creamos nueva instancia con el nombre almacenado en 'buff' menos el primer caracter (SEPARADOR)
                ins = new Instancia(buff.substr(1, buff.size() - 1));
                //cout << "ins->GetName() = " << ins->GetName() << endl;  // DEBUG
                //cout << "Instancia: " << buff << endl;  // DEBUG

            // Línea con los datos de la instancia
            } else if ((enInstancia) && (!haveData)) {
                //uint16_t i = 0; // Iterador de 'buff'
                uint16_t j = 0; // Iterador de 'buff2'
                uint16_t c = 0, n = 0, best = 0; // Datos de la instancia
                while (i <= buff.size()){
                    // Copiar en Buffer 2
                    // Si no se trata de un SEPARADOR llenamos 'buff2'
                    if ((buff[i] != SEPARADOR)&&(i < buff.size())){
                        buff2[j] = buff[i];
                        j++;
                    // Usar el número leído
                    }else if ((!IsEmpty(buff2, BUFFSIZE))/* || (i >= buff.size() - 1)*/){
                        //cout << "buff2 : " << buff2 << endl;  // DEBUG
                        if ( c == 0 ){
                            //cout << " c << " << buff2 << endl;   // DEBUG
                            c = atoi(buff2);
                            ins->SetCapacidadC(c);
                            ResetChar (buff2, BUFFSIZE);
                            j = 0;
                        }else if ( n == 0 ){
                            //cout << " n << " << buff2 << endl;   // DEBUG
                            n = atoi(buff2);
                            ins->SetNumObjetos(n);
                            ResetChar (buff2, BUFFSIZE);
                            j = 0;
                        }else if ( best == 0 ){
                            //cout << " best << " << buff2 << endl;   // DEBUG
                            best = atoi(buff2);
                            ins->SetMejorNumContenedores(best);
                            ResetChar (buff2, BUFFSIZE);
                            j = 0;
                        }
                    }
                    i++;
                }
                haveData = true;

            // Línea con el peso de un componente
            } else if ((enInstancia) && (haveData)) {
                // Copiamos a 'buff2'
                while (i < buff.size()){
                    buff2[i] = buff[i];
                    i++;
                }
                ins->AddPeso(atoi(buff2));
                //cout << "w = " << atoi(buff2) << endl;  // DEBUG
            }

         /*else{
            uint16_t j = 0; // Iterador del Buffer 'buff2'
            col = 0;
            while (i <= buff.size()){
               // Copiar en Buffer 2
               // Si no se trata de un SEPARADOR o del último caracter
               if ((buff[i] != SEPARADOR)&&(i < buff.size())){
                  buff2[j] = buff[i];
                  j++;
               // Introducir numero en la matriz
               }else if (!IsEmpty(buff2, BUFFSIZE)){
                  mat->Set(fila, col, atoi(buff2));
                  col++;
                  ResetChar (buff2, BUFFSIZE);
                  j = 0;
               }
               i++;
            }
            fila++;
         }*/
      }
      cout << *ins << endl;   // DEBUG (mostramos la última instancia generada)
   }
}

int main (){
    //Instancia* ins = new Instancia("P_0", 150, 120, 52);
    Problema* prob = new Problema(10);
    prob->ParseFile("data/BPP10.txt");
    return 0;
}

