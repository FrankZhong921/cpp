#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>


template <typename T>
void move(std::vector<T>& vec,typename std::vector<T>::iterator& next,typename std::vector<T>::iterator& min){
	
	if(next == vec.end()){
		std::cout << "min: " << *min <<std::endl;		
		return;
	}
	if(*next < *min){
		min = next;
	}
	move(vec,++next,min);
	
	



}
template <typename T>
void Selection(std::vector<T>& vec,typename std::vector<T>::iterator& first){
	//实现选择排序的递归版本
	if(first >= vec.end()) return;
	//std::cout << *first << " ";
	typename std::vector<T>::iterator next = first+1,min = first;
	
	move(vec,next,min);//找到最小值
	std::swap(*first,*min); //交换
	Selection(vec,++first);

}
int main(){
	std::ifstream input("../words.txt");
	std::istream_iterator<std::string> iter(input),end;
	std::vector<std::string> vec(iter,end);
	auto begin = vec.begin();
	for(const auto a:vec){
		std::cout << a <<std::endl;
	}
	std::cout << std::endl;
	Selection(vec,begin);
	for(const auto a:vec){
		std::cout << a <<std::endl;
	}

}
