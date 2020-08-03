#ifndef XSE_XSE_H
#define XSE_XSE_H

#include<functional>
#include<map>

#include<xse/HttpServer.hpp>
#include<xse/HttpMethod.hpp>
#include<xse/HttpStatus.hpp>
#include<xse/Request.hpp>
#include<xse/Response.hpp>

namespace XSe{

	typedef std::map<XSe::HttpMethod, XSe::Handler> MethodHandler;

	class XServer{
		private:
			int port;
			HttpServer httpServer;
			std::map<std::string, XSe::MethodHandler> routes;
			std::map<std::string, std::string> staticRoutes;
			std::map<XSe::HttpStatus, XSe::Handler> statusRoutes;
			XSe::Handler handler;
		public:
			XServer(int _port);
			void start();
			void setStatusRoute(XSe::HttpStatus status, XSe::Handler _handler){ this->statusRoutes[status]=_handler; }
			void route(std::string _route, std::vector<XSe::HttpMethod> methods, XSe::Handler _handler){
				std::map<std::string, XSe::MethodHandler>::iterator routeHandler = this->routes.find(_route);
				if(routeHandler == this->routes.end()){
					this->routes[_route] = XSe::MethodHandler();
					routeHandler = this->routes.find(_route);
				}
				for(auto method: methods){
					this->routes[_route][method] = _handler;
				}
			}
			void serve(std::string _dest, std::string _src){ this->staticRoutes[_dest] = _src; }
			void setHandler(XSe::Handler handler){ this->handler = handler; }
			XSe::Handler getHandler(){ return this->handler; }
	};

	static std::map<std::string, std::string> FILETYPES = {
		{"html","text/html"},
		{"js","application/javascript"},
		{"css","text/css"},
		{"apng","image/apng"},
		{"bmp","image/bmp"},
		{"gif","image/gif"},
		{"ico","image/x-icon"},
		{"jpg","image/jpeg"},
		{"png","image/png"},
		{"svg","image/svg+xml"},
		{"tif","image/tiff"},
		{"webp","image/webp"}
	};
}

#endif