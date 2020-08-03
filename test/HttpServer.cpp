#include<functional>

#include<xse/HttpServer.hpp>
#include<xse/Request.hpp>
#include<xse/Response.hpp>
#include<xse/HttpStatus.hpp>

int main(){
	XSe::HttpServer ser(8080);
	std::function<void(XSe::Request, XSe::Response)> handler = [](XSe::Request req, XSe::Response res){
		res.set(XSe::HTTP1_1, XSe::OK, std::string("hello"));
		res.send();
	};
	ser.setRequestHandler(handler);
	ser.listen();
}