#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>
typedef  std::vector<std::string>::iterator iterator;

int cmp(std::string a, std::string b){
	if(a < b) return -1;
	if(a > b) return 1;
	return 0;

}
void print(iterator &a,std::string b){
	std::cout << b << ": " <<*a << std::endl;
}
void swap(iterator a,iterator b){
	std::string tmp = *a;
	*a = *b;
	*b = tmp;


}
void UniqueSort(iterator first, iterator last,std::vector<std::string>& vec){

	if(first >= last) return ;
	iterator lt= first, i = first+1,gt = last;
	std::string v = *first;
	int a;
	std::cout << "Start while: " <<std::endl;
	
	while( i <= gt){
		/*
		int cmp = [&]() int {
						if(*i < v) return -1;
						if(*i > v) return 1;
						return 0;
						};
		*/
		std::cout << std::endl;
		print(lt,"lt");
		print(i,"i");
		print(gt,"gt");
		a = cmp(*i,v);
		std::cout << a <<std::endl;	
		if(a < 0) {
			std::cout << "cmp<0:swap(" <<*lt <<","<<*i <<")" <<std::endl;
			swap(lt++,i++);
		}
		else if(a > 0){
			std::cout << "cmp>0:swap(" <<*i <<","<<*gt <<")" <<std::endl;
			swap(i,gt--);
		}
		else {
			i++;
		}
		for(const auto s :vec){
			std::cout << s <<" ";
		}
		std::cout << std::endl;
	}
	std::cout << "Finish while " <<std::endl;
	UniqueSort(first,lt-1,vec);

	UniqueSort(gt+1,last,vec);

	std::for_each(lt+1,gt+1,[](std::string& t){t ="";});

}

int main(){
	std::ifstream input("../words.txt");
	std::istream_iterator<std::string> iter(input),eof;
	std::vector<std::string> vec(iter,eof);
	//使用三向的快速排序，但是不把相同的元素放入
	for(const auto a:vec){
		std::cout << a <<std::endl;
	}
	std::cout << std::endl;
	UniqueSort(vec.begin(),--vec.end(),vec);
	for(const auto a:vec){
		std::cout << a <<std::endl;
	}
}






