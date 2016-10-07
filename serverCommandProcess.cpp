#include "serverCommandProcess.h"

using namespace std;

CommandProcess::CommandProcess(){

}

int CommandProcess::ComandoA(){

}

int CommandProcess::ComandoF(map<tuple<int, int>, int> tiempoparadas,
    vector<int> paradas, int parada, time_t consulta, time_t salida){
    int i;
    i = TiempoHastaParada (paradas,parada);
    if (i == -1){

    }else{
        return i - (consulta - salida);
    }
}

int CommandProcess::ComandoL(){

}

int CommandProcess::ComandoR(){

}

int CommandProcess::TiempoHastaParada(map<tuple<int, int>, int> tiempoparadas,vector<int> paradas, int parada){
    int i = 0;
    int acum = 0;
    bool encontrado = false;
    int size = paradas.size();

    while (!encontrado && i < size){
        if (paradas[i] = parada)
            encontrado = true;
        else
            acum += tiempoparadas[make_tuple<paradas[i],paradas[i+1]>];
    }

    if (!encontrado)
        return -1;

    return acum;
}

virtual CommandProcess::~CommandProcess(){

}
