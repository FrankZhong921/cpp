#include <iostream>
#include "RedBlackBST.h"
#include <string>

int main(){

	RedBlackBST<int,std::string> rbt;
	for(int i=0;i<10;i++){
		rbt.put(i,"key is a");
	}
	std::cout << "rbt.size : "<<rbt.size() << std::endl;
	rbt.deleteMin();
	std::cout << "Deletemin "<< std::endl;
	std::cout << "rbt.size : "<<rbt.size() << std::endl;
	auto p = rbt.root;
	rbt.Midsearch(p);

	return 0;

}
