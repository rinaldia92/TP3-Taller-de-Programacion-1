#ifndef CLIENTPROCESS_H
#define CLIENTPROCESS_H


#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <cstdint>
#include <tuple>
#include <string>

class Process{
    public:
      Process();//Constructor
      /*Devuelve una tupla con los valores correspondientes al comando.
        Deja un 0 en la ultima posicion si se trata del comando A*/
      std::tuple<unsigned char,time_t,uint32_t,uint32_t> ProcesarArch
      (std::string archivo);
      //Destruye los elementos de la estructura
      virtual ~Process();
    protected:
    private:
};

#endif
