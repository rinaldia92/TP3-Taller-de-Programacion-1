#include "serverClientproxy.h"
#include "serverClientacceptor.h"
#include <string>

#define EXITO 0
#define ERROR_CONEXION 1
#define ARGUMENTOS_INVALIDOS 2
#define QUIT "q"

int main(int argc, char *argv[]){
    bool seguir = true;
    std::string entrada;

    Clientacceptor clientacceptor(argv[1],argv[2],argv[3]);
    clientacceptor.start();

	while(seguir){
		getline(std::cin, entrada);
		if (entrada == QUIT){
			seguir = false;
			clientacceptor.Terminate();
		}
	}

    clientacceptor.join();

    return 0;
}
