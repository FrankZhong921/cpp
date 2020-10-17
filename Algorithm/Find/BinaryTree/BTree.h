#ifndef BTREE
#define BTREE
#include <iostream>
template <typename K,typename V>
class Btree{
	struct Node{
		K key;
		V value;
		int size;
		Node* left;
		Node* right;
		Node(K k,V v, int s):key(k),value(v),size(s),left(0),right(0){};
	};
	
	public:
		Node* root = 0;
		int size(){	return size(root);}  //返回BTree的大小
		V get(K key){ return get(root, key);} //通过key获取对应的value
		void put(K key, V value){	root = put(root,key,value);}  //插入
		K min() { return min(root)->key;} //返回最小键的元素,即最左的叶节点
		K floor(K key) {
			 Node* p = floor(root,key);
			if(!p) {
				std::cout << "There is no Floor Node" <<std::endl;
				return K();
			}
			else return p->key;
		}

		K max(){ return max(root)->key;}
 		K celling(K key){ 
			Node* p = celling(root,key);
			if(!p){
				std::cout << "There is no Celling Node" <<std::endl;
				return K();
			}
			else return p->key;
		}
		K select(int k){
			
			if(k>= size()){
				std::cout << "Out of range!!" <<std::endl;
				return K();
			}
			else {
				return select(root,k)->key;
			}
		}
		int rank(K key){ return rank(root,key);}
	

	private:
		int size(Node* n){
			if(!n) return 0;
			else{
					return n->size;
			}
		}
		
		V get(Node* n, K key){
			//递归查找
			if(!n) {
				std::cerr<< "There is no Node's key is "<< key <<std::endl;
				return V();
			}
			if(n->key < key) return get(n->right,key); //为了高度泛化，该行可进行改进，使用更加抽象的比较方式而不是使用<
			else if(n->key > key) return get(n->left , key);
			else return n->value;

			/*
			//非递归查找
			Node* p = root;
			while(p){
				if(p->key < key) p = p->right;
				else if(p-> key > key) p = p->left;
				else break;
			}
			return p->value; // p有可能是空指针，即找不到<key,value>,此时value必须有定义空值
			*/

		}
	
		Node* put(Node* n, K key, V value){
			//递归方式的插入元素
			if(!n) return new Node(key,value,1);
			if(n->key < key) n->right =put(n->right, key,value);
			else if(n->key > key) n->left = put(n->left , key, value);
			else n->value = value;
			n->size = size(n->left)+size(n->right)+1;
			return n;

			/*
			//非递归插入
			Node* p = root;
			while(p){
				if(p->key < key) p = p->right;
				else if(p-> key > key) p = p->left;
				else break;
			}
			p = new Node(key,value,1);
			*/
		}

		Node* min(Node* n){
			if(!n) std::cout << "There is No Node" <<std::endl;
			if(!n->left) return n;
			return min(n->left);

			/*非递归实现
			Node* p = root;
			if(!p) std::cout << "There is No node" << std::endl;
			while(p->left){
				p = p->left;
			}
			return p->value;
			*/
		}

		
 		Node* floor(Node* n,K key){
			//返回小于等于key的最大值，
			//因此需要先找到一个小于key的数再慢慢增大
			if(!n) return 0;
			if(n->key == key) return n;
			else if(n->key > key) return floor(n->left,key);
			Node* t = floor(n->right,key);
			if(t) return t;
			else return n;
		}
		 
		Node* max(Node* n){
			if(!n) std::cout << "There is No Node" <<std::endl;
			if(!n->right) return n;
			else n = max(n->right);
		}

		Node* celling(Node* n,K key){
			//返回大于等于key的最小值
			//因此需要先找到一个比key大的数再慢慢减小
			if(!n) return 0;
			if(n->key == key) return n;
			else if(n->key < key) return celling(n->right,key);
			Node* t = celling(n->left,key);
			if(t) return t;
			else return n;
			
		}
 
		Node* select(Node* n,int k){
			if(!n) return 0;
			int t = size(n->left);
			if(t>k) return select(n->left, k);
			else if( t < k) return select(n->right,k-t-1);
			else return n;
		}
			
		int rank(Node* n,K key){
			if(!n) return 0;
			if(n->key > key) return rank(n->left,key);
			else if(n->key < key) return 1+size(n->left) + rank(n->right,key);
			else return size(n->left);
		}
				
		


};
#endif
