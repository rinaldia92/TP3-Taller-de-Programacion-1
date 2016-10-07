#ifndef SERVER_H
#define SERVER_H

#include "commonsocket.h"
//#include "Thread.h"
#include <iostream>
#define MAXLENGTH 2000


class Clientproxy{
    public:
      Clientproxy();//Constructor
      int Connect(char *port);
      int Receive();
      int Send(char* buffer, int size);
      //Destruye los elementos de la estructura
      virtual ~Clientproxy();
    protected:
    private:

      Socket socket, *peerskt;
    	char buffer[MAXLENGTH];
};

#endif
