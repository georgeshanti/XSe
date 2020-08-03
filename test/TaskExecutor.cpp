#include<xse/TaskExecutor.hpp>
#include<iostream>
#include <unistd.h>

unsigned int microseconds;

std::function<void()> display(int x){
	return [x](){
		usleep(1000000);
		std::cout<<x<<std::endl;
		usleep(1000000);
	};
}

std::function<void()> create(int x){
	return display(x);
}

int main(){
	XSe::TaskExecutor exe;
	for(int i=0; i<100; i++){
		std::function<void()> d = display(i);
		exe.push(d);
	}
	// usleep(100000);
	while(1);
}