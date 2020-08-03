#include<xse/XServer.hpp>
#include<fstream>

namespace XSe{
	XServer::XServer(int _port):port(_port), httpServer(_port){
		this->handler = 
			[this](XSe::Request req, XSe::Response res){
				std::string path = req.getPath();

				bool found = false;

				// Search static folder routes
				for(auto staticRoute: this->staticRoutes){
					if(path.find(staticRoute.first)==0){
						std::string subpath = path.substr(staticRoute.first.length());
						std::string sourcePath = staticRoute.second + subpath;
						std::ifstream file(sourcePath, std::ios::binary);
						if(file.good()){
							res.set(XSe::HTTP1_1, XSe::OK, "");
							std::string contentType = "appication/unknown";
							for(std::pair<std::string, std::string> it: XSe::FILETYPES){
								int extensionPos = sourcePath.rfind(it.first.c_str());
								if( (extensionPos + it.first.length()) == sourcePath.length() ){
									contentType = it.second;
									break;
								}
							}
							res.sendFile(file, contentType);
							return;
						}
					}
				}

				// Search API routes
				auto routeHandler = this->routes.find(path);
				if(routeHandler!=this->routes.end()){
					auto methodHandlers = routeHandler->second; 
					auto methodHandler = methodHandlers.find(req.getMethod());
					if(methodHandler!=methodHandlers.end()){
						(methodHandler->second)(req, res);
						return;
					}
					res.set(XSe::HTTP1_1, XSe::METHOD_NOT_ALLOWED, "");
					res.send();
				}

				// If not found then default to 404 route
				auto _404Function = this->statusRoutes.find(XSe::NOT_FOUND);
				if(_404Function!=this->statusRoutes.end()){
					(_404Function->second)(req, res);
				}
				else{
					res.set(XSe::HTTP1_1, XSe::NOT_FOUND, "");
					res.send();
				}
			};
	}

	void XServer::start(){
		this->httpServer.setRequestHandler(this->handler);
		this->httpServer.listen();
	}
}