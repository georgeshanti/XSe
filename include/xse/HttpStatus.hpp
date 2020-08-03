#ifndef XSE_HTTP_STATUS_H
#define XSE_HTTP_STATUS_H

#include<vector>
#include<string>
#include<cstdint>

namespace XSe{

	// HTTP Status
	class HttpStatus{
		public:
		int code;
		std::string str;

		HttpStatus(int c, std::string s):code(c),str(s){};
		HttpStatus():code(-1),str("INVALID"){};

		bool operator==(HttpStatus& __x) const { return this->code==__x.code; };
		bool operator<(const HttpStatus& __x) const { return this->code<__x.code; };
	};

	//Http method constants
	const HttpStatus INVALID_STATUS(-1,"INVALID");

	const HttpStatus CONTINUE(100, "Continue");
	const HttpStatus SWITCHING_PROTOCOLS(101, "Switching Protocols");

	const HttpStatus OK(200, "OK");
	const HttpStatus CREATED(201, "Created");
	const HttpStatus ACCEPTED(202, "Accepted");
	const HttpStatus NON_AUTHORITATIVE(203, "Non-authoritative");
	const HttpStatus NO_CONTENT(204, "No Content");
	const HttpStatus RESET_CONTENT(205, "Reset Content");
	const HttpStatus PARTIAL_CONTENT(206, "Partial Content");

	const HttpStatus MULTIPLE_CHOICES(300, "Multiple Choices");
	const HttpStatus MOVED_PERMANENTLY(301, "Moved Permanently");
	const HttpStatus FOUND(302, "Found");
	const HttpStatus SEE_OTHER(303, "See Other");
	const HttpStatus NOT_MODIFIED(304, "Not Modified");
	const HttpStatus USE_PROXY(305, "Use Proxy");
	const HttpStatus UNUSED(306, "Unused");
	const HttpStatus TEMPORARY_REDIRECT(307, "Temporary Redirect");

	const HttpStatus BAD_REQUEST(400, "Bad Request");
	const HttpStatus UNAUTHORIZED(401, "Unauthorized");
	const HttpStatus PAYMENT_REQUIRED(402, "Payment Required");
	const HttpStatus FORBIDDEN(403, "Forbidden");
	const HttpStatus NOT_FOUND(404, "Not Found");
	const HttpStatus METHOD_NOT_ALLOWED(405, "Method Not Allowed");
	const HttpStatus NOT_ACCEPTABLE(406, "Not Acceptable");
	const HttpStatus PROXY_AUTHENTICATION_REQUIRED(407, "Proxy Authentication Required");
	const HttpStatus REQUEST_TIMEOUT(408, "Request Timeout");
	const HttpStatus CONFLICT(409, "Conflict");
	const HttpStatus GONE(410, "Gone");
	const HttpStatus LENGTH_REQUIRED(411, "Length Required");
	const HttpStatus PRECONDITION_FAILED(412, "Precondition Failed");
	const HttpStatus REQUEST_ENTITY_TOO_LARGE(413, "Request Entity Too Large");
	const HttpStatus REQUEST_URL_TOO_LONG(414, "Request-url Too Long");
	const HttpStatus UNSUPPORTED_MEDIA_TYPE(415, "Unsupported Media Type");
	const HttpStatus REQUESTED_RANGE_NOT_SATISFIABLE(416, "Requested Range Not Satisfiable");
	const HttpStatus EXPECTATION_FAILED(417, "Expectation Failed");

	const HttpStatus INTERNAL_SERVER_ERROR(500, "Internal Server Error");
	const HttpStatus NOT_IMPLEMENTED(501, "Not Implemented");
	const HttpStatus BAD_GATEWAY(502, "Bad Gateway");
	const HttpStatus SERVICE_UNAVAILABLE(503, "Service Unavailable");
	const HttpStatus GATEWAY_TIMEOUT(504, "Gateway Timeout");
	const HttpStatus HTTP_VERSION_NOT_SUPPORTED(505, "HTTP Version Not Supported");

	const std::vector<HttpStatus> STATUSES{
		CONTINUE, SWITCHING_PROTOCOLS, OK, CREATED, ACCEPTED, NON_AUTHORITATIVE,NO_CONTENT, RESET_CONTENT,
		PARTIAL_CONTENT, MULTIPLE_CHOICES, MOVED_PERMANENTLY, FOUND, SEE_OTHER, NOT_MODIFIED,USE_PROXY,
		UNUSED, TEMPORARY_REDIRECT, BAD_REQUEST, UNAUTHORIZED, PAYMENT_REQUIRED, FORBIDDEN, NOT_FOUND,
		METHOD_NOT_ALLOWED, NOT_ACCEPTABLE, PROXY_AUTHENTICATION_REQUIRED, REQUEST_TIMEOUT, CONFLICT, GONE,
		LENGTH_REQUIRED, PRECONDITION_FAILED, REQUEST_ENTITY_TOO_LARGE, REQUEST_URL_TOO_LONG, UNSUPPORTED_MEDIA_TYPE,
		REQUESTED_RANGE_NOT_SATISFIABLE, EXPECTATION_FAILED, INTERNAL_SERVER_ERROR, NOT_IMPLEMENTED, BAD_GATEWAY,
		SERVICE_UNAVAILABLE, GATEWAY_TIMEOUT, HTTP_VERSION_NOT_SUPPORTED
	};
}

namespace std
{
	template<> struct  less<XSe::HttpStatus>
	{
		bool operator() (const XSe::HttpStatus& lhs, const XSe::HttpStatus& rhs) const
		{
			return lhs.code < rhs.code;
		}
	};

}

#endif