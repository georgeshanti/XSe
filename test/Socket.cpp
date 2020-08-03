#include<xse/Socket.hpp>

int main(){
	char buffer[1024], mes[]="Hello world!";
	XSe::Socket socket(IPV4, TCP, 8080);
	socket.listen();
	XSe::Connection connection = socket.accept();
	int len = connection.receive(buffer, 1024);
	std::cout<<buffer;
	connection.send(mes, strlen(mes));
}