#include<iostream>
#include<fstream>

#include<xse/Response.hpp>

namespace XSe{

	Response::Response(std::string message):connection(0){
		int pos = 0;

		//Save message
		this->message = message;

		//Get version
		if(pos = getVersionFromMessage(message), pos == XSe::INVALID_RESPONSE)
			return;
		message=message.substr(pos);

		if(pos = getStatusFromMessage(message), pos == XSe::INVALID_RESPONSE)
			return;
		message=message.substr(pos);

		//Get headers
		if(pos = getHeadersFromMessage(message), pos == XSe::INVALID_RESPONSE)
			return;
		message=message.substr(pos);
		
		//Get body
		if(pos = getBodyFromMessage(message), pos == XSe::INVALID_RESPONSE)
			return;
	}

	void Response::sendFile(std::ifstream &file, std::string type){
		char buff[1024];
		std::streampos length = file.tellg();
		file.seekg( 0, std::ios::end );
		length = file.tellg() - length;
		this->headers["Content-Length"] = std::to_string(double(length));
		this->headers["Content-Type"] = std::string(type);
		this->sendHeaders();
		file.seekg( 0, std::ios::beg );
		while(file){
			file.read(buff, 1024);
			this->connection.send(buff, file.gcount());
		}
		this->connection.close();
	}

	std::string Response::writeHeaders(){
		std::string str="";

		//Write response line
		str += this->httpVersion.str + " " + std::to_string((int)this->status.code) + " " + this->status.str + "\n";

		//Write headers
		for(std::pair<std::string, std::string> header: this->headers){
			str += header.first + ": " + header.second + "\r\n";
		}

		//Write extra CRLF to separate headers and body
		str += "\r\n";	
		return str;
	}

	std::string Response::writeAsString(){
		//Write headers
		this->setHeader("Content-Length", std::to_string(this->body.length()));
		std::string headersString = this->writeHeaders();

		//Write body
		std::string str = headersString + this->body;
		return str;
	}

	int Response::getVersionFromMessage(std::string message){
		for(HttpVersion version : VERSIONS){
			if(message.find(version.str) == 0 && message[version.str.length()] == ' '){
				this->httpVersion = version;
				return version.str.length()+1;
			}	
		}
		this->httpVersion = INVALID_VERSION;
		this->valid = INVALID_RESPONSE;
		return INVALID_RESPONSE;
	}

	int Response::getStatusFromMessage(std::string message){
		for(HttpStatus status : STATUSES){
			if(message.find(std::to_string(status.code)) == 0 && message[3] == ' '){
				this->status = status;
				return message.find("\n")+1;
			}
		}
		this->status = INVALID_STATUS;
		this->valid = INVALID_RESPONSE;
		return INVALID_RESPONSE;
	}

	int Response::getHeadersFromMessage(std::string message){
		std::smatch m;
		int length = 0;
		while( std::regex_search(message, m, HEADER_REGEX) && m.position()==0 ){
			this->headers.insert( splitHeaderLine( m.str() ) );
			message = m.suffix();
			length += m.length();
		}
		return length;
	}

	int Response::getBodyFromMessage(std::string message){
		if(message.empty())
			return 0;
		else if( message.find("\n")==0 ){
			this->body = message.substr(1, message.length()-2);
			return this->body.length();
		}
		this->valid = INVALID_RESPONSE;
		return INVALID_RESPONSE;
	}

	std::pair<std::string, std::string> Response::splitHeaderLine(std::string line){
		int colonPosition = line.find(":");
		int nextlinePosition = line.find("\n");
		std::string headerKey, headerValue;
		std::pair<std::string, std::string> header;
		if(colonPosition != -1){
			headerKey = line.substr(0, colonPosition);
			headerValue = line.substr(colonPosition+2, nextlinePosition-colonPosition-2);
			header = std::pair<std::string, std::string>(headerKey, headerValue);
		}
		return header;
	}
}