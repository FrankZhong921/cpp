#include "List.h"
#include <iostream>

int main(){
	List<int> list;
	for(int i=0;i<10;++i){
		list.addNode(i,i+1);
		std::cout << list.getLength() <<std::endl;
	}
	std::cout << "ADD Finish!!" << std::endl;
	auto iter = list.begin();
	for(int i=1;i<5;i++){
		std::cout << *iter <<std::endl;
		++iter;
	}
} 
