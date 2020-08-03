#ifndef XSE_HTTP_SERVER_H
#define XSE_HTTP_SERVER_H

#include<functional>

#include<xse/Socket.hpp>
#include<xse/TaskExecutor.hpp>
#include<xse/Request.hpp>
#include<xse/Response.hpp>

namespace XSe{

	typedef std::function<void(Request, Response)> Handler;

	class HttpServer{
		private:
			Socket socket;
			int port;
			Handler requestHandler;

			TaskExecutor parserQueue;
			std::function<void()> parserGenerator(Connection);
		public:
			HttpServer(int);
			void listen();
			void setRequestHandler(Handler requestHandler){ this->requestHandler = requestHandler; }
	};
}

#endif