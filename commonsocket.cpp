#include "commonsocket.h"

#define MAXLISTEN 20

Socket::Socket(int fd, int type){
  this->skt = fd;
  memset(&this->hints, 0, sizeof(struct addrinfo));
 	this->hints.ai_family = AF_INET;       /* IPv4 (or AF_INET6 for IPv6)     */
 	this->hints.ai_socktype = SOCK_STREAM; /* TCP  (or SOCK_DGRAM for UDP)    */
 	this->hints.ai_flags = type;     /* None (or AI_PASSIVE for server) */
}

int Socket::SetSocket(int value){
  this->skt = value;
  return 0;
}

int Socket::GetSocket(){
  return this->skt;
}

int Socket::BindAndListen(char* port){
   int s;

   struct addrinfo *ptr;

   s = getaddrinfo(NULL, port, &(this->hints), &ptr);

   this->skt = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

   s = bind(this->skt, ptr->ai_addr, ptr->ai_addrlen);
   if (s == -1) {
      close(this->skt);
      freeaddrinfo(ptr);
      return 1;
   }

   freeaddrinfo(ptr);

   s = listen(this->skt, MAXLISTEN);
   if (s == -1) {
      close(this->skt);
      return 1;
   }

   return 0;
}

int Socket::Connect(const char* host_name, char* port){
   int s;
   bool are_we_connected;

   struct addrinfo *result, *ptr;

   are_we_connected = false;

   s = getaddrinfo(host_name, port, &(this->hints), &result);

   for (ptr = result; ptr != NULL && !are_we_connected; ptr = ptr->ai_next) {
      this->skt = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

      if (this->skt == -1) {
         return 1;
      }else{
         s = connect(this->skt, ptr->ai_addr, ptr->ai_addrlen);
         if (s == -1) {
            close(this->skt);
            return 1;
         }
         are_we_connected = (s != -1);
      }
   }
   freeaddrinfo(result);
   return 0;
}
//
// int Socket::Accept(Socket &accepted_socket){
// //int Socket::Accept(){
//    accepted_socket.SetSocket(accept(this->skt, NULL, NULL));
//
//    if (accepted_socket.GetSocket() == -1)
//       return -1;
//
//
//    return 0;
// }

Socket* Socket::Accept(){
    int SocketFD = accept(this->skt, NULL, NULL);

    if (SocketFD == -1){
		    std::cerr << "ERROR AL ACEPTAR CONEXION. "
					<< gai_strerror(SocketFD) << std::endl;
		    //return NULL;
	  }
	  return new Socket(SocketFD,AI_PASSIVE);
}

int Socket::Send(const char* buffer, size_t length){
   int s;
   unsigned int bytes_sent;
   bool socket_error, remote_socket_closed;

   socket_error = false;
   remote_socket_closed = false;

   bytes_sent = 0;

   while (bytes_sent < length && !socket_error && !remote_socket_closed) {
      s = send(this->skt, &buffer[bytes_sent],
          length - bytes_sent, MSG_NOSIGNAL);

      if (s < 0) {
         socket_error = true;
      }else if (s == 0) {
         remote_socket_closed = true;
      }else {
         bytes_sent += s;
      }
   }

   if (!remote_socket_closed && !socket_error) {
      return bytes_sent;
   }else {
      return -1;
   }
}

int Socket::Receive(char* buffer, size_t length){
  int s = 1;
  char aux;
  strncpy(&aux,"",1);
  while (strcmp(&aux,"\n")!=0 && s>0){
    s = recv(this->skt,&aux, 1, MSG_NOSIGNAL);
    strncat(buffer,&aux,1);
  }

  return s;
}

// Socket::Socket(Socket&& other) {
//     this->skt = std::move(other.skt);
// }
//
//
// Socket& Socket::operator=(Socket&& other) {
//     this->skt = std::move(other.skt);
//     return *this;
// }
//

void Socket::Shutdown() {
   shutdown(this->skt, SHUT_RDWR);
}

Socket::~Socket(){
  this->Shutdown();
  close(this->skt);
}
