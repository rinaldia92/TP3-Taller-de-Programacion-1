#include "clientServerproxy.h"

#define EXITO 0
#define ERROR_CONEXION 1
#define ARGUMENTOS_INVALIDOS 2

int main(int argc, char *argv[]){
  int s;
  char mensaje[25] = "Enviando mensaje \n";

  Serverproxy client;

  s = client.Connect(argv[1],argv[2]);

  if (s!=0){
    return ERROR_CONEXION;
  }

  client.Send(mensaje,strlen(mensaje));
  s = 0;
  while (s == 0){
    s = client.Receive();
  }
  return 0;
}
