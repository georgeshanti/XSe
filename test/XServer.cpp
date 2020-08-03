#include<xse/XServer.hpp>
#include<fstream>

int main(int argc, char *argv[]){
	// Print the CLI arguments
	for(int i=0;i<argc;i++)
		std::cout<<argv[i]<<std::endl;

	// Initialize the weberver
	XSe::XServer xse(8080);

	// Set up a route to serve static files
	xse.serve("/","resources/static/");

	// Set up a function to serve the / route
	xse.route("/", std::vector<XSe::HttpMethod>{XSe::POST, XSe::GET},[](XSe::Request req, XSe::Response res){
		res.set(XSe::HTTP1_1, XSe::OK, "");
		res.setHeader("Content-Type", "application/json");
		res.setBody(std::string("{\"status\": \"1\", \"message\": \"This is the index page!\"}"));
		res.send();
	});

	// Set up a default 404 page
	xse.setStatusRoute(XSe::NOT_FOUND, [](XSe::Request req, XSe::Response res){
		res.setStatus(XSe::NOT_FOUND);
		res.setHttpVersion(XSe::HTTP1_1);
		std::ifstream file("resources/404.html", std::ios::binary);
		res.sendFile(file, "text/html");
	});

	// Add middleware that will add a custom header 'Server: XSe-XServer
	XSe::Handler handler = xse.getHandler();
	xse.setHandler([handler](XSe::Request req, XSe::Response res){
		res.setHeader("Server", "XSe-XServer");
		handler(req, res);
	});

	// Start the server
	xse.start();
}