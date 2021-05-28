#include <iostream>
#include <string>
#include <sstream>

using namespace std;
int main(int argc,char* argv[]){
	//函数接受两个参数，第一个参数表数量，第二个参数表是否等键值(y/n)，第三个参数表输出位置;
	if(argc <4){
		cout<< "need args num y/n dst" << endl;
	}
	int num = atoi(argv[1]);
	bool yn = (*argv[2] == 'y'? 1:0);
	string dst = string(argv[3]);
	cout<< "num is " << num << endl
		<< "yn is "<< yn << endl
		<< "dst is "<< dst << endl;
	
	
	


}
