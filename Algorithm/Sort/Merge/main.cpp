#include "Sort_base.h"
#include "Merge.h"
#include <iostream>
#include <vector>
int main(){
	std::vector<int> arr= {9,1,100,4,50,1,45};

	/*	std::vector<int>::iterator iter = arr.begin();
		std::vector<int>::iterator::value_type a =*begin;
		std::cout << a << std::endl;
	*/
	Merge<std::vector<int>::iterator> s(arr.begin(),arr.end(),arr.size());
	
	s.print(std::cout);
	std::cout << std::endl;
	s.sort();
	s.print(std::cout);
	std::cout << std::endl;
}
