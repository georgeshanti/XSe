#include<iostream>

#include<xse/Request.hpp>

namespace XSe{

	Request::Request(std::string message){
		int pos = 0;

		//Save message
		this->fullMessage = message;

		// //Get method
		// if(pos = getMethodFromMessage(message), pos == XSe::INVALID_REQUEST)
		// 	return;
		// message=message.substr(pos);

		// //Get path
		// if(pos = getPathFromMessage(message), pos == XSe::INVALID_REQUEST)
		// 	return;
		// message=message.substr(pos);

		// //Get version
		// if(pos = getVersionFromMessage(message), pos == XSe::INVALID_REQUEST)
		// 	return;
		// message=message.substr(pos);

		// //Get headers
		// if(pos = getHeadersFromMessage(message), pos == XSe::INVALID_REQUEST)
		// 	return;
		// message=message.substr(pos);
		
		// //Get body
		// if(pos = getBodyFromMessage(message), pos == XSe::INVALID_REQUEST)
		// 	return;
	}

	std::string Request::writeAsString(){
		std::string str = "";

		//Write request line
		str += this->method.str + " " + this->path + " " + this->httpVersion.str + "\r\n";

		//Write headers
		for(std::pair<std::string, std::string> header: this->headers){
			str += header.first + ": " + header.second + "\r\n";
		}

		//Write extra CRLF to separate headers and body
		str += "\r\n";

		//Write body
		str += this->messageBody;
		return str;
	}

	int Request::getMethodFromString(std::string message){
		for(HttpMethod method : METHODS){
			if(message==method.str){
				this->method = method;
				return method.str.length()+1;
			}	
		}
		this->method = INVALID_METHOD;
		this->valid = INVALID_REQUEST;
		return INVALID_REQUEST;
	}

	int Request::getPathFromString(std::string message){
		std::smatch m;
		if( std::regex_search(message, m, URL_REGEX) && m.position()==0 ){
			this->path = m.str();
			return m.length()+1;
		}
		return INVALID_REQUEST;
	}

	int Request::getVersionFromString(std::string message){
		for(HttpVersion version : VERSIONS){
			if(message==version.str){
				this->httpVersion = version;
				return version.str.length()+1;
			}	
		}
		this->httpVersion = INVALID_VERSION;
		this->valid = INVALID_REQUEST;
		return INVALID_REQUEST;
	}

	int Request::getRequestLineFromMessage(std::string requestLine){

		//Get method
		int spacePos = requestLine.find(" "), pos;
		std::string method = requestLine.substr(0,spacePos);
		if(pos = getMethodFromString(method), pos == XSe::INVALID_REQUEST)
			return 0;

		//Get path
		requestLine=requestLine.substr(spacePos+1);
		spacePos = requestLine.find(" ");
		std::string path = requestLine.substr(0,spacePos);
		if(pos = getPathFromString(path), pos == XSe::INVALID_REQUEST)
			return 0;

		//Get version
		requestLine=requestLine.substr(spacePos+1);
		spacePos = requestLine.find("\r\n");
		std::string version = requestLine.substr(0,spacePos);
		if(pos = getVersionFromString(version), pos == XSe::INVALID_REQUEST)
			return 0;

		return 1;
	}

	int Request::getHeadersFromMessage(std::string message){
		int pos;
		int length = 0;
		while( pos = message.find("\r\n"), pos>=0 ){
			std::string header = message.substr(0,pos);
			int colonPosition = header.find(":");
			if( colonPosition >= 0 ){
				std::pair<std::string, std::string> pair = splitHeaderLine( header );
				this->headers.insert( pair );
				message = message.substr(pos+2);
				length += header.length();
			}
		}
		return length;
	}

	std::pair<std::string, std::string> Request::splitHeaderLine(std::string line){
		int colonPosition = line.find(":");
		int nextlinePosition = line.find("\r\n");
		std::string headerKey, headerValue;
		std::pair<std::string, std::string> header;
		if(colonPosition != -1){
			headerKey = line.substr(0, colonPosition);
			headerValue = line.substr(colonPosition+2, nextlinePosition-colonPosition-2);
			header = std::pair<std::string, std::string>(headerKey, headerValue);
		}
		return header;
	}

	int Request::getBodyFromMessage(std::string message){
		if(message.empty())
			return 0;
		else if( message.find("\n")==0 ){
			this->messageBody = message.substr(1, message.length()-2);
			return this->messageBody.length();
		}
		this->valid = INVALID_REQUEST;
		return INVALID_REQUEST;
	}

	void Request::parseMessageHeader(std::string messageHeader){
		int pos = 0;
		if(pos=messageHeader.find("\r\n"), pos>0){
			std::string requestLine = messageHeader.substr(0,pos);
			getRequestLineFromMessage(requestLine);
		}
		else{
			this->valid=false;
		}

		//Get headers
		std::string headersString = messageHeader.substr(pos+2);
		if(pos = getHeadersFromMessage(headersString), pos == XSe::INVALID_REQUEST)
			return;
	}

	int Request::parse(std::string buffer){
		this->fullMessage += buffer;
		int contentLength=0;
		switch(state){
			case 0:{
				int pos = fullMessage.find("\r\n\r\n");
				if(pos>0){
					messageHeader = fullMessage.substr(0,pos);
					this->parseMessageHeader(messageHeader);
					messageBody = fullMessage.substr(pos+4);
					state = 1;
					std::stringstream(this->getHeader("Content-Length"))>>contentLength;
					if(messageBody.length()>=contentLength){
						this->setBody(messageBody);
						state = 2;
						this->valid=true;
						return 1;
					}
				}
				return 0;
			}
			case 1:{
				messageBody += std::string(buffer);
				if(messageBody.length()>=contentLength){
					this->setBody(messageBody);
					state = 2;
					this->valid=true;
					return 1;
				}
				break;
			}
			case 2:
			default:{
				break;
			}
		}
		return 1;
	}
}