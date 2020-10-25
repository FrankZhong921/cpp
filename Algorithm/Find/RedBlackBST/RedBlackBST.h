#ifndef REDBLACKBST
#define REDBLACKBST

/*   定义
红黑平衡二叉树（2-3树）:红链接均为左链接（由于一个结点都有唯一一个链接指向它，因此结点颜色代表指向该结点的链接颜色，等价于只有左红子结点）
						没有任何一个结点同时和两条红链接相连（不存在4-结点）
						该树是完美黑色平衡的，即任意空链接到根结点的路径上的黑链接数量相同
			
*/



const bool BLACK = false;
const bool RED = true;

/*
//直接使用模板还是继承比较好？
若为继承，可在基类定义关于键的比较方式，
而若使用只用模板，则必须保证使用自己定义的键必须定义了比较方法

*/

template<typename K, typename V> 
class RedBlackBST{
	struct Node{
		K key;
		V val;
		Node* left;
		Node* right;
		int size;
		bool color;
		Node(K k,V v,int s,bool c):key(k),val(v),size(s),color(c),left(0),right(0){};

	};
	public:
			Node* root = 0; //root必须初始化后才能用，可以创建构造函数,否则segment fault
			int size(){
				return size(root);
			}

			void put(K key,V val){
				root = put(root,key,val);
				root->color = BLACK;
			}
			
			void deleteMin(){
				if(!root) std::cerr<<"RedBlackTree underflow"<< std::endl;
				if(!isRed(root->left)&&!isRed(root->right)){
					root->color =RED;  //也可使用flipcolors,但是与后面的deleteMin重复，而且还需将root->color置为黑
				}
				root = deleteMin(root);
				if(root) root->color = BLACK;
			}
			void deleteNode(K k){
				if(!root) std::cerr<<"RedBlackTree underflow"<< std::endl;
				if(!isRed(root->left)&&!isRed(root->right)){
					root->color =RED;  //也可使用flipcolors,但是与后面的deleteMin重复，而且还需将root->color置为黑
				}
				root = deleteNode(root,k);
				if(root) root->color = BLACK;
			}
			V get(V v){
				return get(root,v);
			}
			
			//中序遍历
			void Midsearch(Node* p){
				if(!p) return;
				Midsearch(p->left);
				std::cout << p->key <<std::endl;
				Midsearch(p->right);
				return;
			}
				

	private:
			inline bool isRed(Node* n ){ 
				if(!n) return false;
				else return n->color == RED;
			}

			inline int size(Node* n){
				if(!n) return 0;
				else return n->size;
			}
			inline void flipColors(Node* n){
				if(!n) return;
 				n->color = !n->color;
				n->left->color = !n->left->color;
				n->right->color = !n->right->color;
				return;
			}
			
			Node* balance(Node* n){
				if(isRed(n->right)) n =rotateLeft(n);
				if(isRed(n->left)&&isRed(n->right)) n = rotateRight(n);
				if(isRed(n->left)&&isRed(n->right)) flipColors(n);
				n->size = size(n->left) + size(n->right) + 1;
				return n;	

			}
			
			V get(Node* n,K k){
				while(n){
					if(n->key > k) n = n->left;
					else if(n->key < k) n = n->right;
					else return n->val;
				}
				return V();
			}
		
			bool contain(K k){
				return get(root,k) != V();
			}
 			Node* rotateLeft(Node* n){  //当右子结点是红结点时需要左旋
				Node* t = n->right;
				n->right = t->left;
				t->left = n;
				t->color = n->color;//保留其父结点与子结点的链接颜色
				n->color = RED;
				n->size = size(n->left) +size(n->right)+1;  //这里应该更新n（作为子结点的先更新）
				return t;
			}
				
			Node* rotateRight(Node* n){
				Node* t = n->left;
				n->left = t->right;
				t->right = n;
				t->color = n->color; //保留其父结点与子结点的链接颜色
				n->color = RED;
				n->size = size(n->left) + size(n->right) +1;  //这里应该更新n（作为子结点的先更新）
				return t;
			}
				
			Node* put(Node* n,K key,V val){
				
				if(!n) return new Node(key,val,1,RED);
				//将flipColors及其条件判断语句置于该处可实现2-3-4树的插入操作
				if(n->key > key) n->left = put(n->left,key,val);
				else if(n->key < key) n->right = put(n->right,key,val);
				else{
					n->val = val;//键值若存在则只需修改
				}
				if(!isRed(n->left) && isRed(n->right)) n = rotateLeft(n); //新键在3-中间
				if(isRed(n->left) && isRed(n->left->left)) n = rotateRight(n);//新建小于3-
				if(isRed(n->left)  && isRed(n->right)) flipColors(n);//新键大于3-
				n->size = size(n->left)+size(n->right)+1;
				
				return n; 
			}
		
			Node* deleteMin(Node* n){
				if(!n->left) return 0;
				if(!isRed(n->left)&&!isRed(n->left->left)){//如果是2-结点，采取相应操作
					n = move(n);
				}
				n->left = deleteMin(n->left);
				return balance(n);
			}
			
			Node* move(Node* n){
				flipColors(n);
				if(isRed(n->right->left)){
					n->right = rotateRight(n->right);
					n = rotateLeft(n);
					flipColors(n);
				}
				return n;
			}
				







};

#endif
