#ifndef SHELL
#define SHELL

template <typename > class Sort_base;

template <typename T>
class Shell : public Sort_base<T>{
	
	public:
		Shell(T a,T b,int s):Sort_base<T>(a,b,s){};
		void sort() override{
			int h = 1;
			while(h < this->size / 3) h = 3*h +1;
			while(h >= 1){
				for(int i = h; i < this->size ; i+=h){
					for(int j =i; j>=0 && this->arr[j] < this->arr[j-h];j-=h){
						swap(j,j-h);
					}
				}
				h/=3;
			}

		}
	private:
		 void swap(int i,int j){
			auto tmp = this->arr[i];
			this->arr[i] = this->arr[j];
			this->arr[j] = tmp;

		}
};



#endif
