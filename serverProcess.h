#ifndef SERVERPROCESS_H
#define SERVERPROCESS_H

#include <vector>
#include <map>
#include <tuple>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <stdint.h>
#include <mutex>

#define MAXTIME 999999
#define COMMA 'A'
#define COMMF 'F'
#define COMML 'L'
#define COMMR 'R'
#define RESULTADOA '0'
#define RESULTADOF '2'
#define RESULTADOL '3'
#define RESULTADOR '4'
#define INVALIDVALUE -1
#define FIRSTSTOP -1

class Process{
    public:
      Process(char *file1,char *file2);//Constructor
      //Procesa el archivo de tiempos entre paradas
      void ProcesarArchTiempos();
      //Procesa el archivo de paradas por linea
      void ProcesarArchParadas();
      //Resuelve la consulta realizada
      std::tuple<unsigned char,uint32_t,uint32_t> ResolverConsulta
      (std::tuple<unsigned char,time_t,uint32_t,uint32_t> consulta);
      //Destruye los elementos de la estructura
      virtual ~Process();

    private:
      std::string archivotiempos; //tiempos entre paradas
      std::string archivoparadas; //paradas de cada linea
      //map con clave tupla de paradas y valor el tiempo que tarda
      std::map<std::tuple<int, int>, time_t> tiempoparadas;
      std::map<int,std::vector<int>> paradasporlinea;
      //Colectivos en circulacion de cada linea
      std::map<int,std::vector<time_t>> lineasencirc;
      /*Setea linea de colectivo que entro en circulacion
       y en que momento lo hizo*/
      std::tuple<unsigned char,uint32_t,uint32_t> ComandoA
      (int linea,time_t tiempo);
      /*Devuelve el tiempo que falta para que llegue
      el siguiente colectivo de cierta linea*/
      std::tuple<unsigned char,uint32_t,uint32_t> ComandoF
      (int linea, int parada, time_t consulta);
      /*Devuelve la linea de colectivo que menos tarda y el tiempo entre
      parada1 y parada2 independientemente de si alguno este en circulacion*/
      std::tuple<unsigned char,uint32_t,uint32_t> ComandoL(int p1, int p2);
      /*Devuelve la linea de colectivo que menos tarda y el tiempo entre
      parada1 y parada2 dependiendo de si alguno esta en circulacion*/
      std::tuple<unsigned char,uint32_t,uint32_t> ComandoR
      (int p1, int p2, time_t consulta);
      /*Calcula el tiempo que tarda cierta linea entre dos paradas.
        Si la parada 1 es NULL considera desde el inicio del recorrido.
        Si alguna parada no se encuentra dentro de esa linea devuelve -1*/
      time_t TiempoHastaParada(int linea, int p1, int p2);
      std::mutex m;
};

#endif
