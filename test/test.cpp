#include<xse/XServer.hpp>
#include<fstream>

int main(int argc, char *argv[]){

	XSe::XServer xse(8080);

	xse.route("/", XSe::Methods{XSe::GET, XSe::POST}, [](XSe::Request req, XSe::Response res){
		for(auto header: req.getHeaders()){
			std::cout<<header.first<<": "<<header.second<<std::endl;
		}
		res.setHeader("Content-Type", "application/json");
		res.setBody(std::string("{\"status\": \"1\", \"message\": \"This is the response of GET and POST methods!\"}"));
		res.send();
	});

	xse.route("/", XSe::Methods{XSe::PUT}, [](XSe::Request req, XSe::Response res){
		res.setHeader("Content-Type", "application/json");
		res.setBody(std::string("{\"status\": \"1\", \"message\": \"This is the response of PUT method!\"}"));
		res.send();
	});

	xse.route("/session", XSe::Methods{XSe::GET}, [](XSe::Request req, XSe::Response res){
		res.setHeader("Content-Type", "application/json");
		std::string *session_id = new std::string("");
		auto session_id_ptr = req.extras.find("session_id");
		if(session_id_ptr != req.extras.end()){
			delete session_id;
			session_id = (std::string*)(session_id_ptr->second);
		}
		res.setBody(std::string("{\"status\": \"1\", \"session_id\": \""+(*session_id)+"\"}"));
		res.send();
	});

	// Get the default function with getHandler() and store it into the variable oldHandler
	XSe::Handler oldHandler = xse.getHandler();

	// Define a new function newHandler
	XSe::Handler newHandler = [oldHandler](XSe::Request req, XSe::Response res){
		res.setHeader("Server", "Xse::XServer");
		oldHandler(req, res);
	};
	
	// Set the default function to newHandler
	xse.setHandler(newHandler);

	oldHandler = xse.getHandler();

	// Define a new function newHandler
	newHandler = [oldHandler](XSe::Request req, XSe::Response res){
		std::string *session_id = new std::string("12345");
		req.setExtra("session_id", (void*)(session_id));
		oldHandler(req, res);
		delete session_id;
	};
	
	// Set the default function to newHandler
	xse.setHandler(newHandler);

	// Start the server
	xse.start();
}