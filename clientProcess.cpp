#include "clientProcess.h"


Process::Process(){
}

void Process::ProcesarArch(std::string archivo,char *buffer){
  std::string fecha;
  uint_32 linea;
  char comando;
  struct tm time = {0};
  time_t tiempo;


  std::cout<<archivo<<std::endl;
  std::istringstream iss(archivo);
  iss >> fecha;

  time.tm_year = stoi(fecha.substr(7,4)) - 1900;
  time.tm_mon = stoi(fecha.substr(4,2)) - 1;
  time.tm_mday = stoi(fecha.substr(1,2));
  time.tm_hour = stoi(fecha.substr(12,2));
  time.tm_min = stoi(fecha.substr(15,2));
  time.tm_sec = stoi(fecha.substr(18,2));

  tiempo = timegm(&time);

  buffer[1] = (unsigned char)((linea >> 0) & 0xFF);
  buffer[2] = (unsigned char)((linea >> 8) & 0xFF);
  buffer[3] = (unsigned char)((linea >> 16) & 0xFF);
  buffer[4] = (unsigned char)((linea >> 24) & 0xFF);

  iss >> comando;
  buffer[0] = comando;

  iss >> linea;
  buffer[5] = (unsigned char)((linea >> 0) & 0xFF);
  buffer[6] = (unsigned char)((linea >> 8) & 0xFF);
  buffer[7] = (unsigned char)((linea >> 16) & 0xFF);
  buffer[8] = (unsigned char)((linea >> 24) & 0xFF);

}

Process::~Process(){

}
