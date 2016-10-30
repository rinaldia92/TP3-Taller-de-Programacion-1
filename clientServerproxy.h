#ifndef CLIENT_H
#define CLIENT_H

#include "commonsocket.h"
#include <iostream>
#include <tuple>
#include <arpa/inet.h>
#define COMMANDSIZE 1
#define MAXANSWER 8
#define MINANSWER 4
#define MAXCOMMAND 13
#define COMMANDASIZE 8
#define COMMANDOTHERSSIZE 12
#define A 'A'
#define F 'F'
#define L 'L'
#define R 'R'

class Serverproxy{
    public:
        Serverproxy();//Constructor
        int Connect(char *hostname,char *port);
        //Recibe la respuesta y la muestra
        int ReceiveAnswer();
        //Procesa la tupla y lo envia
        int SendCommand
        (std::tuple<unsigned char,time_t,uint32_t,uint32_t> command);
        //Destruye los elementos de la estructura
        virtual ~Serverproxy();
    protected:
    private:
        Socket socket;
        unsigned char bufferanswer[MAXANSWER];
        unsigned char buffercommand[MAXCOMMAND];
        unsigned char command;
};

#endif
