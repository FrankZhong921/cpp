#include "BTree.h"
#include <string>
 
int main(){
	Btree<std::string,std::string> tree;
	
	tree.put("a","key is a");
	tree.put("b","key is b");
	tree.put("abc","key is abc");

	std::cout << tree.get("o") <<std::endl;
	std::cout << tree.floor("ab") << std::endl;
	std::cout << tree.celling("df") << std::endl;
		



	std::cout << tree.size() << std::endl;
	std::cout << (tree.root)->size  << std::endl;
	std::cout << tree.select(3) << std::endl;
	std::cout << tree.max() <<std::endl;
	std::cout << tree.min() <<std::endl;
}
