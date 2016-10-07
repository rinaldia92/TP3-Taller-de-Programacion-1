#ifndef CLIENTPROCESS_H
#define CLIENTPROCESS_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <vector>
#include <stdint.h>

class Process{
    public:
      Process();//Constructor
      void ProcesarArch(std::string archivo,char *buffer);
      //Destruye los elementos de la estructura
      virtual ~Process();
    protected:
    private:
};

#endif
