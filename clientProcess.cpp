#include "clientProcess.h"
#include <tuple>
#include <string>

Process::Process(){
}

std::tuple<unsigned char,time_t,uint32_t,uint32_t> Process::ProcesarArch
(std::string archivo){
      std::string fecha;
      uint32_t value,tiempo;
      unsigned char comando;
      struct tm time = {0};
      time_t tiempounix;
      std::tuple<unsigned char,time_t,uint32_t,uint32_t> aux(0,0,0,0);

      std::istringstream iss(archivo);
      iss >> fecha;

      time.tm_year = stoi(fecha.substr(7,4)) - 1900;
      time.tm_mon = stoi(fecha.substr(4,2)) - 1;
      time.tm_mday = stoi(fecha.substr(1,2));
      time.tm_hour = stoi(fecha.substr(12,2));
      time.tm_min = stoi(fecha.substr(15,2));
      time.tm_sec = stoi(fecha.substr(18,2));

      tiempounix = timegm(&time);
      tiempo = htonl(tiempounix);

      std::get<1>(aux) = tiempo;

      iss >> comando;
      std::get<0>(aux) = comando;

      iss >> value;
      value = htonl(value);
      std::get<2>(aux) = value;

      if (iss >> value){
          value = htonl(value);
          std::get<3>(aux) = value;
      }

      return aux;
}

Process::~Process(){
}
