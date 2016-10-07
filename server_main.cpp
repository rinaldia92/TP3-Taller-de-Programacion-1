#include "serverClientproxy.h"

#define EXITO 0
#define ERROR_CONEXION 1
#define ARGUMENTOS_INVALIDOS 2

int main(int argc, char *argv[]){
  int s;
  char mensaje[25] = "Mensaje recibido \n";

  Clientproxy server;

  s = server.Connect(argv[1]);

  if (s!=0){
    return ERROR_CONEXION;
  }

  s = server.Receive();
  server.Send(mensaje,strlen(mensaje));

  return 0;
}
