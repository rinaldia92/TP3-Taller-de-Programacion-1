#include "clientServerproxy.h"
#include "clientProcess.h"
#include <tuple>
#include <string>


#define EXITO 0
#define ERROR_CONEXION 1
#define ARGUMENTOS_INVALIDOS 2
#define DEFAULT 0

int main(int argc, char *argv[]){
    int s;
    std::string archivo;
    std::tuple<unsigned char,time_t,uint32_t,uint32_t> consulta(0,0,0,0);
    Serverproxy serverproxy;
    Process proc;

    s = serverproxy.Connect(argv[1],argv[2]);

    if (s != 0){
        return ERROR_CONEXION;
    }

    while (getline(std::cin, archivo)){
        consulta = proc.ProcesarArch(archivo);
        serverproxy.SendCommand(consulta);
        serverproxy.ReceiveAnswer();
    }

    return 0;
}
