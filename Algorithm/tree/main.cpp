#include "BSTree.h"

int main(){


	BSTree<int> t;
	t.insert(5);
	t.insert(6);
	t.insert(3);
	t.insert(2);
	t.insert(10);
	std::cout << "preorder :" << std::endl;
	t.preorder();
	
	std::cout << "preorder_iterate : " << std::endl;
	t.preorder_iterate();

	std::cout << "inorder :" << std::endl;
	t.inorder();
	
	std::cout << "inorder_iterate :" << std::endl;
	t.inorder_iterate();

	std::cout << "postorder : " << std::endl;
	t.postorder();

	std::cout << t.root->val << std::endl;
	std::cout << t.root->left->val << std::endl;
	std::cout << t.root->right->val << std::endl;
	return 0;
}
	
