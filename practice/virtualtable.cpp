#include <iostream>

using namespace std;

class A{
	public:	A(){
			cout << "A constructor "<< endl;
		}

		virtual void ope(){
			cout << "A oper.... " << endl;
		}
};


class B{
	public: 
		B(){
			cout << "B constructor "<< endl;
		}
		virtual void ope(){
			cout << "B oper.... " << endl;
		}
};

class C : public A,public B{
	public: C(){
		cout << "C consturctor " << endl;
		}
		
		virtual void oper(){
			cout << "C oper...." << endl;
		}
		
};


int main(){
	/*
	A* ptr = new C();
	cout << "派生类对象的所在首地址（虚表指针）为："<<  ptr << endl;
	ptr->ope();
	
	uintptr_t** vptr = reinterpret_cast<uintptr_t**>(ptr); //虚表指针
	cout << "虚表指针" << vptr << endl;
	uintptr_t* vtbl = *vptr;	//虚函数指针
	uintptr_t func = *vtbl;		//虚函数
		
	auto p = reinterpret_cast<void(*)()>(func);
	//cout  << "\n 该函数指针的地址为"<< p << endl;
	p();
	cout << "指针大小"<< sizeof(void(*)()) << endl;
	uintptr_t func1 = *(vtbl+1);
	auto p2 = reinterpret_cast<void(*)()>(func1);
	p2();

	*/

	/*	虚表的结构
	 *	vtp -------->Ａ的虚表｜Ａ｜Ｃ｜
	 *	vtp+1 ------>Ｂ的虚表｜Ｂ｜
	 *
	 */
	C tmp;
	uintptr_t** vtp = reinterpret_cast<uintptr_t**>(&tmp);//　vtp指向Ｃ对象的首地址，该收地址的值是指向虚表数组的指针
	uintptr_t* vfpA = *vtp;		//Ｃ继承Ａ下的虚函数表，vfpA是虚表指针
	uintptr_t* vfpB = *(vtp+1);	//Ｃ继承Ｂ下的虚函数表，vfpA是第二个虚表指针
	uintptr_t vpAA = *vfpA;		//Ａ虚函数表的第一个位置
	uintptr_t vpAC = *(vfpA+1);	//Ａ虚函数表的第二个位置
	uintptr_t vpBB = *vfpB;
	uintptr_t vpBC = *(vfpB+1);
	auto fAA = reinterpret_cast<void(*)()>(vpAA);
	fAA();
	auto fAC = reinterpret_cast<void(*)()>(vpAC);
	fAC();
	auto fBB = reinterpret_cast<void(*)()>(vpBB);
	fBB();
	auto fBC = reinterpret_cast<void(*)()>(vpBC);
	fBC(); //这个调用会出错，因为第二个虚函数表并没有派生类的相关函数

}
	
