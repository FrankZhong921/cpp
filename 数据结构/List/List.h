#ifndef LIST
#define LIST

#include <iostream>
#include <stdexcept>

template <typename T> class List;
template <typename T> class List_iterator;

// Class Node Defination
template <typename T>
class Node{
	friend class List<T>;
	friend class List_iterator<T>;
	Node(T v,Node<T>* n = 0): value(v),next(n){};
	T value;
	Node<T>* next;	
};

// Class List_iterator Defination
template <typename T>
class List_iterator{
	friend class List<T>;
	public:
		List_iterator<T>& getIter(List<T>& );
		List_iterator<T>& operator++(); 
		T& operator*(){return cur->value;};
	private:
		Node<T>* cur;
};

template <typename T>

List_iterator<T>& List_iterator<T>::getIter(List<T>& list){ 
	if(list.first){
		cur =list.first;
		return *this; 
	}else{
		throw std::out_of_range("There is nothing to get");
	}
}

template <typename T>
List_iterator<T>& List_iterator<T>::operator++(){
	if(cur){
		cur = cur->next;
		return *this;
	}
	else{
		throw std::out_of_range("There is no next node!!");
	}	
}

// Class List Defination
template <typename T>
class List{
	friend class List_iterator<T>;
	public:
		List(int l = 0,Node<T>* p =0):length(l),first(p){ };
		inline int getLength(){
			return length;
		}
		inline bool isEmpty(){
			return length == 0;
		}
		List& addNode(int,const T&); // add Node on the next Node of the giving location
		List& deleteNode(int,T& ); // delete Node according by its location,1111
		T& find(int value);//find Node by its value;
		List_iterator<T>& begin() ;
	private:
		Node<T>* first;
		List_iterator<T> be;
		int length;
};

template <typename T>
List<T>& List<T>::addNode(int loca,const T& v){
	if(loca < 0 || loca >length) 
		throw std::out_of_range("There is no where to add.");
	if(isEmpty()){
		first = new Node<T>(v,0);
	}
	else{
		if(loca){
			Node<T>* tmp =first;
			for(int i =1; i< loca;++i){
				tmp = tmp->next;
			}
			tmp->next = new Node<T>(v,tmp->next);
		}
		else{
			first = new Node<T>(v,first->next);
		}
	}
	++length;
	return *this;
}
template <typename T>
List<T>& List<T>::deleteNode(int loca,T& v) {
	if(loca <= 0 || loca >length) throw std::out_of_range("There is no Node to delete!!");
	Node<T>* p = first;	
	if(loca == 1){
		v = p->value;
		first = p->next;
		delete p;
	}
	else{
		for(int i=1;i<loca -1; ++i){
			p = p->next;
		}
		Node<T>* cur = p->next;
		p->next = cur->next;	
		v = cur->value;
		delete cur;	
	}
	--length;
	return *this;

}

template <typename T>
List_iterator<T>& List<T>::begin() { 
	return be.getIter(*this);
}

#endif
