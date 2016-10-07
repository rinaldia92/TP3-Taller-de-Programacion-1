#include "serverClientproxy.h"


Clientproxy::Clientproxy():socket(0,AI_PASSIVE){
	(this->buffer)[0]=0;
}

int Clientproxy::Connect(char *port){
	int s;
  s = this->socket.BindAndListen(port);
	if (s != 0){
		this->socket.Shutdown();
		return 1;
	}
	this->peerskt = this->socket.Accept();

	return 0;
}

int Clientproxy::Send(char* buffer, int size){
  this->peerskt->Send(buffer,size);
	return 0;
}

int Clientproxy::Receive(){
	this->peerskt->Receive(this->buffer, strlen(this->buffer));
  std::cout<< this->buffer;
	return 0;
}

Clientproxy::~Clientproxy(){
	this->socket.Shutdown();
  this->peerskt->Shutdown();
}
