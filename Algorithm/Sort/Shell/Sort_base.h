#ifndef SORT_BASE
#define SORT_BASE
#include <algorithm>
#include <vector>
#include <iostream>
template <typename T>
class Sort_base{
	//排序算法构造函数接受一对迭代器和大小，拷贝赋值给array/vector容器
	public:
		Sort_base(T a,T b, int s):arr(a,b),size(s){};
		virtual void sort(){};  //若无{},则使用时出现undefine reference to `Sort_base<__gnu_cxx::__normal_iterator<int*, std::vector<int, std::allocator<int> > > >::sort()'
		bool isSorted();  //调用sort()后使用assert isSorted()进行判断
		void print(std::ostream&);
	protected:
		std::vector<typename T::value_type> arr;
		int size;
		
};

template <typename T>
bool Sort_base<T>::isSorted(){
	for(const auto a : arr){
		for(const auto b : arr)
			if(a > b) return false;
	}
	return true;
}


template <typename T>
void Sort_base<T>::print(std::ostream& out){
	std::for_each(arr.cbegin(),arr.cend(),[&](const int a){ out << a <<", ";});
}
#endif
