#ifndef QUICK
#define	QUICK

template <typename > class Sort_base;

template <typename T>
class Quick
: public Sort_base<T>{
	public:
		typedef typename T::value_type value_type;
		Quick(T a,T b, int s): Sort_base<T>(a,b,s){};
		void sort(){
			int i = 0,j = this->size-1;
			quickSort(i,j);
		}

	
	private:
		int partition(int,int);
		void quickSort(int, int);
		void quick3Way(int,int );
		inline int compare(int a,int b) {
			if(this->arr[a] > this->arr[b]) return 1;
			else if return -1;
			else return 0;
		}
		inline void exc(int a, int b){
			typename T::value_type tmp = this->arr[a];
			this->arr[a] = this->arr[b];
			this->arr[b] = tmp;
		}
}; 

template <typename T>
int Quick<T>::partition(int lo, int hi){
	int i =lo , j = hi+1;
	typename T::value_type p = this->arr[lo];
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

template <typename T>
void Quick<T>::quick3Way(int lo , int hi){
	if(lo>= hi) return;
	int lt = lo , i = lo +1, gt = hi; //使用三个指针维护，其中lt指向重复元素的第一个，hi指向重复元素的最后一个
	int v = this->arr[lo];
	while(i <= gt){
		int cmp = compare(i,v);
		if(cmp < 0){
			exc(lt++,i++);
		}
		else if(cmp >0){
			exc(i,gt--);
		}
		else{
			i++;		
		}
	}
	quick3Way(lo, lt-1);
	quick3Way(gt+1. hi);
	
}
#endif
