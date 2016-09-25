#ifndef SOCKET_H
#define SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>


class Socket{
    public:
      Socket(int type);//Constructor
      int SetSocket(int value);
      int GetSocket();
      /*Seteamos en que puerto queremos escuchar y cuantos clientes podemos mantener
      en espera antes de aceptar a alguno*/
      int BindAndListen(char* port);
      //Elije la direccion que mejor funcione para poder conectarse
      int Connect(const char* host_name, char* port);
      //Acepta un cliente
      int Accept(socket_t* accepted_socket);
      //Envia el buffer a traves de la red
      int Send(const char* buffer, size_t length);
      //Recibe informacion a traves de la red y lo guarda en el buffer
      int Receive(char* buffer, size_t length);
      //Cierra la conexion del socket
      void Shutdown();
      //Destruye los elementos de la estructura
      virtual ~Socket();
    protected:
    private:
      int socket;
    	struct addrinfo hints;
};

#endif
