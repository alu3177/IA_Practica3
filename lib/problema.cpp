#include "problema.hpp"

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

                //cout << "_nInstancias = " << _nInstancias << endl;   // DEBUG
                //cout << "buff = " << buff << endl;   // DEBUG

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
                    //cout << *ins << endl;   // DEBUG (mostramos la instancia generada)
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
        }
        //cout << *ins << endl;   // DEBUG (mostramos la última instancia generada)
    }
}
