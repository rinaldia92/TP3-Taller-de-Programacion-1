#include "serverFileProcess.h"

FileProcess::FileProcess(char *file1,char *file2){
  archivotiempos = file1;
  archivoparadas = file2;
}
void Process::ProcesarArchTiempos(){
    std::string line;
    int parada1, parada2, tiempo;

    std::ifstream tiepoentreparadas(archivotiempos);

    while (getline(tiepoentreparadas,line)){
        std::stringstream iss(line);
        iss >> parada1 >> parada2 >> tiempo;
        tiempoparadas[std::make_tuple(parada1,parada2)] = tiempo;
    }
}

int FileProcess::ObtenerTiempo(int parada1, int parada2){
    return tiempoparadas[std::make_tuple(parada1,parada2)];
}

void FileProcess::ProcesarArchParadas(){
    std::string line;
    std::vector <int> auxparadas;
    int linea,parada;
    int i = 0;

    std::ifstream paradas(archivoparadas);

    while (getline(paradas,line)){
        std::stringstream iss(line);
        iss >> linea;
        lineas[linea] = i;

        while (iss >> parada){
            auxparadas.push_back(parada);
        }
        paradasporlineas.push_back(auxparadas);
        auxparadas.clear();
        i++;
    }
}

std::vector<int> FileProcess::ObtenerParadas(int linea){
    return paradasporlineas[lineas[linea]];
}

//Destruye los elementos de la estructura
FileProcess::~FileProcess(){
}
