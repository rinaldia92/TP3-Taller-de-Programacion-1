#ifndef CLIENTACCPETOR_H
#define CLIENTACCPETOR_H

#include "commonThread.h"
#include "commonsocket.h"
#include "serverProcess.h"
#include "serverClientproxy.h"
#include <mutex>
#include <vector>

class Clientacceptor: public Thread {
    public:
        Clientacceptor(char *port,char *file1,char *file2);//Constructor
        //Llama a ListenConnections
        virtual void run();
        //Finaliza todo
        void Terminate();
        //Destruye los elementos de la estructura
        virtual ~Clientacceptor();

    private:
        Socket socket;
        Process process;
        //Process process;
        std::mutex m;
        char *port;
        bool proceed;
        bool deletedclients;
        /*Escucha a los clientes y genera nuevos sockets para cada nueva
        conexion y los lanza en un nuevo hilo*/
        void ListenConnections(char *port);

        //Realiza el join de todos los hilos y elimina los clientproxys
        void DeleteClients();
        std::vector<Thread*> threads;
};



#endif
