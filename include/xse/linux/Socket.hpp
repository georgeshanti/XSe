#ifndef XSE_LINUX_SOCKET
#define XSE_LINUX_SOCKET

#include<sys/socket.h>
#include<iostream>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>

#include<xse/linux/Connection.hpp>
#include<xse/exceptions/NetworkException.hpp>

#define IPV4 AF_INET
#define IPV6 AF_INET6

#define TCP SOCK_STREAM
#define UPD SOCK_DGRAM

namespace XSe{
	class Socket{
		private:
			int opt;
			int socketFD; 
			struct sockaddr_in address; 
			int addrlen; 
			int port;
		public:
			Socket(){}
			Socket(int domain, int protocol, int port){
				this->opt = 1;
				this->addrlen = sizeof(address);
				this->port = port;
				if( (this->socketFD = socket(IPV4, TCP, 0)) == 0 )
					throw NetworkException("Cannot create socket");
				if( setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &(this->opt), sizeof(this->opt)) )
					throw NetworkException("Cannot set socket options");
				address.sin_family = AF_INET;
				address.sin_addr.s_addr = INADDR_ANY;
				address.sin_port = htons( this-> port);
				if (bind(this->socketFD, (struct sockaddr *)&address, sizeof(address))<0)
					throw NetworkException("Cannot bind socket to port");
			}

			void listen(){
				if (::listen(this->socketFD, 3) < 0)
					throw NetworkException("Cannot listen on socket");
			}

			XSe::Connection accept(){
				int connection;
				if ((connection = ::accept(this->socketFD, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
					return Connection(0);
				return XSe::Connection(connection);
			}
	};
}

#endif