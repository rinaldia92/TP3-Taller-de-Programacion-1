#include "clientServerproxy.h"
#include <tuple>

Serverproxy::Serverproxy():socket(0){
  bufferanswer[0]=0;
  buffercommand[0]=0;
}

int Serverproxy::Connect(char *hostname, char *port){
	int s;
	s = socket.Connect(hostname, port);
	if (s != 0){
		socket.Shutdown();
		return 1;
	}
	return 0;
}

int Serverproxy::SendCommand
(std::tuple<unsigned char,time_t,uint32_t,uint32_t> command){
    int size, i = 0;
    uint32_t parametro1,parametro2;
    time_t tiempo;
    unsigned char comando;
    comando = std::get<0>(command);
    tiempo = std::get<1>(command);
    parametro1 = std::get<2>(command);
    parametro2 = std::get<3>(command);

    if (comando == A)
        size = COMMANDASIZE + COMMANDSIZE;
    else
        size = COMMANDOTHERSSIZE + COMMANDSIZE;

    buffercommand[0] = comando;
    for (i = 0; i < 4; i++){
        buffercommand[1+i] = (unsigned char)((tiempo >> (8*(3-i))) & 0xFF);
        buffercommand[5+i] = (unsigned char)((parametro1 >> (8*(3-i))) & 0xFF);
        buffercommand[9+i] = (unsigned char)((parametro2 >> (8*(3-i))) & 0xFF);
    }
	socket.Send(buffercommand, size);

	return 0;
}

int Serverproxy::ReceiveAnswer(){
	int s,i, minutos, segundos;
    uint32_t parametro1 = 0;
    uint32_t parametro2 = 0;

	s = socket.Receive(&command, COMMANDSIZE);
    if ((command == '0')||(command == '2')){
        s = socket.Receive(bufferanswer, MINANSWER);
        for (i = 0; i < 4; i++){
            parametro1 = (parametro1 << 8) + bufferanswer[i];
        }
        parametro1 = ntohl(parametro1);
    }else{
        if ((command == '3')||(command == '4')) {
            s = socket.Receive(bufferanswer, MAXANSWER);
            for (i = 0; i < 4; i++){
                parametro1 = (parametro1 << 8) + bufferanswer[i];
                parametro2 = (parametro2 << 8) + bufferanswer[4+i];
            }
            parametro1 = ntohl(parametro1);
            parametro2 = ntohl(parametro2);
        }
    }

    switch (command) {
        case '0':
            std::cout << "Un colectivo de la línea " << parametro1
            << " ha sido agregado." << std::endl;
            break;
        case '2':
            minutos = parametro1/60;
            std::cout << "Faltan " << minutos
            << " minutos para que llegue el siguiente colectivo." << std::endl;
            break;
        case '3':
            minutos = parametro2/60;
            segundos = parametro2%60;
            std::cout << "La línea con el recorrido más rápido es la "
            << parametro1 << ", que tarda " << minutos << " minutos y "
            << segundos << " segundos en llegar a destino." << std::endl;
            break;
        case '4':
            minutos = parametro2/60;
            segundos = parametro2%60;
            std::cout << "El colectivo de la línea "
            << parametro1 << " tardará " << minutos << " minutos y "
            << segundos << " segundos en llegar a destino." << std::endl;
            break;
        default:
            std::cout << "Error." << std::endl;
            break;
    }
    return s;
}

Serverproxy::~Serverproxy(){
	socket.Shutdown();
}
