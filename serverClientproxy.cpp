#include "serverClientproxy.h"
#include <tuple>

Clientproxy::Clientproxy(Socket peerskt,Process &process,std::mutex &m):
peerskt(std::move(peerskt)),process(process),m(m){
	buffercommand[0]=0;
	bufferanswer[0]=0;
	command = CONSULTAINICIAL;
}

int Clientproxy::SendAnswer(std::tuple<unsigned char,uint32_t,uint32_t> answer){
	int size, i = 0;
    uint32_t parametro1,parametro2;
    unsigned char comando;
    comando = std::get<0>(answer);
    parametro1 = std::get<1>(answer);
    parametro2 = std::get<2>(answer);

	parametro1 = htonl(parametro1);
	parametro2 = htonl(parametro2);

    if ((comando == RESULTADOA)||(comando == RESULTADOF))
        size = MINANSWER;
    else
		if ((comando == RESULTADOL)||(comando == RESULTADOR))
        	size = MAXANSWER;
		else
			size = COMMANDSIZE;

    bufferanswer[0] = comando;

	for (i = 0; i < 4; i++){
		bufferanswer[1 + i] = (unsigned char)((parametro1 >> (8*(3-i))) & 0xFF);
		bufferanswer[5 + i] = (unsigned char)((parametro2 >> (8*(3-i))) & 0xFF);
	}

  	peerskt.Send(bufferanswer,size);
	return 0;
}

std::tuple<unsigned char,time_t,uint32_t,uint32_t> Clientproxy::ReceiveCommand()
{
	std::tuple<unsigned char,time_t,uint32_t,uint32_t> aux(0,0,0,0);
	time_t tiempo = 0;
	uint32_t parametro1 = 0;
	uint32_t parametro2 = 0;
	int i;

	peerskt.Receive(&command, COMMANDSIZE);

	if (command == COMMA){
		peerskt.Receive(buffercommand, COMMANDASIZE);
		for (i = 0; i < 4; i++){
			tiempo = (tiempo << 8) + buffercommand[0+i];
	        parametro1 = (parametro1 << 8) + buffercommand[4+i];
	    }
		tiempo = ntohl(tiempo);
		parametro1 = ntohl(parametro1);
	} else{
		if ((command == COMMF)||(command == COMML)||(command == COMMR)){
			peerskt.Receive(buffercommand, COMMANDOTHERSSIZE);
			for (i = 0; i < 4; i++){
				tiempo = (tiempo << 8) + buffercommand[0+i];
	        	parametro1 = (parametro1 << 8) + buffercommand[4+i];
	        	parametro2 = (parametro2 << 8) + buffercommand[8+i];
	    	}
			tiempo = ntohl(tiempo);
			parametro1 = ntohl(parametro1);
			parametro2 = ntohl(parametro2);
		} else {
			command = CONSULTAINICIAL;
		}
	}
	std::get<0>(aux) = command;
	std::get<1>(aux) = tiempo;
	std::get<2>(aux) = parametro1;
	std::get<3>(aux) = parametro2;
	command = CONSULTAINICIAL;
	return aux;
}

void Clientproxy::run(){
	std::tuple<unsigned char,time_t,uint32_t,uint32_t> consulta(0,0,0,0);
    std::tuple<unsigned char,uint32_t,uint32_t> respuesta(0,0,0);

	consulta = this->ReceiveCommand();
    while (std::get<0>(consulta) != CONSULTAINICIAL) {
		std::cerr << "Comando " << std::get<0>(consulta)
		<< " recibido." << std::endl;
		m.lock();
        respuesta = process.ResolverConsulta(consulta);
		m.unlock();
        this->SendAnswer(respuesta);
        consulta = this->ReceiveCommand();
    }

	//peerskt.Shutdown();
	std::cerr << "Cliente desconectado." << std::endl;
}

Clientproxy::~Clientproxy(){
 	peerskt.Shutdown();
}
