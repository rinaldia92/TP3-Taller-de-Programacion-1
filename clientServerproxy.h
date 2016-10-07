#ifndef CLIENT_H
#define CLIENT_H

#include "commonsocket.h"
#include <iostream>
#define MAXLENGTH 2000

class Serverproxy{
    public:
      Serverproxy();//Constructor
      int Connect(char *hostname,char *port);
      int Receive();
      int Send(char* buffer, int size);
      //Destruye los elementos de la estructura
      virtual ~Serverproxy();
    protected:
    private:
      //Socket *socket;
      Socket socket;
    	char buffer[MAXLENGTH];
};

#endif
