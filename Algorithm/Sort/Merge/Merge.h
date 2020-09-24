#ifndef MERGE
#define MERGE

#include <iostream>
template <typename > class Sort_base;
template <typename T>
class Merge : public Sort_base<T> {
	public:
		Merge(T a,T b,int s):Sort_base<T>(a,b,s),arrp(a,b){};
		void sort() override{
			fromUptoDown_sort(0,this->size -1);
		}
	private:
		std::vector<typename T::value_type> arrp;
		void merge(int,int,int );
		void fromUptoDown_sort(int,int);

};

template <typename T>
void Merge<T>::merge(int lo, int mid,int hi){
	std::cout << "merge" << lo << ","<< mid << "," << hi <<std::endl;
	int i = lo, j = mid+1;
	for(int i = lo; i <= hi ;++i){
		arrp[i] = this->arr[i];
	}  //并不是为了初始化数组，而是保存递归到最后两个小数组的分别有序
		//因为下面那个循环始终使用arrp作为数据源进行赋值！！
	for(int k=lo ;k<=hi;k++){ //遍历新数组的每一个元素
		if(i > mid) this->arr[k] = arrp[j++];	
		else if(j > hi)  this->arr[k] = arrp[i++];
		else if(arrp[i] < arrp[j]) this->arr[k] = arrp[i++];
		else this->arr[k] = arrp[j++];
		std:: cout << "arr["<<k<<"] = " << this->arr[k] << std::endl;
		
	}
}

template <typename T>
void Merge<T>::fromUptoDown_sort(int lo, int hi){
	if(lo >= hi) return;
	int mid  = lo + (hi - lo)/2;
	std::cout << "mid = " << mid <<std::endl;
	fromUptoDown_sort(lo,mid);  //左半边排序
	std::cout << "merge_sort(" << lo << ","<< mid <<") " <<std::endl;
	fromUptoDown_sort(mid+1,hi);	//右半边排序
	std::cout << "merge_sort(" << mid+1 << ","<< hi <<") " <<std::endl;
	merge(lo,mid,hi);
	
}
	

#endif
