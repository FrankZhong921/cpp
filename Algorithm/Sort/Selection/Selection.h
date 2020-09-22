#ifndef SELECTION
#define SELECTION

#include "Sort_base.h"

/*
	模板类作为基类！！！
*/

template <typename> class Sort_base;  //声明
template <typename T>
class Selection : public Sort_base<T> {
	public:
	Selection(T a,T b, int s):Sort_base<T>(a,b,s){};  //基本构造函数必须提供模板
	

	void sort() override{
		for(int i=0;i < this->size;++i){
			int min = i;
			for(int j =i+1;j<this->size;++j){
				if(this->arr[j]<this->arr[min]){
					min = j;
				}
			}
			auto tmp = this->arr[min];

			this->arr[min] = this->arr[i];
			this->arr[i] = tmp;
		}
	}

};


#endif
