#ifndef XSE_NETWORK_EXCEPTION
#define XSE_NETWORK_EXCEPTION

#include<exception>
#include<string>

namespace XSe{
	class NetworkException: public std::exception{
		std::string _message;
		public:
			NetworkException(std::string message):_message(message){}
	};
}

#endif