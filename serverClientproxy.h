#ifndef SERVER_H
#define SERVER_H

#include "commonsocket.h"
#include "commonThread.h"
#include <iostream>
#include <tuple>
#include <arpa/inet.h>
#include <mutex>
#include "serverProcess.h"
#define COMMANDSIZE 1
#define MINANSWER 5
#define MAXANSWER 9
#define MAXCOMMAND 12
#define COMMANDASIZE 8
#define COMMANDOTHERSSIZE 12
#define COMMA 'A'
#define COMMF 'F'
#define COMML 'L'
#define COMMR 'R'
#define CONSULTAINICIAL '0'
#define RESULTADOA '0'
#define RESULTADOF '2'
#define RESULTADOL '3'
#define RESULTADOR '4'

class Clientproxy: public Thread{
    public:
      Clientproxy(Socket peerskt,Process &process,std::mutex &m);//Constructor
      //Recibe una consulta, la procesa y devuelve la respuesta.
      virtual void run();
      //Destruye los elementos de la estructura
      virtual ~Clientproxy();
    protected:
    private:
      Socket peerskt;
      //Recibe la consulta
      std::tuple<unsigned char,time_t,uint32_t,uint32_t> ReceiveCommand();
      //Envia la respuesta
      int SendAnswer(std::tuple<unsigned char,uint32_t,uint32_t> answer);
      //Buffer para recibir la consulta
      unsigned char buffercommand[MAXCOMMAND];
      //Buffer para la respuesta
      unsigned char bufferanswer[MAXANSWER];
      unsigned char command;
      //El encargado de procesar las consultas
      Process &process;
      std::mutex &m;
};

#endif
