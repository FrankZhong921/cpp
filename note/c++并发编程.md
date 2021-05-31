#　第一章

- 

# 第二章：管理线程

**所有的管理都是通过和给定线程关联的std::thread对象完成．**

## 线程管理基础

### 启动线程

线程在构造std::thread对象时启动，这个对象指定了要运行的任务。

- 最简单的情况下返回void的无参数函数。这个函数在其所属线程上运行，直到它返回，线程也就结束了。
- 另一个极端，任务可以是个带有参数的函数对象，在运行的时候，它执行一系列通过消息系统指定的独立的操作，只有通过消息系统明确通知它结束后线程才会停止

```c++
/*std::thread接受任何可调用类型，所以你可以传递一个重载了调用操作符的类实例给std::thread构造函数*/
void do_some_work();
std::thread my_thread(do_some_work);
```

需要注意的是，当把函数对象传入到线程构造函数中时，需要避免“[最令人头痛的语法解析](https://link.zhihu.com/?target=http%3A//en.wikipedia.org/wiki/Most_vexing_parse)”，如果你传递了一个临时变量，而非命名变量，编译器会将其解析为函数声明，而不是对象的定义。

``````c++
std::thread my_thread(do_some_work());//编译不通过
``````

- 解决办法

  1. 使用额外的括号

     ```c++
     std::thread my_thread((background_task())); //g++10.2.0编译不通过
     ```

  2. 使用新的统一初始化语法，花括号

     ```c++
     std::thread my_thread{background_task()}; //g++10.2.0编译不通过
     ```

  3. 可调用对象类型是lambda表达式

     ```c++
     std::thread my_thread([]{	//g++ 10.2.0编译通过
      do_something();
      do_something_else();
     });
     ```

### 连接或分离

每个C++程序至少有一个线程：由**C++运行时库启动的执行main()函数的线程。然后你的程序可以以不同的函数作为入口点启动其他线程。**这些线程彼此并发的运行。一旦启动了线程，你需要明确是要等待线程结束，还是让它自主运行．

如果std::thread对象销毁之前还没有做出决定，程序就会终止(std::thread的析构函数会调用***std::terminate()***　)。

如果不等待线程结束，你需要确保线程访问的数据直到线程结束之前都是有效的。

- spawing thread孵化线程即主线程将自身变量传入，并创建新线程，而由于线程分离主线程提前结束，导致该变量被销毁，若新线程对其访问将为定义．
- 处理这种情况的通用做法是让线程函数自包含,将数据拷贝到线程中，而非共享数据。因此原始对象可以被立即销毁。

---

1. **主线程等待线程完成**

   如果需要等待线程完成，可以调用std::thread实例的join()函数。此时可以确保传入的变量在线程完成后才被销毁．

   - join()是简单粗暴的技术——要么等待线程完成，要么不等待，当需要更细粒度的控制，比如，检查某个线程是否结束，或者只等待一段时间。**需要使用其他机制来完成，比如条件变量和期望(futures)完成更精细的控制**

   - **对一个线程只能调用一次join()**；一旦已经调用过join()，std::thread对象就不能再次连接，同时 ***joinable()***　将返回false。

   - **需要确保销毁std::thread对象之前调用join()或detach()。**如果需要分离一个线程，通常可以在线程启动后，立刻调用detach()进行分离，因此这不算啥事。

     ​	但如果打算等待线程，则需要细心挑选调用join()代码的位置。这意味着如果**在线程启动后，调用join()前有异常抛出**，join()调用很容易被跳过。

     - 为了避免应用因为异常抛出而终止，需要在异常处理过程中调用join()

       ```c++
       struct func;
       void f(){
       	int some_local_state = 0;
       	func my_func(some_local_state);
       	std::thread t(my_func);
       	try{
       		do_something_in_current_thread();
       	}
       	catch(...){
       		t.join();
       		throw;
       	}
       	t.join();
       }
       ```

       try/catch块比较冗长，而且容易把作用域搞乱.

       使用标准的“资源获取即初始化”(RAII，Resource Acquisition Is Initialization)，并且**提供一个类，在析构函数中执行join()**

       ```c++
       class thread_guard{
       	std::thread& t;
       	public:
       		thread_guard(std::thread＆ t_):t(t_){}
       		~thread_guard(){
       			if(t.joinable()){
       				t.join();
       			}
       			//join()只能对给定的对象调用一次，所以连接已经连接过的线程将会导致错误。
       		}
       		thread_guard(const thread_guard&)=delete;
       		thread_guard& operator=(const thread_guard&) = delete;
       };
       struct func;
       void f(){
       	int some_local_state = 0;
       	func my_func(some_local_state);
       	std::thread t(my_func);
       	thread_guard(t);
       	do_some_thing_in_current_thread();
       }
       ```

       当线程执行到f的尾部时，局部对象就要按照构造的逆序销毁。因此，thread_guard对象g是第一个被销毁的，这时线程在析构函数中被连接。即使do_something_in_current_thread抛出一个异常，这个销毁依旧会发生。

2. **分离线程（在后台运行线程）**

   如果线程被分离了，那就不可能有std::thread对象能引用它．分离的线程的确在后台运行；所有权和控制会传递给C++运行时库，它会保证和线程相关的资源在线程退出的时候被正确的回收。

   - 分离线程经常叫做***守护线程***(daemon threads)这是参照UNIX中守护进程的概念,它们运行在应用的整个生命周期中，可能会在后台监视文件系统，还有可能清理没用的对象缓存，亦或优化数据结构。

   调用std::thread成员函数***detach()***来分离一个线程。

   ```c++
   std::thread　t(do_background_work);
   t.detach();
   //只有当std::thread对象t执行t.joinable()返回true，才可以使用t.detach()。
   assert(!t.joinable());
   ```



- 传递参数给线程函数

  只需要将这些参数作为`std::thread`构造函数的附加参数即可。默认情况下这些参数被拷贝到新线程中

  ```c++
  void f(int i,const std::string& s);
  std::thread t(f,3,"hello");
  ```

  创建了一个和t关联的新的执行线程，这个线程调用f(3, "hello")。注意，**虽然f需要一个`std::string`作为第二个参数，但字符串的字面值作为`char const *`类型传递，然后只在新线程的上下文转换为一个std::string对象**

  ```c++
  void f(int i,const std::string& s);
  void oop(int some_param){
  	char buffer[1024];
  	std::thread　t(f,3,buffer);
  	t.detach();
  }
  ```

  ​	由于只在新线程的上下文发生**指向buffer的指针**转换成**std::string对象**的**隐式转换**，而oop函数可能在转换之前就结束，导致buffer变量销毁，新线程中指针将不能访问到buffer．**需要在传入之前就显式转换**．

  ```
  void  oop(int some_param){
  	char buffer[1024];
  	std::thread t(f,3,std::string(buffer));
  	t.detach();
  }
  ```

- 传递的参数是非常量的指针或引用

- 传递的参数不能被拷贝，只能移动

  - 使用std::move()
  - 虽然，`std::thread`实例不像`std::unique_ptr`那样能占有一个动态对象的所有权，但它确实拥有一个资源：每个实例都负责管理一个执行线程。这个所有权可以在多个`std::thread`实例中互相转移，因为尽管`std::thread`实例不可拷贝，但他们是*可移动的*。这保证了在任何时候一个`std::thread`实例只能关联一个特定的执行线程，同时允许程序员在对象间转移所有权。

### 转移线程所有权

​	假设要写一个函数，创建一个线程在后台运行，但是把新线程的所有权传回到调用函数而不是让函数等待它结束；或与之相反：创建一个线程，并且把所有权传递到某个函数，这个函数等待它结束。不管哪种情况，你都需要把所有权从一个地方转移到另一个地方。

```c++
void some_function();
void some_other_function();
std::thread t1(some_function);           
std::thread t2=std::move(t1);           
t1=std::thread(some_other_function);   
std::thread t3;                           
t3=std::move(t2);                       
t1=std::move(t3);//错误，不能通过赋予管理它的std::thread对象一个新值来简单丢弃这个线程。
```

- `std::thread`支持移动，就意味着线程的所有权可以在函数外进行转移

  ```c++
  //函数返回std::thread对象
  std::thread f(){
  	void some_function();
  	return std::thread(some_function);
  }
  //作为参数传入
  void f(std::thread t);
  void g(){
  	void some_function();
      std::thread t(some_function);
    	f(std::move(t));
  }
  ```

  - std::thread使用移动语义的另外一个好处就是可以创建thread_guard类

    ```c++
    class scoped_thread{
    	std::thread t;
    	scoped_thread(std::thread t_): t(std::move(t)){
    		if(!t.joinable()){
    			throw std::logic_error("No thread");
    		}
    	}
    	~scope_thread(){
    		t.join();
    	}
        scoped_thread(const scoped_thread &)=delete;
        scoped_thread& operator=(const scoped_thread&)=delete;
    }
    struct func;
    void f(){
        int some_local_state;
        scoped_thread t(std::thread(func(some_local_state)));
        do_something_in_current_thread();
    }
    ```

    当初始线程到达f函数末尾时，scoped_thread对象就会销毁，然后连接（等待）构造函数中提供的线程（执行完毕）。

    给C++17的一个提案就是添加一个joining_thread类，这个类型与`std::thread`类似；不同的是在析构函数中自动连接，类似于scoped_thread。

  

### 运行时选择线程数量

C++标准库中的`std::thread::hardware_concurrency()`可以表明执行程序真正可以并发的线程数．在一个多核系统中，返回值可以是CPU的核数。**返回值也仅仅是个提示**，当系统信息无法获取时，函数也可能返回０．

- 使用`std::thread::hardware_concurrency()`判断可用线程数实现并行算法



### 标识线程

线程标识类型为`std::thread::id`，并可以通过两种方式进行检索。

1. `std::thread`对象的成员函数`get_id()`来获取。

   如果`std::thread`对象没有与任何执行线程相关联，`get_id()`将返回默认构造的`std::thread::id`对象，表示“没有任何线程”("not any thread"，直接std::cout输出时会抛出std::system_error)。

2. 调用`std::this_thread::get_id()`也可以获得当前线程的标识，这个函数也定义在`<thread>`头文件中。

C++线程库不会限制你去检查线程标识是否一样，`std::thread::id`类型对象提供相当丰富的比较操作符，为所有不同的值提供了全序关系。这意味着可以将其当做容器的键，或者做排序，或以其他方式做比较，只要你认为合适就行。标准库也提供`std::hash<std::thread::id>`，所以`std::thread::id`也可以作为无序关联容器的键。

- `std::thread::id`实例常用作检测线程是否需要执行一些操作。例如，当用线程来划分工作.

  初始线程需要启动其他线程，因此可能要做一些与其他线程不同的工作。这种情况下，在启动其他线程前，它存储`std::this_thread::get_id()`返回的值，然后在算法的核心部分（对所有线程都一样）可以检查自己的线程ID和这个存储的值。

  ```c++
  std::thread::id master_thread;
  void some_core_part_of_algorithm(){
  	if(std::this_thread::get_id() == master_thread){
  		do_master_thread_work();
  	}
  	d0_common_work();
  }
  ```

- 或者，当前线程的`std::thread::id`可以**存储到一个数据结构中作为操作的一部分**。之后在这个数据结构上的操作可以检查存储的ID和执行这个操作的线程ID，从而决定哪些操作是允许/需要的。

- 同样，**线程ID可以用作关联容器的键，在容器中指定需要关联给某个线程的数据，而其他机制如线程局部存储并不适合。**例如，这样的一个容器可以用于控制线程存储在其控制下的每个线程的信息，或者在线程间传递信息。

# 第三章：线程间共享数据（1/2）

- 共享数据面临的问题
- 使用互斥锁保护数据
- 保护共享数据的替代设施

线程间共享数据的问题全都归咎于修改数据。如果共享数据是只读的，那就没有问题．

## **使用互斥锁保护共享数据**

将所有访问数据结构的代码片段都标记为互斥的（mutually exclusive），任何一个线程在执行其中一段时，其他线程试图访问共享数据的话，就必须进行等待，直到第一个线程操作结束。

- ***std::mutex***和***std::lock_guard***都在<mutex>头文件中声明。

- C++中通过构建一个***std::mutex***实例创建互斥锁，通过成员函数**lock()**对互斥锁上锁，**unlock()**进行解锁。

- **实践中不推荐直接去调用成员函数，因为这意味着，必须记住在每个函数出口都要去调用unlock()，也包括发生异常的情况。**

  - C++标准库为互斥锁提供了**一个RAII语法的std::lock_guard类模板**，在构造时锁住提供的互斥锁，并在析构的时候进行解锁，从而保证了一个锁住的互斥锁能被正确解锁。

    ```c++
    #include <list>
    #include <mutex>
    #include <algorithm>
    
    std::list<int> some_list;
    std::mutex　some_mutex;
    void add_to_list(int new_value){
        std::lock_guard<std::mutex> guard(some_mutex);
        some_list.push_back(new_value);
    }
    bool list_contains(int value_to_find){
        std::lock_guard<std::mutex> guard(some_mutex);
        return std::find(some_list.begin(),some_list.end(),value_to_find)
    }
    ```

    c++17添加了新特性，称为类模板参数推导，意味着像std::lock_guard这样的简单模板，其模板参数列表通常可以省略．

    ```c++
    std::lock_guard guard(some_mutex);
    ```

    c++17还有一种加强版锁保护机制***std::scoped_lock***

    ```c++
    std::scoped_lock guard(some_mutex);
    ```

  - 某些情况下使用全局变量没问题，但在**大多数情况下，互斥锁通常会与需要保护的数据放在同一类中**，而不是使用全局变量**。互斥锁和需要保护的数据，在类中都定义为private成员**，这可以更简单的识别哪些代码需要访问这个数据，因而哪些代码需要锁住互斥锁。**当所有成员函数在访问任务数据成员前锁住互斥锁，结束时解锁**，数据就被很好的保护起来免受所有外来者的干扰。

- **构建代码来保护数据**

  - 和确保成员函数不会传出指针或引用一样，检查它们不会把这些指针或者引用传给它们调用的函数同样重要，尤其这些函数不受你控制，这种将引用或指针作为参数传入或返回值会使锁失去了意义．

    ```c++
    class some_data{
        int a;
        std::string b;
        public:
        	void do_something();
    }
    class data_wrapper{
        private:
        	some_data data;
        	std::mutex m;
        public:
        	template typename<Function>
        	void process_data(Function func){
                std::lock_guard<std::mutex> l(m);
                func(data);	//而这个类外传入的对象可以访问内部数据
            }
    }
    void malicious_function(some_data& data){
    	//对data进行操作，此时没有锁的保护
    }
    
    ```

- **发现接口中固有的竞争条件**

  仅因为使用了互斥锁或其他机制保护了共享数据，并不意味着你不会受到竞争条件的影响

  ```c++
  stack<int> s;
  if(!s.empty()){						//1
      const int value = s.top();		//2
      s.pop();
      do_something(value)
  }
  ```

  如果在１和２之间有另一个线程使用s.pop()，那么top()将出现错误．使用互斥锁也不能阻止．

  此时需要保证一次只有一个线程能调用栈的成员函数．

  当**栈（而不是栈元素类型的数据）**被一个内部互斥锁所保护时，一次只有一个线程可以调用栈的成员函数，所以调用可以很好地交错执行，但是do_something()是可以并发运行的。

# 第三章：线程间共享数据（2/2）

### 阿斯顿

