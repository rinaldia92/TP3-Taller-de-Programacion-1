#include "socket.h"

#define MAXLISTEN 20

Socket::Socket(int type){
   this->socket = 0;

  memset(&this->hints, 0, sizeof(struct addrinfo));
 	this->hints.ai_family = AF_INET;       /* IPv4 (or AF_INET6 for IPv6)     */
 	this->hints.ai_socktype = SOCK_STREAM; /* TCP  (or SOCK_DGRAM for UDP)    */
 	this->hints.ai_flags = type;     /* None (or AI_PASSIVE for server) */

   return 0;
}
int Socket::SetSocket(int value){
  this->socket = value;
  return 0;
}

int GetSocket(){
  return this->socket;
}

int Socket::BindAndListen(char* port){
   int s;

   struct addrinfo *ptr;

   s = getaddrinfo(NULL, port, &(this->hints), &ptr);

   this->socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

   s = bind(this->socket, ptr->ai_addr, ptr->ai_addrlen);
   if (s == -1) {
      close(this->socket);
      freeaddrinfo(ptr);
      return 1;
   }

   freeaddrinfo(ptr);

   s = listen(this->socket, MAXLISTEN);
   if (s == -1) {
      close(this->socket);
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
      this->socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

      if (this->socket == -1) {
         return 1;
      }else{
         s = connect(this->socket, ptr->ai_addr, ptr->ai_addrlen);
         if (s == -1) {
            close(this->socket);
            return 1;
         }
         are_we_connected = (s != -1);
      }
   }
   freeaddrinfo(result);
   return 0;
}

int Socket::Accept(Socket &accepted_socket){
   accepted_socket.SetSocket(accept(this->socket, NULL, NULL));

   if (accepted_socket.GetSocket() == -1)
      return -1;

   return 0;
}

int Socket::Send(const char* buffer, size_t length){
   int s;
   int bytes_sent;
   bool socket_error, remote_socket_closed;

   socket_error = false;
   remote_socket_closed = false;

   bytes_sent = 0;

   while (bytes_sent < length && !socket_error && !remote_socket_closed) {
      s = send(this->socket, &buffer[bytes_sent],
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
    s = recv(this->socket,&aux, 1, MSG_NOSIGNAL);
    strncat(buffer,&aux,1);
  }

  return s;
}

void Socket::Shutdown() {
   shutdown(this->socket, SHUT_RDWR);
}

Socket::~Socket(){
  socket_shutdown(this);
  close(this->socket);

	return 0;
}
