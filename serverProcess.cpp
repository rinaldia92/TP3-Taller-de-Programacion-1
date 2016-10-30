#include "serverProcess.h"
#include <vector>
#include <map>
#include <tuple>
#include <string>

Process::Process(char *file1,char *file2){
  archivotiempos = file1;
  archivoparadas = file2;
}

void Process::ProcesarArchTiempos(){
    std::string line;
    int parada1, parada2;
    time_t tiempo;
    std::tuple<int,int> aux(0,0);

    std::ifstream tiepoentreparadas(archivotiempos);

    while (getline(tiepoentreparadas,line)){
        std::stringstream iss(line);
        iss >> parada1 >> parada2 >> tiempo;
        std::get<0>(aux) = parada1;
        std::get<1>(aux) = parada2;
        tiempoparadas[aux] = tiempo;
    }
}

void Process::ProcesarArchParadas(){
    std::string line;
    int linea,parada;

    std::ifstream paradas(archivoparadas);

    while (getline(paradas,line)){
        std::stringstream iss(line);
        iss >> linea;

        while (iss >> parada){
            paradasporlinea[linea].push_back(parada);
        }
    }
}

std::tuple<unsigned char,uint32_t,uint32_t> Process::ResolverConsulta
    (std::tuple<unsigned char,time_t,uint32_t,uint32_t> consulta){
    unsigned char comando = std::get<0>(consulta);
    time_t tiempo = std::get<1>(consulta);
    uint32_t parametro1 = std::get<2>(consulta);
    uint32_t parametro2 = std::get<3>(consulta);

    std::tuple<unsigned char,uint32_t,uint32_t> consultaresuelta(0,0,0);

    m.lock();

    switch (comando) {
        case COMMA:
            consultaresuelta = ComandoA(parametro1,tiempo);
            break;
        case COMMF:
            consultaresuelta = ComandoF(parametro1,parametro2,tiempo);
            break;
        case COMML:
            consultaresuelta = ComandoL(parametro1,parametro2);
            break;
        case COMMR:
            consultaresuelta = ComandoR(parametro1,parametro2,tiempo);
            break;
        default:
            consultaresuelta = std::make_tuple(255,0,0);
            break;
    }

    m.unlock();

    return consultaresuelta;
}


std::tuple<unsigned char,uint32_t,uint32_t> Process::ComandoA(int linea,
                                                        time_t tiempo){
    std::tuple<unsigned char,uint32_t,uint32_t> aux(0,0,0);
    lineasencirc[linea].push_back(tiempo);

    std::get<0>(aux) = RESULTADOA;
    std::get<1>(aux) = linea;

    return aux;
}

std::tuple<unsigned char,uint32_t,uint32_t> Process::ComandoF
                    (int linea, int parada,time_t consulta){
    time_t tiempohasta, tiempocomp;
    std::vector<time_t> circulando;
    std::tuple<unsigned char,uint32_t,uint32_t> aux(0,0,0);
    unsigned int i = 0;

    circulando = lineasencirc[linea];
    tiempohasta = TiempoHastaParada(linea,FIRSTSTOP,parada);
    if (tiempohasta != INVALIDVALUE){
        tiempocomp = MAXTIME;
        while ((i < circulando.size()) && (tiempocomp > tiempohasta)){
            tiempocomp = consulta - circulando[i];
            i++;
        }

        std::get<0>(aux) = RESULTADOF;

        if (tiempocomp <= tiempohasta){
            std::get<1>(aux) = tiempohasta - tiempocomp;
            return aux;
        } else {
            std::get<1>(aux) = MAXTIME;
            return aux;
        }
    } else {
        std::get<0>(aux) = 255;
        return aux;
    }
}

std::tuple<unsigned char,uint32_t,uint32_t> Process::ComandoL(int p1, int p2){
    int linea = 0;
    int lineaaux;
    time_t tiempo = 0;
    time_t tiempominimo = MAXTIME;
    std::map<int,std::vector<int>>::iterator it;
    std::tuple<unsigned char,uint32_t,uint32_t> aux(0,0,0);

    for (it = paradasporlinea.begin(); it != paradasporlinea.end(); it++){
        lineaaux = it->first;
        tiempo = TiempoHastaParada(lineaaux,p1,p2);
        if ((tiempo != INVALIDVALUE) && (tiempo < tiempominimo)){
            linea = lineaaux;
            tiempominimo = tiempo;
        }
    }

    std::get<0>(aux) = RESULTADOL;
    std::get<1>(aux) = linea;
    std::get<2>(aux) = tiempominimo;
    return aux;
}

std::tuple<unsigned char,uint32_t,uint32_t> Process::ComandoR(int p1, int p2,
                                                        time_t consulta){
    int linea = 0;
    int lineaaux;
    unsigned int i;
    time_t tiempo, tiempo1,tiempo2,tiempofaltante;
    time_t tiempominimo = MAXTIME;
    std::vector<time_t> circulando;
    std::map<int,std::vector<int>>::iterator it;
    std::tuple<unsigned char,uint32_t,uint32_t> aux(0,0,0);

    for (it = paradasporlinea.begin(); it != paradasporlinea.end(); it++){
        i = 0;
        lineaaux = it->first;
        tiempo1 = TiempoHastaParada(lineaaux,FIRSTSTOP,p1);
        tiempo2 = TiempoHastaParada(lineaaux,p1,p2);
        if (tiempo1 != INVALIDVALUE && tiempo2 != INVALIDVALUE){
            circulando = lineasencirc[lineaaux];
            tiempofaltante = MAXTIME;
            while ((i < circulando.size()) && (tiempofaltante > tiempo1)){
                tiempofaltante = consulta - circulando[i];
                i++;
            }
            if (tiempofaltante <= tiempo1){
                tiempo = tiempo1 + tiempo2 - tiempofaltante;
                if (tiempo < tiempominimo){
                    linea = lineaaux;
                    tiempominimo = tiempo;
                }
            }
            circulando.clear();
        }
    }

    std::get<0>(aux) = RESULTADOR;
    std::get<1>(aux) = linea;
    std::get<2>(aux) = tiempominimo;

    return aux;
}

time_t Process::TiempoHastaParada(int linea, int p1, int p2){
    int i = 0;
    time_t acum = 0;
    bool encontrado = false;
    std::vector<int> paradas = paradasporlinea[linea];
    int size = paradas.size();
    std::tuple<int,int> auxparadas(0,0);

    if (p1 != FIRSTSTOP){
        while ((!encontrado) && (i < size)){
            if (paradas[i] == p1)
                encontrado = true;
            else
                i++;
        }
        if (!encontrado){
            return INVALIDVALUE;
        } else {
            encontrado = false;
        }
    }

    while ((!encontrado) && (i < size)){
        if (paradas[i] == p2){
            encontrado = true;
        }else{
            if (i != size -1){
                std::get<0>(auxparadas) = paradas[i];
                std::get<1>(auxparadas) = paradas[i+1];
                acum = acum + tiempoparadas[auxparadas];
            }
            i++;
        }
    }

    if (!encontrado){
        return INVALIDVALUE;
    } else {
        return acum;
    }
}

//Destruye los elementos de la estructura
Process::~Process(){
}
