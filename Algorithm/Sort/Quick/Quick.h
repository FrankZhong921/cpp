#ifndef QUICK
#define	QUICK

template <typename > class Sort_base;

template <typename T>
class Quick
: public Sort_base<T>{
	public:
		Quick(T a,T b, int s): Sort_base<T>(a,b,s){};
		void sort(){
			int i = 0,j = this->size-1;
			quickSort(i,j);
		}

	
	private:
		int partition(int,int);
		void quickSort(int, int);

};

template <typename T>
int Quick<T>::partition(int lo, int hi){
	int i =lo , j = hi+1;
	int p = this->arr[lo];
	while(true){
		while(this->arr[++i] < p) if(i == hi) break;
		while(this->arr[--j] > p) if(j == lo) break;
		if(i >= j) break;
		int arr_j = this->arr[j];
		this->arr[j] = this->arr[i];
		this->arr[i] = arr_j;
		std::cout << this->arr[j]<<" swap " << this->arr[i] <<std::endl;
	}
	this->arr[lo] = this->arr[j];
	this->arr[j] = p;
	std::cout << this->arr[lo] <<std::endl;
	return j;
}

template <typename T>
void Quick<T>::quickSort(int lo,int hi){
	if(lo >= hi) return;
	int p = partition(lo, hi);
	quickSort(lo,p-1);
	quickSort(p+1, hi);


}
#endif
