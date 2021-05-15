# wcpp

## c

- c与c++的区别

  - 明确一个，C++不是C的一个超集。

  - 并且在编写C++时尽量避免使用诸如(void*) 之类的程序风格。

  - 在不得不使用C时，应该尽量使用**extern "C"** 这种特性，将C语言代码和C++代码进行分离编译，再统一链接这种做法。

    ``` #ifdef __cplusplus ``` 

     ```extern "C"{```

     ```#endif```

     ```int add(int x, int y);```

     ```#ifdef __cplusplus```

     ```}```

     ```#endif ```

- c++98和c99的区别

## c++ 11

- STL
- 左值右值

### 被弃用的特性

- 不再允许将字符串字面值常量赋值给一个char*。应该使用const char * 或者 auto。
- auto_ptr被弃用，应使用unique_ptr。
- register关键字被弃用，可以使用但不再具有任何实际含义
- bool类型的++操作被弃用
- 如果一个类有析构函数，为其生成拷贝构造函数和拷贝赋值运算符的特性被弃用了。
- C语言风格的类型转换被弃用（即在变量前使用(convert_type)），应该使用static_cast、reinterpret_cast、const_cast来进行类型转换。

### nullptr

​	nullptr出现的目的为了替代NULL。传统C++会将NULL、0视为同一种东西，这取决于编译器如何定义NULL。

​	**C++不允许直接将void * 隐式转换到其他类型**，将NULL定义成((void *)0)，那么

` char* ch = NULL; `

没有了void * 的隐式转换的C++将NULL定义为0，这将导致C++中**重载**特性发生混乱。

``` void foo(chat*);``` 

```void foo(int);```  

​	为了解决这个问题，c++引入了nullptr关键字，专门用来区分空指针和0。而nullptr的类型为 nullptr_t ，能够隐式的转换为任何指针或成员指针的类型，也能和他们进行相等或不等的比较。

```if(std::is_same<decltype(NULL), decltype(0)>::value) ```

 ```std::cout << “NULL == 0” << std::endl;```

 ```if(std::is_same<decltype(NULL),std::nullptr)>::value) ```

```std::cout << "NULL == nullptr" << std::endl;```

```if(std::is_same<decltype(NULL),dectype((void*)0)>::value) ```

```std::cout << "NULL == (void*)0" << td::endl;```

​	从上面代码可知得出：NULL与nullptr和0各不相同。

### constexpt

​	不仅可以用于表达式如 1*2，3+4中，也可以用在函数，失能编译器再编译时就能够将表达式或者函数直接优化并植入到程序运行时，将能增加程序的性能。

​	C++11提供了*constexpr*让用户显示的声明函数或对象构造函数在编译器会成为常量表达式，这个关键字明确的告诉编译器应该去验证函数在编译器就应该时一个常量表达式。

​	此外，*constexpr*修饰的函数可以使用**递归**。

``` c++
constexpr int fibonacci(const int n){
	return n == 1 || n == 2? 1:fibonacci(n-1) + fibonacci(n-2);
}
```

​	从**c++14**开始，*constexpr*函数可以在内部使用局部变量、循环和分支等简单语句，例如下面函数形式在**c++11**标准下时不能够通过编译的：

```c++
constexpr int fibonacci(const int n){
	if(n == 1) return 1;
	if(n == 2) return 1;
	return fibonacci(n-1) + fibonacci(n-2);
}
```

### 初始化列表（POD）

- POD(Plain OLd Data)：即没有构造，析钩和需函数的类或结构体类型
- 在传统Ｃ++中，不同对象有着不同的初始化方法，例如**普通数组类型**，**POD类型**，都可以用｛｝进行初始化，也就是我们所说的初始化列表．而对于类对象的初始化，要么通过拷贝构造，要么使用（）进行．
- 为了解决这个问题，C++11首先１把初始化列表的概念绑定到类型上，并将其称为*std::initializer_list*，这就为类对象的初始化与普通数组和POD类型的初始化提供了统一的桥梁．

### 结构化绑定

​	结构化绑定提供了类似其他语言类型提供的多返回值的功能．Ｃ++11提供了*std::tuple*容器用于构造一个元组，进而囊括多个返回值，但缺陷是，C++11/14并没有提供一种简单那的方法直接从元组中拿到并定义元组中的元素，尽管可以使用*std::tie*对元组进行拆包，但我们依然必须非常清楚这个元组包含多少个对象，各个对象是什么类型．

- C++17完善这一设定，给出的结构化绑定可以让我们写出这样的代码

  ```c++
  #include <iostream>
  #include <tuple>
  std::tuple<int,double,std::string> f(){
  	return std::make_tuple(1,2.3,"4.5");
  }
  
  int main(){
  	auto [x,y,z] = f();
  	std::cout << x << "," << y <<"," << z <<std::endl;
  	return 0;
  }
  ```


### 变长参数模板

在**c++11之前**，无论是类模板还是函数模板，都只能按其指定的样子，接受一组固定数量的模板参数；而**c++11加入了新的表示方法，允许任意个数、任意类别的模板参数，同时也不需要在定义时将参数的个数固定。**

```c++
template<typename... Ts> class Magic; //可以接收任意个数的模板参数
class Magic<int,std::vector<int>,std::map<std::string>>;
```

既然是任意数量，所以个数为0的模板参数也是可以的：`class Magic<> nothing;`

变长参数模板也能被直接调整到模板函数上。传统C中的printf函数，虽然也能达成不定个数的形参调用，但并非类型安全。而c++11除了能定义类型安全的变长参数模板外，也可以使类似printf的函数能自然地处理非自带类别的对象。

```template <typename Require,typename... Args> class Magic;```

- 如何对变长的模板参数进行解包？

  1. 可以使用 ***sizeof...*** 来计算参数的个数

     ```c++
     template<typename... Ts>
     void magic(Ts... args){
     	std::cout << sizeof...(args) << std::endl;
     }
     
     magic(); 	
     magic(1);
     magic(1,"")
     ```

  2. 目前还没有一种简单的方法能够处理参数包，但有两种经典处理方法

     1. 递归模板参数

        ```c++
        #include <iostream>
        template<typename T0>
        void printf1(T0 value) {	//如果不用单参数的重载是否能够顺利运行
        	std::cout << value << std::endl;
        }
        //由于可变长参数模板支持任意数量，因此即使是0参数也能继续递归，导致没有退出条件，必须定义单参数重载。
        template<typename T, typename... Ts>
        void printf1(T value, Ts... args) {
        	std::cout << value << std::endl;
        	printf1(args...);
        }
        
        int main() {
        printf1(1, 2, "123", 1.1);
        return 0;
        }
        
        ```

     2. 上面比较繁琐，在**c++17**中增加了变参模板展开的支持，可以在一个函数中完成pritnf的编写

        ```c++
        template<typename T0,typename... T>
        void printf2(T0 t0,T... t){
        	std::cout << t0 << std::endl;
        	if constexpr(sizeof...(t) > 0) printf2(t...);
        }
        ```

        事实上，有时候我们虽然使用了变参模板，却不一定需要对参数做逐个遍历，我们可以利用 std::bind 及完美转发等特性实现对函数和参数的绑定，从而达到成功调用的目的。

     3. **初始化列表展开**

        使用递归模板函数是一种标准做法，但缺点在于必须定义一个终止递归的函数。

        ```c++
        template<typename T,typename... Ts>
        auto printf3(T value, Ts... args){
        	std::cout << value << std::endl;
        	(void) std::initializer_list{
        		([&args]{std::cout << args << std::endl;}() , value)...
        	}
        }
        ```

        在这个代码中，额外**使用了 C++11 中提供的初始化列表以及 Lambda 表达式的特性**（下一节中将 提到）。 通过初始化列表，(lambda 表达式, value)... 将会被展开。由于逗号表达式的出现，首先会执行 前面的 lambda 表达式，完成参数的输出。**为了避免编译器警告，我们可以将 std::initializer_list 显式的转为 void。**

        

## c++ 14

### if/switch变量声明强化

​	c++11及之前，只有*for*循环中才能有声明，其它例如*if* *switch*中不能有局部变量声明。

```c++
//将临时变量放到if语句内
if(auto itr = std::find(vec.begin(),vec.end(),3); itr != vec.end()){
    *itr = 4;
}
```

### 尾后返回类型的推导

​	auto不能用于函数形参进行类型推导 `int add(auto x,auto y)` ，那是否能用于尾后推导

​	在传统c++中尾后类型推导只能这样写：

```c++
template <typename A, typename B,typename C>
A add(B x,C y){
	return x+y;
}
```

但这样写使得在使用这个模板函数时必须指出返回类型。但我们并不知道这个函数会做什么样的操作。

可能马上会想到：

```c++
template<typename A,typename B>
decltype(x+y) add(A x,B y){
	return x+y;
}
```

**但是在编译器读到*decltype(x+y)*时x, y还未定义**，使用c++11引入的尾后返回类型推导

```c++
template<typename A,typename B>
auto add(A x,B y)-> decltype(x+y){
	return x+y;
}
```

**在c++14中，可以直接让普通函数具备返回值推导**

```c++
template<typename A,typename B>
auto add(A x,B y){
	return x + y;
}
```



### decltype(auto)

****

​	decltype(auto)是C++14开始提供的略微复杂的用法．

- 主要用于对转发函数或封装的返回类型的推导，它使我们无需显式指定decltype的参数表达式

``` c++
//如下例子
std::string lookup1();
std::String lookup2();

//c++11中封装为如下形式
std::string look_up_a_string_1(){
	return lookup1();
}
std::string look_up_a_string_2(){
	return lookup2();
}
//有了decltype(auto),可以让编译器完成这一件烦人的参数转发
decltype(auto) look_up_a_string_1(){
    return lookup1();
}

decltype(auto) look_up_a_string_2(){
    return lookup2();
}
```



## c++ 17

### if constexpr

​	c++11引入了*constexpr*关键字，将表达式或函数编译为常量的结果．很容易想到，如果把这一特性引入到条件判断中去，让**代码在编译时就完成分支判断**，岂不是能让程序效率更高？

* c++17将*constexpr*这个关键字引入到*if*语句中，允许代码中声明常量表达式的判断条件．

```c++
template<typename T>
auto print_type_info(const T& t){
	if constexpr(std::is_integeral<T>::value){
		return t + 1;
	}else{
		return t + 0.001;
	}
}
int main(){
	std::cout << print_type_info(5) << std::endl;
	std::cout << print_type_info(3.14) << std::endl;
}

//经编译后形成的实际代码如下：
int print_type_info(const int & t){
	return t + 1;
}
double print_type_info(const double& t){
	return t + 0.001;
}
```

### 折叠表达式

​	c++17将**变长参数模板**的这种特性进一步带给了表达式。

```c++
#include <iostream>
template<typename... T>
auto sum(T... t){
	return (t + ...);
}
int main(){
	std::cout << sum(1,2,3,4,5,6) << std::endl;
}
```

### 非类型模板参数推导

一种常见模板参数形式可以让不同**字面量成为模板参数**，即非类型模板参数

```c++
template <typename T, int BufSize>
class buffer_t {
public:
    T& alloc();
    void free(T& item);
private:
    T data[BufSize];
}

buffer_t<int, 100> buf; // 100 作为模板参数
```

​	既然此处的模板参数以具体的字面量进行传递，能否让编译器辅助我们 进行类型推导，通过使用占位符 auto 从而不再需要明确指明类型？C++17 引入了这一特性， 我们的确可以 auto 关键字，让编译器辅助完成具体类型的推导。

```c++
template <auto value> void foo() {
    std::cout << value << std::endl;
    return;
}
int main() {
    foo<10>(); // value 被推导为 int 类型
}
```



实现文件系统



## c++20



## 