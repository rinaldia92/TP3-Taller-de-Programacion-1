#ifndef SERVERCOMMANDPROCESS_H
#define SERVERCOMMANDPROCESS_H

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
        CommandProcess();//Constructor
        int ComandoA();
        int ComandoF(std::map<std::tuple<int, int>, int> tiempoparadas,
        std::vector<int> paradas, int parada, time_t consulta, time_t salida);
        int ComandoL();
        int ComandoR();
        //Destruye los elementos de la estructura
        virtual ~CommandProcess();
    protected:
    private:
        //Tiempo que tarda cierta linea en llegar a x parada.
        int TiempoHastaParada(std::map<std::tuple<int, int>, int> tiempoparadas,
            std::vector<int> paradas, int parada);
};

#endif
