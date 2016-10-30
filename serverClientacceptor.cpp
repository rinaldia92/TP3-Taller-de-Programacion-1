#include "serverClientacceptor.h"

Clientacceptor::Clientacceptor(char *port,char *file1,char *file2):
    socket(AI_PASSIVE),process(file1,file2){
    this->port = port;
    this->proceed = true;
    this->deletedclients = false;
}

void Clientacceptor::ListenConnections(char *port){
    unsigned int i = 0;
    this->socket.BindAndListen(port);
	while (this->proceed){
		Socket peerskt = this->socket.Accept();
		if (peerskt.ValidSocket()){
            std::cerr << "Cliente conectado." << std::endl;
			threads.push_back(new Clientproxy(std::move(peerskt), process,m));
			threads[i]->start();
            i++;
		} else {
            this->proceed = false;
            this->socket.Shutdown();
        }
	}
	this->DeleteClients();
}

void Clientacceptor::Terminate(){
    this->proceed = false;
	this->DeleteClients();
	this->socket.Shutdown();
}

void Clientacceptor::DeleteClients(){
    unsigned int i;
    unsigned int size;
    size = threads.size();

    if (!this->deletedclients){
        for (i = 0; i < size; i++){
                threads[i]->join();
                delete (threads[i]);
        }
        threads.clear();
    }
    this->deletedclients = true;
}

void Clientacceptor::run(){
    process.ProcesarArchTiempos();
    process.ProcesarArchParadas();
    this->ListenConnections(port);
}

Clientacceptor::~Clientacceptor(){
    this->DeleteClients();
}
