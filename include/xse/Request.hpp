#ifndef XSE_REQUEST_H
#define XSE_REQUEST_H

#include<cstdint>
#include<map>
#include<string>
#include<regex>

#include<xse/HttpMethod.hpp>
#include<xse/HttpVersion.hpp>

namespace XSe{

	// Regular Expressions
	const std::string DIR_REGEX="(([-\\w\\.])+)";
	const std::regex URL_REGEX("/(" + DIR_REGEX + "/)*" + DIR_REGEX + "?");

	// INVALID
	const uint8_t INVALID_REQUEST = -1;

	// HTTP Request class
	class Request{

		private:
			HttpMethod method;
			std::string path;
			HttpVersion httpVersion;
			std::string host;
			std::map<std::string,std::string> headers;
			std::map<std::string,std::string> cookies;
			std::string messageBody;
			std::string fullMessage;
			std::string messageHeader;
			int getMethodFromString(std::string);
			int getPathFromString(std::string);
			int getVersionFromString(std::string);
			int getRequestLineFromMessage(std::string);
			int getHeadersFromMessage(std::string);
			int getBodyFromMessage(std::string);
			static std::pair<std::string, std::string> splitHeaderLine(std::string);
			int receivedLength=0;
			int state=0;

		public:
			bool valid;
			std::map<std::string, void*> extras;

			//Body Methods
			void append(std::string str){ this->messageBody += str; }
			void append(uint8_t* buff, int len){ this->messageBody += std::string(buff, buff+len); }

			//Parse/Writing methods
			std::string writeAsString();
			int parse(std::string);
			void parseMessageHeader(std::string);

			//Constructors
			Request():method(INVALID_METHOD), path(), httpVersion(INVALID_VERSION), host(), headers(), cookies(), messageBody(), valid(false), fullMessage(){}
			Request(std::string);

			//Getters
			HttpMethod getMethod()const{ return this->method; }
			std::string getPath()const{ return this->path; }
			HttpVersion getHttpVersion()const{ return this->httpVersion; }
			std::map<std::string, std::string> getHeaders()const{ return this->headers; }
			std::string getBody()const{ return this->messageBody; }
			std::string getHeader(std::string key){ return this->headers[key]; }

			//Setters
			void setMethod(HttpMethod method){ this->method = method; }
			void setPath(std::string path){ this->path = path; }
			void setHttpVersion(HttpVersion httpVersion){ this->httpVersion = httpVersion; }
			void setHeaders(std::map<std::string, std::string> headers){ this->headers = headers; }
			void setBody(std::string body){ this->messageBody = body; }
			void setBody(uint8_t *buff , int len){ this->messageBody = std::string(buff, buff+len); }
			void setHeader(std::string key, std::string value){ this->headers[key]=value; }
			void setExtra(std::string key, void* extra){ this->extras[key]=extra; }
	};
}

#endif