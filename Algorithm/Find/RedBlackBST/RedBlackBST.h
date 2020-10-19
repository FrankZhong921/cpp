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
				n->left->color = BLACK;
				n->right->color = BLACK;
				return;
			}
			Node* rotateLeft(Node* n){  //当右子结点是红结点时需要左旋
				Node* t = n->right;
				n->right = t->left;
				t->left = n;
				t->color = n->color;//保留其父结点与子结点的链接颜色
				n->color = RED;
				t->size = size(t->left) +size(t->right)+1;
				return t;
			}
				
			Node* rotateRight(Node* n){
				Node* t = n->left;
				n->left = t->right;
				t->right = n;
				t->color = n->color; //保留其父结点与子结点的链接颜色
				n->color = RED;
				t->size = size(t->left) + size(t->right) +1;
				return t;
			}
				
			Node* put(Node* n,K key,V val){
				std::cout << "start put" <<std::endl;
				if(!n) return new Node(key,val,1,RED);
				if(n->key > key) n->left = put(n->left,key,val);
				else if(n->key < key) n->right = put(n->right,key,val);
				else{
					n->val = val;//键值若存在则只需修改
				}
				if(n->right->color== RED && n->left->color != RED) n = rotateLeft(n); //新键大于3-
				if(n->left->color == RED && n->left->left->color == RED) n = rotateRight(n);//新建在3-中间
				if(n->left->color == RED && n->right->color == RED) flipColors(n);//新键小于3-
				n->size = size(n->left)+size(n->right)+1;
				return n;
			}
		









};

#endif
