#ifndef BSTREE_H
#define BSTREE_H
#include <iostream>
#include <stack>

template <typename T>
class BSTreeNode{
	
	public:
	BSTreeNode(T v,BSTreeNode* l = nullptr,BSTreeNode* r = nullptr): val(v),left(l),right(r){}
	
	public:
		T val;
		BSTreeNode* left;
		BSTreeNode* right;

};





template <typename T>
class BSTree{
	public:
		BSTree():root(nullptr),size(0){}
		void insert(T val);
		void preorder(){
			//改进：使用函数对象，使得前中后序中的visit操作得以泛化
			preorder(root);
		};
		void inorder(){
			inorder(root);
		}
		void postorder(){
			postorder(root);
		}
		void preorder_iterate(){
			preorder_iterate(root);
		}
		void inorder_iterate(){
			inorder_iterate(root);
		}

	private:
		void preorder(BSTreeNode<T>*);
		void inorder(BSTreeNode<T>*);
		void postorder(BSTreeNode<T>*);

		void preorder_iterate(BSTreeNode<T>*);
		void inorder_iterate(BSTreeNode<T>*);
		void postorder_iterate(BSTreeNode<T>*);
	public:
		BSTreeNode<T>* root;
		int size;
};



template <typename T>
void BSTree<T>::insert(T val){
	if(root == nullptr){
		root = new BSTreeNode<T>(val);
	}else{
		BSTreeNode<T>* p = root;
		while(p){
			if(val < p->val && p->left != nullptr){
				p = p->left;
				continue;
			}
			else if(val > p->val && p->right != nullptr){
			 	p = p->right;
			 	continue;
			}

			if( p->left == nullptr && val < p->val){
				p->left = new BSTreeNode<T>(val);
				break;
			}
			else{
				p->right = new BSTreeNode<T>(val);
				break;
			}
		}		}
	++size;					
}


template <typename T>
void BSTree<T>::preorder(BSTreeNode<T>* p){
	if(p == nullptr) return;
	std::cout << p->val << std::endl;
	preorder(p->left);
	preorder(p->right);

}

template <typename T>
void BSTree<T>::inorder(BSTreeNode<T>* p){
	if(p == nullptr) return;
	inorder(p->left);
	std::cout << p->val << std::endl;
	inorder(p->right);
}

template <typename T>
void BSTree<T>::postorder(BSTreeNode<T>* p){
	if(p == nullptr) return;
	postorder(p->left);
	postorder(p->right);
	std::cout << p->val << std::endl;
}

template <typename T>
void BSTree<T>::preorder_iterate(BSTreeNode<T>* p){
	std::stack<BSTreeNode<T>*> s;
	BSTreeNode<T>* ptr = p;
	s.push(ptr);
	while(!s.empty()){
		BSTreeNode<T>* t = s.top();
		s.pop();
		//visit
		std::cout << t->val << std::endl;
		//left && right
		if(t->right != nullptr) s.push(t->right);
		if(t->left != nullptr) s.push(t->left);
		
	
	}

}


template <typename T>
void BSTree<T>::inorder_iterate(BSTreeNode<T>* p){
	std::stack<BSTreeNode<T>*> s;
	BSTreeNode<T>* ptr = p;
	while( ptr != nullptr){
		while( ptr != nullptr){
			//stack the right child(if any)
			if( ptr->right != nullptr) s.push(ptr->right);
			s.push(ptr);
			ptr = ptr->left;
		}
		ptr = s.top();
		s.pop();
		while( !s.empty() && ptr->right == nullptr){
			//visit
			std::cout << ptr->val << std::endl;
			ptr = s.top();
			s.pop();
		}
		std::cout << ptr->val << std::endl;	//visit
		if(!s.empty()){
			ptr = s.top();
			s.pop();
		}
		else{
			ptr == nullptr;
		}
	}
}
		











#endif
