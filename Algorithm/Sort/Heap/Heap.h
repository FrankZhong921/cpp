#ifndef HEAP
#define HEAP

template<typename> class Sort_base;
template<typename T>
class Heap : public Sort_base<T>{
	public:
		Heap(T a,T b,int s):Sort_base<T>(a,b,s),sz(s){
			this->arr.insert(this->arr.begin(),0);	
			
		};
		void sort() 	{
			//现已定义了arr和size;
			
			makeHeap();
			std::cout << "Make Heap :";
			this->print(std::cout);
			std::cout <<std::endl;
			//arr索引从1到sz
			while(sz > 0){
				swap(1,sz--); //sz-- 很重要，决定了最后的元素不再移动，因为最后的元素是最大的
				sink(1);
			}
		}




	private: 
		int sz;//sz指的是堆的大小，不包括第一个元素
		//以下为工具函数
		void swim(int );
		void sink(int );
		void makeHeap();
		inline void swap(int a,int b){
			typename T::value_type t = this->arr[a];
			this->arr[a] = this->arr[b];
			this->arr[b] = t;
		}

};

template<typename T>
void Heap<T>::sink(int i){
	while( 2*i <= sz){
		int k = 2*i;
		
		if(k < sz && this->arr[k] < this->arr[k+1]){
			++k;		
		}
		if(this->arr[i] >= this->arr[k]) break;
		swap(i,k);
		i = k;
	}
}

template<typename T>
void Heap<T>::swim(int i){
	while(i>1){
		if(this->arr[i/2] < this->arr[i]){
			swap(i/2,i);
			i/= 2;
		}
		else break;
	}
}

template<typename T>
void Heap<T>::makeHeap(){
	
	
	for(int k = sz /2; k>0 ; --k){
		sink(k);
	}

}






#endif
