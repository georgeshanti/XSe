#ifndef XSE_HTTP_METHOD_H
#define XSE_HTTP_METHOD_H

#include<cstdint>
#include<string>
#include<vector>

namespace XSe{

	// HTTP Method
	class HttpMethod{
		public:
		uint8_t code;
		std::string str;

		HttpMethod(uint8_t c, std::string s):code(c),str(s){};
		HttpMethod():code(-1),str("INVALID"){};

		bool operator==(HttpMethod& __x) const { return this->code==__x.code; };
		bool operator<(const HttpMethod& __x) const { return this->code<__x.code; };
	};

	//Http method constants
	const HttpMethod INVALID_METHOD(-1,"INVALID");
	const HttpMethod GET(0,"GET");
	const HttpMethod HEAD(1,"HEAD");
	const HttpMethod POST(2,"POST");
	const HttpMethod PUT(3,"PUT");
	const HttpMethod DELETE(4,"DELETE");
	const HttpMethod CONNECT(5,"CONNECT");
	const HttpMethod OPTIONS(6,"OPTIONS");
	const HttpMethod TRACE(7,"TRACE");
	const HttpMethod PATCH(8,"PATCH");

	const std::vector<HttpMethod> METHODS{GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, PATCH};

	typedef std::vector<XSe::HttpMethod> Methods;
}

#endif