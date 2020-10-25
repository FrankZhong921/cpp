#include <iostream>
#include "RedBlackBST.h"
#include <string>

int main(){

	RedBlackBST<int,std::string> rbt;
	for(int i=0;i<4;i++){			//		1
		rbt.put(i,"key is a");		//	   0        2\3
	}
	
	rbt.deleteNode(3);
	std::cout << rbt.size() <<std::endl;	
	rbt.Midsearch(rbt.root);
	

	return 0;

}
