#ifndef XSE_LINUX_CONNECTION
#define XSE_LINUX_CONNECTION

#include<sys/socket.h>
#include<iostream>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>

#define IPV4 AF_INET
#define IPV6 AF_INET6

#define TCP SOCK_STREAM
#define UPD SOCK_DGRAM

namespace XSe{
	class Connection{
		private:
		
		public:
			int connection;
			Connection(int conn):connection(conn){}
			bool valid(){ return this->connection!=0; }
			int receive(char *buffer, int size) const{
				return ::read( this->connection , buffer, size);
			}
			void send(const char *buffer, int size) const{
				::send( this->connection , buffer , size , 0 );
			}
			void close() const{
				::close(this->connection);
			}
	};
}

#endif