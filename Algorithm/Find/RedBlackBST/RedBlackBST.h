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
			void deleteMax(){
				if(!root) std::cerr << "RedBlackBST underflow" << std::endl;
				if(!isRed(root->left) && !!isRed(root->right)) {
					root->color = RED;
				}
				root = deleteMax(root);
				if(root) root->color = BLACK;
			}
			void deleteNode(K k){
				if(!root) std::cerr<<"RedBlackTree underflow"<< std::endl;
				if(!isRed(root->left)&&!isRed(root->right)){ //左右子结点都是黑结点
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
					n = moveLeft(n);
				}
				n->left = deleteMin(n->left);
				return balance(n);
			}

			Node* deleteMax(Node *n){
				if(isRed(n->left)) n = rotateRight(n);
				if(!n->right) return 0;
				if(!isRed(n->right) &&!isRed(n->right->left)){//如果是2-结点，采取相应操作
					n = moveRight(n);

				}
				n->right = deleteMax(n->right);
				return balance(n);
					
			}
			Node* moveLeft(Node* n){
				flipColors(n);
				if(isRed(n->right->left)){ //如果兄弟结点是3-，4-结点则将其借给父结点
					n->right = rotateRight(n->right);
					n = rotateLeft(n);
					flipColors(n);
				}
				return n;
			}
			Node* moveRight(Node* n){
				flipColors(n);
				if(isRed(n->left->left)){
					n =rotateRight(n);
					flipColors(n);
				}
				return n;
			}
				
			Node* deleteNode(Node* n,K k){
				//if(!n) return balance(n);  //没有该元素，直接恢复树的形状，如果能先判断有无该结点就不用往下逐层拆解
				if(k < n->key){
					if(!isRed(n->left)&&!isRed(n->left->left)){ //2-结点
						n = moveLeft(n);
					}
					n->left = deleteNode(n->left,k);
				}
				/*
				else if(k > n->key){
					if(isRed(n->left)) n = rotateRight(n);
					if(!isRed(n->right)&&!isRed(n->right->left)){
						n = moveRight(n);
					}
					
					n->right = deleteNode(n->right,k);
				}
				*如果像的deleteMax一样先右转，那么原先可能作为左子结点的目标值将没有参与比较，必须再进行比较。				
				*也即每次旋转都需要再比较一次

				*/
				else{ 	//如果目标key大于等于结点key
					if(isRed(n->left)) n = rotateRight(n);//转换为右红
					if(k == n->key && !n->right){ //当前结点是红结点且没有子结点
						return 0;
					} 
					if(!isRed(n->right)&&!isRed(n->right->left)){
						n = moveRight(n);
					}
					if (k == n->key) { //当前结点是红结点且有子结点
               					 n->val = get(n->right, min(n->right)->key);
              				 	 n->key = min(n->right)->key;
               					 n->right = deleteMin(n->right);
         				}
					else n->right = deleteNode(n->right, k);
				}
				return balance(n);
			}

			
			K min() {
       				 if (!root) return 0;
       				 return min(root)->key;
    			} 
			Node* min(Node* x) { 
      			  
        			if (!x->left) return x; 
       				else   return min(x->left); 
    			} 
				





};

#endif
