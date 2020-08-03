#include<fstream>
#include<iostream>

#include<xse/Response.hpp>

int main(){
	std::fstream get_request("resources/get_response.txt");

	std::string str="", tmp;

	while( getline(get_request, tmp) )
		str += tmp + "\n";

	XSe::Response res(str);
	char sti[]="Hello";
	res.append((uint8_t*)sti, 5);
	std::cout<<res.writeAsString()<<std::endl;
	return 0;
}