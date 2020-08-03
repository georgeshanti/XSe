#include<fstream>
#include<iostream>

#include<xse/Request.hpp>

int main(){
	std::fstream get_request("resources/get_request.txt");

	std::string str="", tmp;

	while( getline(get_request, tmp) )
		str += tmp + "\n";

	XSe::Request req(str);

	std::cout<<req.writeAsString()<<std::endl;
	return 0;
}