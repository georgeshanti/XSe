#ifndef XSE_RESPONSE_H
#define XSE_RESPONSE_H

#include<cstdint>
#include<map>
#include<string>
#include<regex>

#include<xse/HttpVersion.hpp>
#include<xse/HttpStatus.hpp>
#include<xse/Connection.hpp>

namespace XSe{

	// HTTP Response class
	class Response{
		Connection connection;
		HttpVersion httpVersion;
		HttpStatus status;
		std::map<std::string, std::string> headers;
		std::string body;
		std::string message;

		int getVersionFromMessage(std::string);
		int getStatusFromMessage(std::string);
		int getHeadersFromMessage(std::string);
		int getBodyFromMessage(std::string);
		std::pair<std::string, std::string> splitHeaderLine(std::string);

		public:
			bool valid;

			//Body Methods
			void append(std::string str){ this->body += str; }
			void append(uint8_t* buff, int len){ this->body += std::string(buff, buff+len); }
			void sendFile(std::ifstream&, std::string);
			void sendHeaders(){
				std::string headersString = this->writeHeaders();
				this->connection.send(headersString.c_str(), headersString.length());
			}

			//Parsing/Writing methods
			std::string writeHeaders();
			std::string writeAsString();
			void send(){
				std::string mes = this->writeAsString();
				this->connection.send(mes.c_str(), mes.length());
				this->connection.close();
			}

			//Constructors
			Response(Connection conn):connection(conn),httpVersion(XSe::HTTP1_1),status(XSe::OK){}
			Response(std::string);
			void set(HttpVersion _version, HttpStatus _status, std::string _body){
				this->httpVersion = _version;
				this->status = _status;
				this->body = _body;
				this->setHeader("Content-Length", std::to_string(this->body.length()));
			}

			//Getters
			HttpVersion getHttpVersion()const{ return this->httpVersion; }
			HttpStatus getStatus()const{ return this->status; }
			std::map<std::string, std::string> getHeaders()const{ return this->headers; }
			std::string getBody()const{ return this->body; }
			std::string getHeader(std::string key){ return this->headers[key]; }

			//Setters
			void setHttpVersion(HttpVersion httpVersion){ this->httpVersion = httpVersion; }
			void setStatus(HttpStatus status){ this->status = status; }
			void setHeaders(std::map<std::string, std::string> headers){ this->headers = headers; }
			void setBody(std::string body){
				this->body = body;
				this->setHeader("Content-Length", std::to_string(this->body.length()));
			}
			void setBody(uint8_t *buff , int len){
				this->body = std::string(buff, buff+len);
				this->setHeader("Content-Length", std::to_string(this->body.length()));
			}
			void setHeader(std::string key, std::string value){ this->headers[key] = value; }

	};
	const std::regex HEADER_REGEX("^[-\\w]+: [-\\w]+\n");

	// INVALID
	const uint8_t INVALID_RESPONSE = -1;
}

#endif