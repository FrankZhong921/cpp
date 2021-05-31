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
		void preorder_iterate(){
			preorder_iterate(root);
		}
		void preorder_morris(){
			preorder_iterate(root);
		}

		void inorder(){
			inorder(root);
		}
		void inorder(){
			inorder_iterate(root);
		}
		void inorder_morris(){
			inorder_morris(root);
		}

		void postorder(){
			postorder(root);
		}
		void postorder_iterate(){
			postorder_iterate(root);
		}
		void postordre_morris(){
			postorder_morris(root);
		}
	private:
		void preorder(BSTreeNode<T>*);
		void preorder_iterate(BSTreeNode<T>*);
		void preorder_morris(BSTreeNode<T>*);

		void inorder(BSTreeNode<T>*);
		void inorder_iterate(BSTreeNode<T>*);
		void inorder_morris(BSTreeNode<T>*);

		void postorder(BSTreeNode<T>*);
		void postorder_iterate(BSTreeNode<T>*);
		void postorder_morris(BSTreeNode<T>*);
	public:
		BSTreeNode<T>* root;
		int size;
};



template <typename T>
void BSTree<T>::insert(T val){
	BSTreeNode<T>* p = root, *prev = nullptr;
	while(p != nullptr){
		prev = p;
		if( val < p->val)  p = p->left;
		else		   p = p->right;
	}
	if( root == nullptr) root = new BSTreeNode<T>(val);
	else if(val < prev->val) prev->left = new BSTreeNode<T>(val);
	else	prev->right = new BSTreeNode<T>(val);
	++size;
}

//递归前序
template <typename T>
void BSTree<T>::preorder(BSTreeNode<T>* p){
	if(p == nullptr) return;
	std::cout << p->val << std::endl; //visit
	preorder(p->left);
	preorder(p->right);

}
//递归中序
template <typename T>
void BSTree<T>::inorder(BSTreeNode<T>* p){
	if(p == nullptr) return;
	inorder(p->left);
	std::cout << p->val << std::endl; //visit
	inorder(p->right);
}

//递归后序
template <typename T>
void BSTree<T>::postorder(BSTreeNode<T>* p){
	if(p == nullptr) return;
	postorder(p->left);
	postorder(p->right);
	std::cout << p->val << std::endl; //visit
}
//迭代前序
template <typename T>
void BSTree<T>::preorder_iterate(BSTreeNode<T>* p){
	std::stack<BSTreeNode<T>*> s;
	BSTreeNode<T>* ptr = p;
	s.push(ptr);
	while(!s.empty()){	//注意条件
		BSTreeNode<T>* t = s.top();
		s.pop();
		//visit
		std::cout << t->val << std::endl;
		if(t->right != nullptr) s.push(t->right);
		if(t->left != nullptr) s.push(t->left);
		
	
	}

}

//迭代中序
template <typename T>
void BSTree<T>::inorder_iterate(BSTreeNode<T>* p){
	std::stack<BSTreeNode<T>*> s;
	while(p != nullptr || !s.empty()){	//注意条件
		while( p != nullptr){
			s.push(p);
			p = p->left;
		}
		p = s.top();
		s.pop();
		//visit
		std::cout << p->val << std::endl;
		p = p->right;
	}
}
//Morris迭代中序空间复杂度O(1);
template <typename T>
void BSTree<T>::inorder_morris(BSTreeNode<T>* p){
	BSTreeNode<T>* predecessor = nullptr;
	while(p != nullptr){	//注意条件
		//如果有左子节点，找predecessor,若predecessor的右子节点为空则与p相连，否则访问并p=p->right
		if(p->left != nullptr){

			predecessor = p->left;
			while(predecessor->right != nullptr && predecessor->right != p){	//注意条件
				predecessor = predecessor->right;
			}
			if(predecessor->right == nullptr){
				predecessor->right = p;
				p = p->left;
			}
			//说明左子树已经访问完，需要恢复结构，断开连接
			else{
				std::cout << p->val << std::endl;//visit
				predecessor->right = nullptr;
				p = p->right;

			}
		}
		//如果没有左子节点,则直接访问右子节点
		else{
			std::cout << p->val << std::endl;	//visit
		
			p = p->right;
		}
	}


}

template <typename T>
void BSTree<T>::postorder_iterate(BSTreeNode<T>* p){
	std::stack<BSTreeNode<T>*> s;
	//判断是否为空
	BSTreeNode<T>* prev = nullptr;

	while(p != nullptr || !s.empty()){
		while(p != nullptr){
			s.push(p);
			p = p->left;
		}	
		//读取左子树的最左节点
		p = s.top();
		s.pop();
		//判断该最左节点是否有右子树或者该右子树已经访问过，若没有则访问,有则重新入栈，进入右子树
		if( p->right == nullptr || p->right == prev){
			std::cout << p->val << std::endl; //visit
			prev = p;
			p = nullptr;
		}else{
			s.push(p);
			p = p->right;
		}		

	}
}




template <typename T>
void BSTree<T>::postorder_morris(BSTreeNode<T>* p){
	

}




#endif
