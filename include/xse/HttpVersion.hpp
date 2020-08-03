#ifndef XSE_HTTP_VERSION_H
#define XSE_HTTP_VERSION_H

#include<cstdint>
#include<map>
#include<string>
#include<regex>

namespace XSe{

	// HTTP Versions
	class HttpVersion{
		public:
		uint8_t code;
		std::string str;

		HttpVersion(uint8_t c, std::string s):code(c),str(s){};
		HttpVersion():code(-1),str("INVALID"){};

		bool operator==(HttpVersion& version) const { return this->code==version.code; };
	};

	//Http version constants
	const HttpVersion INVALID_VERSION(-1, "INVALID");
	const HttpVersion HTTP1(0,"HTTP/1");
	const HttpVersion HTTP1_1(1,"HTTP/1.1");
	const HttpVersion HTTP2(2,"HTTP/2");

	const std::vector<HttpVersion> VERSIONS{HTTP1, HTTP1_1, HTTP2};
}

#endif