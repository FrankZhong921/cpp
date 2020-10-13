#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include  <string>
#include <iterator>
//从文件中获取单词并存入vector,
//暂时用暴力解法，时间复杂度立方级

int main(){
	
	std::ifstream input("./words.txt");
	std::istream_iterator<std::string> iter(input),end;
	std::string line,word;
	std::vector<std::string>  vec(iter,end),result;
	std::cout << vec.size() <<std::endl;
	/*
	while(std::getline(input,line)){
		std::istringstream s(line);
		while(s>>word){
			vec.push_back(word);
		}
	}
	*/
	//立方级暴力解法
	for(int i =0;i<vec.size();++i){
		for(int j = 0;j<vec.size();++j){
			for(int k = 0;k<vec.size();++k){
				if(vec[i] == vec[j]+vec[k])
					result.push_back(vec[i]);
			}
		}
	}
	//先排序
	NlogN
	for(int i =0;i<vec.size();++i){
		for(int j = i+1;j<vec.size();++j){
			for(int k = j+1;k<vec.size();++k){
				if(vec[i] == vec[j]+vec[k])
					result.push_back(vec[i]);
			}
		}
	}
	std::cout << result.size() <<std::endl;
}
