#include<xse/HttpServer.hpp>

namespace XSe{
	HttpServer::HttpServer(int _port):port(_port){
		// Open the port on initialization
		this->socket = XSe::Socket(IPV4, TCP, port);
	}

	void HttpServer::listen(){
		// Begin accepting connections
		this->socket.listen();

		// Start a loop to keep accepting connections
		while(true){
			// Establish a connection
			XSe::Connection connection = this->socket.accept();

			// Push Connection to parser queue
			this->parserQueue.push(this->parserGenerator(connection));
		}
	}

	std::function<void()> HttpServer::parserGenerator(Connection connection){
		return [this, connection](void){
			// Create connection buffer, empty request and response objects
			char buffer[1024];
			XSe::Request request;
			XSe::Response response(connection);			// The TCP connection is passed to response object
			int len;									// so that it can write itself back to the conenction

			// Start reading from connection into buffer as long as data is being received (len>0)
			while((len=connection.receive(buffer, 1023), len>0)){
				// Null terminate buffer and pass to request object for parsing
				buffer[len] = '\0';

				// Parse buffer into request object and check if request as reach a final or irrecoverable state
				if(request.parse(buffer)==1){
					// Exit from loop if such a state is returned
					break;
				}
			}

			// Check validity of the request object
			if(request.valid){
				// Call request handler if valid
				this->requestHandler(request, response);
			}

			// Close the connection
			connection.close();
		};
	}
}