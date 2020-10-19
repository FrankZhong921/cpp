#include <iostream>
#include "RedBlackBST.h"
#include <string>

int main(){

	RedBlackBST<std::string,std::string> rbt;
	for(int i =0;++i;i<10){
		rbt.put("aa","key is aa");
	}
	std::cout << rbt.size() <<std::endl;
}
