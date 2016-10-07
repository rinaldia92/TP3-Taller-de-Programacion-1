#include "clientServerproxy.h"

Serverproxy::Serverproxy():socket(0,0){
  (this->buffer)[0]=0;
}

int Serverproxy::Connect(char *hostname, char *port){
	int s;
	s = this->socket.Connect(hostname, port);
	if (s != 0){
		this->socket.Shutdown();
		return 1;
	}
	return 0;
}

int Serverproxy::Send(char* buffer, int size){
	this->socket.Send(buffer, size);
	return 0;
}

int Serverproxy::Receive(){
	int s = 0;

	s = this->socket.Receive(this->buffer, strlen(this->buffer));
  std::cout<< this->buffer;
	return s;
}

Serverproxy::~Serverproxy(){
	this->socket.Shutdown();
}
