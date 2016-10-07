#ifndef SERVERFILEPROCESS_H
#define SERVERFILEPROCESS_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <vector>
#include <map>
#include <tuple>
#include <stdint.h>

class Process{
    public:
      FileProcess(char *file1,char *file2);//Constructor
      void ProcesarArchTiempos();
      int ObtenerTiempo(int parada1, int parada2);
      void ProcesarArchParadas();
      std::vector<int> ObtenerParadas(int linea);
      //Destruye los elementos de la estructura
      virtual ~FileProcess();
    protected:
    private:
      std::string archivotiempos; //tiempos entre paradas
      std::string archivoparadas; //paradas de cada linea
      std::map<std::tuple<int, int>, int> tiempoparadas;
      std::map<int,int> lineas; //Mapea la linea a una pos. del vector de paradas
      std::vector<std::vector<int>> paradasporlineas;
};

#endif
