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

  仅因为使用了互斥锁或其他机制保护了共享数据，并不意味着你不会受到竞争条件的影响：不仅仅在单个步骤需要保护数据，在整个数据结构和整个操作过程也需要保护。这是接口固有的竞争条件，因此在无锁实现中任然会出现竞争条件。

  ```c++
  //对于stack<int> s;
  if(!s.empty()){						//1
      const int value = s.top();		//2
      s.pop();						//3
      do_something(value)
  }
  ```

  1. 如果共享栈中只有一个元素，在1和2之间有另一个线程使用s.pop()，那么top()将出现错误．使用互斥锁也不能阻止．
  2. 如果两个线程交替运行上面代码，两次top都获取同一个元素，而两次pop弹出两个元素，其中一个将没有被获取到就直接弹出
  
  **这就要求接口需要进行变化，其中之一就是组合top()和pop()在一个互斥锁的保护下。**
  
  **但如果对象的拷贝构造函数在栈中会抛出一个异常，这种方式就会出现问题。**
  
  - 如果pop()返回栈顶值，那么在调用pop()时，我们使用临时量存储栈顶，删除栈顶，将临时量返回，调用拷贝构造函数返回给函数外的对象。
  - 如果拷贝数据返回给调用者的时候出现异常，要弹出的这个数据就丢失了，但它已经从栈上移除了。因此stack接口的设计人员将这个操作分为两个部分：先top，再pop(且top无返回值)
  
  **但我们在消除竞争条件时，就是为了试图避免这种分割而将两者组合在一个互斥锁之下。**
  
  1. 传入一个引用
  
     ```c++
     std::vector<int> result;
     some_stack.pop(result);
     ```
  
     这种方法提前构造好元素（占用空间，避免空间不足而拷贝构造函数失败）。但有一个明显的劣势：需**要提前构造出栈中值的类型的实例用于接收目标值**。并且**要求存储的类型必须时可赋值的**，从而在pop函数内对其赋值。
  
  2. 要求一个无异常抛出的拷贝构造或移动构造函数
  
     对于有返回值的pop()函数来说，只有异常安全方面的问题，如果值返回可以抛出一个异常的话。一个方法是限制线程安全的栈只用在能够安全按值返回并且不会抛异常的类型上。尽管在编译时能够使用`std::is_nothrow_copy_constructible`和`std::is_nothrow_move_constructible`，但有较大局限性
  
  3. 返回指向弹出值的指针
  
     第三个选项是返回一个指向弹出项的指针,而不是按值返回项，这样你就能避免 拷贝构造异常的问题了。**缺点就是返回一个指针需要动态分配给对象的内存（否则离开作用于即销毁）**，对于选择这个方案的接口,使用 std::shared_ptr 不仅能避免内存泄露,因为一旦最后一个指针销毁了,对象也会销毁,而且不需要 new 和 delete 操作。从优化的目的看这是很重要的:需要栈中的每个对象分别用 new 来分配,相比原始的非线程安全版本强加了太多开销。
  
  4. “1+2”或“1+3‘’
  
     **线程安全栈示例**

以上都是只加一个锁，而对于必须锁住两个或更多互斥锁，就要可能出现死锁。

# 第三章：线程间共享数据（2/2）

## 死锁：问题和解决方案

**避免死锁的一般建议，就是让两个互斥锁总是相同的顺序上锁。**

- 比如完成一件事需要得到资源A和资源B，当线程a取得资源A，线程b取得资源B则发生死锁。如果规定线程获取资源时必须先得到A再得到B，那就不存在线程b会取得资源B，从而不会发生死锁。

- 一个对同一类别的两个不同示例的交换操作，为了保证数据被正确交换，就要避免数据被并发的修改，每个实例的互斥锁都必须锁住。`void swap(Object& lhs,Object& rhs)`

  但是选择一个固定顺序时，例如先对第一个参数上锁，再对第二个参数上锁会出现问题。

  `ThreadA call swap(ObjA,ObjB)`

  `ThreadB call swap(ObjB,ObjB)`

  两个线程调用swap函数时，内部函数如果交替执行，那么仍然会出现死锁，因此加锁的过程也必须组合。

  - c++标准库：`std::lock` 函数可以一次性锁住两个或者更多的互斥锁，并且没有死锁的风险，但是由于q`std::lock()`是函数，仍要自己解锁，可利用`std::lock_guard`

    ```c++
    class Object;
    void swap(Object& lhs,Object& rhs);
    class X{
        private:
        	Object obj_data;
        	std::mutex m;
        public:
        	X(const Object& obj):obj_data(obj){};
        	friend void swap(X& lhs,X& rhs){
                if(&lhs == &rhs)
                    return;
                std::lock(lhs.m,rhs.m);	//1
                std::lock_guard<std::mutex> lock_a(lhs.m,std::adopt_lock);
                std::lock_guard<std::mutex> lock_b(rhs.m,std::adopt_lock);
                swap(lhs.obj_data,rhs.obj_data);
            }
    };
    ```

    - if先判断是否是不同的实例，如果是自身实例，将会对自身的mutex加锁两次而造成未定位行为。而lock_guard的`std::adopt_lock`参数高手`std::lock_guard`互斥锁已经上锁，仅结果互斥锁的所有权即可，不用在构造函数中上锁。
    - 当`std::lock`成功的获取了一个互斥锁上的锁，并且当其尝试从另一个互斥锁上再获取锁而有异常抛出时，第一个锁会自动释放，即`std::lock`有要么全做要么不做的语义。s

  - 由于使用`std::lock`需要将锁的持有权转移到`std::lock_guard`上以便在退出时释放互斥锁，c++17提供了`std::scoped_lock<>`一种新的RAII模板，其接受一个互斥锁类型的列表作为可变模板参数，在其构造函数中使用同`std::lock`相同的算法对所有互斥锁加锁，在析构函数中全部解锁。

    ```c++
    void swap(X& lhs, X& rhs)
    {
    	if(&lhs==&rhs)
    	return;
    	std::scoped_lock guard(lhs.m,rhs.m); 
        //std::scoped_lock<std::mutex,std::mutex> guard(lhs.m,rhs.m);
    	swap(lhs.some_detail,rhs.some_detail);
    }
    ```

    

