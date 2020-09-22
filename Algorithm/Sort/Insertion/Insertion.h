#ifndef INSERTION
#define INSERTION
#include "Sort_base.h"

template <typename> class Sort_base;

template <typename T>
class Insertion : public Sort_base<T>{

	public:
		Insertion(T a,T b,int s):Sort_base<T>(a,b,s){};
		void sort() override{
			for(int i = 1;i < this->size;++i){
				for(int j = i ; j>= 0 ;--j){
					if(this->arr[j] < this->arr[j-1]){ //往前逐对交换，
						swap(j,j-1);
					}
				}
			
			}
		}
	private:
		void swap(int i,int j){
			auto tmp  = this->arr[i];
			this->arr[i] = this->arr[j];
			this->arr[j] = tmp;
		}
};
					




#endif
