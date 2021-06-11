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


## 避免死锁的更多指南

​	死锁不仅仅发生在锁上，无锁情况下，仅需要两个线程`std::thread`对象互相调用join()，就能产生死锁。

- **避免死锁的的指南全部可以归结为一个理念：不要等待另一个线程，如果它有可能等待你的话。**

1. 避免嵌套锁

   如果已经持有一把锁的话不要再去获取一把，如果坚持这条原则，就不可能**仅从锁的用法**上造成死锁，因为每个线程只持有一把锁。当需要获取多个锁，应该使用`std::lock`同时上锁来避免死锁。

2. 避免在持有锁时调用用户的代码

   因为用户的代码可能还会有其他加锁操作或者重复获取同一个锁造成嵌套锁，参照第一条

3. 使用固定顺序获取锁

   当需要获取两个或两个以上的锁，并且不能使用`std::lock`来获取它们时，最好的方法是在每个线程上，用固定的顺序获取它们。

   但例如交换锁的操作即使使用固定顺序，但传入参数的位置却可以交换，应该注意。

   - 遍历链表的过程中，我们需要判断元素是否会被修改，链表的值和指向都可能会被修改，因此我们对当前元素上锁，遍历时，对下一个节点上锁，然后再对本元素解锁从而保证安全，这种交叉锁的风格允许多个线程访问链表。

     - 对于单项链表，线程A对元素a加锁开始遍历，线程B对元素b加锁进行修改，将会使线程A的遍历结果发生改变。
     - 对于双向链表而言，可能有一线程B反向遍历，线程B对元素b加锁而想获得下一个元素a而线程A正向遍历对元素a加锁,并想获得下一个元素b的锁，而导致死锁。

     因此一种避免死锁的方法就是定义遍历的顺序，一个线程必须先锁住元素a的锁才能对下一个元素b进行加锁。

     **利用锁的层次结构可以定义锁的顺序**，当代码试图对一个互斥锁上锁,在该锁已被低层持有时,上锁是不允许的。可以在运行时检查这个问题,方法是为每个互斥对象分配层编号,并记录每个线程锁定了哪些互斥锁。这是一种常见的模式,但是 C++标准库没有提供对它的直接支持,因此你需要编写自定义的`hierarchical_mutex`互斥锁类型

     ```c++
     //简单的层级互斥锁实现
     class hierarchical_mutex{
         std::mutex internal_mutex;
         unsigned long const hierarchy_value;
     	unsigned long previous_hierarchy_value;
         static thread_local unsigned long this_thread_hierarchy_value;
         void check_for_hierarchy_violation(){
             if(this_thread_hierarchy_value <= hierarchy_value){ // 2
         		throw std::logic_error(“mutex hierarchy violated”);
         	}
         }
         void update_hierarchy_value(){
     		previous_hierarchy_value=this_thread_hierarchy_value; // 3
     		this_thread_hierarchy_value=hierarchy_value;
     	}
         public:
     	explicit hierarchical_mutex(unsigned long value):hierarchy_value(value),
         previous_hierarchy_value(0){}
         
         void lock(){
             check_for_hierarchy_violation();
             internal_mutex.lock(); // 4
             update_hierarchy_value(); // 5
         }
         void unlock(){
             if(this_thread_hierarchy_value!=hierarchy_value)
             throw std::logic_error(“mutex hierarchy violated”);
             this_thread_hierarchy_value=previous_hierarchy_value;
             internal_mutex.unlock();
     	}
         bool try_lock(){
             check_for_hierarchy_violation();
             if(!internal_mutex.try_lock()) // 7
             return false;
             update_hierarchy_value();
             return true;
     	}
         thread_local unsigned long hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);// 8
     }
     ```

     `hierarchical_mutex `的实现使用了 thread_local 的变量来存储当前线程的层级值。这
     个值可以被所有的互斥实例访问,但每个线程都有一个不同的值。因为其声明中有 thread_local,所以每个线程都有其自己的副本,因此,一个线程中变量的状态完全独立于从另一个线程读取到的变量的状态

**灵活的 std::unique_lock 锁**

一旦你设计了避免死锁的代码, std::lock()和 std:: lock_guard 就可以处理大多数简单锁
的情况,但是有时需要更多的灵活性。

- 在这种情况下,标准库提供了 std::unique_lock 模板。与 std::lock_guard 一样,这是一 个互斥锁类型参 数化的类模板，一个`std::unique_lock`并不总是拥有与其关联的互斥锁。

- 正如你可以将 `std::adopt_lock `作 为 第二个参数传递给std::lock_guard的构造函数一样，也可以将`std::defer_lock`作为第二个参数给构造函数，表明互斥锁在构造函数应该保持解锁状态。

- `std::unique_lock`会占用比较多的空间，并且比std::lock_guard稍慢一些，因为`std::unique_lock`需要维护是否拥有当前互斥锁的标志。这个标志是为了确保 unlock()在析构函数中被正确调用。如果实例拥有互斥锁,那么析构函数必须调用 unlock();但当实例中不拥有互斥锁时,析构函数就不能去调用 unlock()。这个标志可以通过 `owns_lock()`成员函数进行查询。

- 因为 std::unique_lock 提供了 lock(), try_lock()和 unlock()成员函数,所以能将 std::unique_lock 对象传递到 std::lock()

  ```c++
  //unqiue_lock版本的swap函数
  class some_big_object;
  void swap(some_big_object& lhs,some_big_object& rhs);
  class X
  {
  	private:
  		some_big_object some_detail;
  		std::mutex m;
  	public:
  		X(some_big_object const& sd):some_detail(sd){}
  		friend void swap(X& lhs, X& rhs){
          if(&lhs==&rhs) return;
          std::unique_lock<std::mutex> lock_a(lhs.m,std::defer_lock); // 1
          std::unique_lock<std::mutex> lock_b(rhs.m,std::defer_lock); // 1 
          std::lock(lock_a,lock_b); // 2 互斥锁在这里上锁
          swap(lhs.some_detail,rhs.some_detail);
         }
  };
  ```

- `std::unique_lock `实例不一定要拥有与之相关的互斥锁,一个互斥锁的所有权可以通过**移动操作**,在不同的实例中进行转移。**当源值是一个右值,所有权转移是自动的**,为了避免意外从一个变量转移所有权,对左值就必
  须显式完成，如std::move()。

  - 一种可能的用法是**允许函数锁住互斥锁并将该锁的所有权转移给调用者,这样调用者就**
    **可以在同一锁的保护下执行其他操作**。

- `std::unique_lock `实例被销毁之前释放锁的能力意味着,如果明显不再需要该锁,你可以
  在特定的代码分支中**选择性地释放它**。这对于应用程序的性能来说很重要：持有锁的时间超
  过所需时间可能会导致性能下降

**在适当的粒度上锁**

锁粒度是描述单个锁保护的数据量的术语。不仅选择足够粗的锁粒度以确保所需数据得到
保护很重要,而且确保锁**只用于需要它的操作**也是很重要的。

## 保护共享数据的替代设施

- 对于共享数据是**只在初始化才需要并发控制，但之后就不需要同步**的数据，如一些只读数据。

  - C++标准提供了一种机制,纯粹用于**在初始化期间保护共享数据。**

  - 假设你有一个共享资源,构建代价很昂贵,你只想在确实需要的时候才构建它（**延迟初始化**）;它可
    能会打开一个数据库连接或分配出很多的内存。

    - 通常的做法：

      ```c++
      //清单 3.11 使用互斥锁的线程安全的延迟初始化
      std::shared_ptr<some_resource> resource_ptr;
      std::mutex resource_mutex;
      void foo(){
      	std::unique_lock<std::mutex> lk(resource_mutex); // 所有线程在此串行化
      	if(!resource_ptr){
      		resource_ptr.reset(new some_resource); // 只有初始化过程需要保护
      	}
      	lk.unlock();
      	resource_ptr->do_something();
      }
      ```

      这段代码很常见,但会出现不必要的串行化问题，每次都需要加锁判断,许多人试图想出一个更好的办法来做这件事,包括声名狼藉的“双重检查加锁”模式。

    - “双重检查加锁”模式：

      ```c++
      void undefined_behaviour_with_double_checked_locking(){
          if(!resource_ptr){	// 1
              std::lock_guard<std::mutex> lk(resource_mutex);
              if(!resource_ptr){ // 2
                  resource_ptr.reset(new some_resource); // 3
              }
          }
          resource_ptr->do_something(); // 4
      }
      ```

      即使一个线程看到了另一个线程写的指针,它也可能看不到新创建的 some_resource 实例,从而导致调用 do_something() //4操作不正确的值。这是竞争条件类型的一个示例,C++标准将其定义为数据竞争(data race) ,并且规定为未定义的行为。

  - C++标准委员会也认为这是一个重要的场景,因此 C++标准库提供了 std::once_flag
    和 std::call_once 来处理这种情况。

    不同于锁住互斥锁并显式地检查指针,每个线程都可以使用 `std::call_once`,因为知道当 `std::call_once `返回时,指针已经被某个线程(以适当同步的方式)初始化了,所以是安全的。

    ```c++
    //清单 3.12 使用 std::call_once 作为类成员的线程安全的延迟初始化
    class X
    {
        private:
            connection_info connection_details;
            connection_handle connection;
            std::once_flag connection_init_flag;
        	void open_connection(){
            	connection=connection_manager.open(connection_details);
            }
    	public:
            X(connection_info const& connection_details_):
    			connection_details(connection_details_){}
    		void send_data(data_packet const& data){ 	// 1
    			std::call_once(connection_init_flag,&X::open_connection,this); // 2
    			connection.send_data(data);
    		}
            data_packet receive_data(){ // 3
                std::call_once(connection_init_flag,&X::open_connection,this); // 2
                return connection.receive_data();
            }
    };
    ```

    - 还有一种初始化过程中潜存着条件竞争:其中一个局部变量被声明为 static 类型。初始化只会在一个线程中发生,并且没有其他线程可在初始化完成前继续执行,

      ```c++
      class my_class;
      my_class& get_my_class_instance(){
      	static my_class instance;	// 初始化保证是线程安全的
      	return instance;
      }
      ```

- 仅在初始化时保护数据是更一般场景的一种特殊情况:**很少更新的数据结构**

  虽然更新频度很低,但更新也有可能发生，并且当这个缓存可被多个线程访问时，它需要在更新期间得到适当的保护。使用 std::mutex 来保护数据结构，显得用力过猛,因为在没有发生修改时，它将削减并发读取数据的可能性。

  - 需要另一种不同的互斥锁，这种互斥锁常被称为“读者-写者”锁(读写锁)，因为它允许两种不同的用法:一个“写者”线程独占访问，让多个“读者”线程并发访问。
  - C++ 17 标准库提供了两个现成的互斥锁` std::shared_mutex `和`std::shared_timed_mutex`。C++14 只提供了 `std::shared_timed_mutex`但是在 C++11 中什么都没有。`std::shared_mutex` 和 `std::shared_timed_mutex `的不同点在于,`std::shared_timed_mutex `支持更多的操作方式
  - 那些不需要更新数据结构的线程可以使用 `std::shared_lock<std::shared_mutex>`来获得共享访问。这个 RAII 类模板是在 C++14 中添加的,使用方法与 std::unique_lock 相同，只是多个线程可以在同一个 std::shared_mutex 上同时拥有一个共享锁。
    - 如果任何线程持有一个共享锁，试图获取独占锁的线程将阻塞，直到所有其他线程让出
      它们的锁，同样，如果任何线程拥有互斥锁，没有其他线程可以获得一个共享或互斥锁直
      到第一个线程让出它的锁。

**递归锁**

对于 std::mutex,一个线程试图锁住它已经拥有的互斥锁是个错误,并且尝试这样做会导致未定义的行为。

- C++标 准 库 提 供 了` std::recursive_mutex` 。 它 的 工 作 原 理 类 似于`std::mutex`，不同之处在于你可以从同一个线程获取单个实例上的多个锁。
- 递归互斥锁的一种常见用法是,类被设计为可以从多个线程并发地访问,因此它有一个互斥锁来保护成员数据。
  - 一个公共成员函数调用另一个成员函数作为其操作的一部分是可取的。第二个成员函数也将尝试锁住互斥锁,从而导致未定义的行为。一种应急的解决方案是将互斥锁更改为递归互斥锁。这将允许第二个成员函数中的互斥锁成功上锁,并继续执行。
  - 通常更好的方法是从两个成员函数中提取一个新的私有成员函数,它不会锁住互斥锁(它期望它已经被锁住)。

# 第四章：同步并发操作

​	第三章讲述在线程间保护共享数据安全的方法，第四章讲述提高线程并发效率的方法。

## 等待一个事件或其他条件

如果线程A正在等待另一个线程B完成一个任务，有以下主要方法：

1. 线程A可以不断检查该任务完成的标志，线程B完成任务后设置该标志，但线程A会一直在询问该标志是否完成，持续占用CPU时间

2. 让线程A采用间断式的检查任务是否完成，但时间间隔难以把控，过长或过短都会造成资源浪费或闲置。

   - 如在线程A检查后进入一定时间的休眠`std::this_thread::sleep_for()`函数休眠。

     ```
     bool flag;	//任务完成的标志
     std::mutex m;	//检查时需要对该标志加锁
     void wait_for_flag(){
     	std::unique_lock<std::mutex> lk(m);
     	while(!flag){
     		lk.lock();
     		std::this_thread::sleep_for(std::chrono::milliseconds(100));
     		lk.unlock();
     	}
     }
     ```

3. 线程A一直等待，直到线程B发送任务完成的信息再唤醒。

   - 使用条件变量，`std::condition_variable`和`std::condition_variable_any`这两个时间都包含在`<condition_variable>`库头文件中。**两者都需要一个互斥锁一起才能工作，区别在于前者仅限使用std::mutex，或者可以使用任何类似互斥锁最低标准的对象**。

     ```c++
     std::mutex mut;
     std::queue<data_chunk> data_queue;	// 1
     std::condition_variable data_cond;
     void data_preparation_thread()	{
     	while(more_data_to_prepare()){
             data_chunk const data=prepare_data();
             {
                 std::lock_guard<std::mutex> lk(mut);
                 data_queue.push(data); // 2
             }
             data_cond.notify_one(); // 3
         }
     }
     void data_processing_thread(){
     	while(true){
             std::unique_lock<std::mutex> lk(mut); // 4
             data_cond.wait(lk,[]{return !data_queue.empty();}); // 5
             data_chunk data=data_queue.front();
             data_queue.pop();
             lk.unlock(); // 6
             process(data);
             if(is_last_chunk(data))	break;
         }
     }
     ```

     线程A,B通过队列传递数据，当前数据准备好之后，准备的线程使用`std::lock_guard`来保护数据，并将其推入队列，然后调用notify_one()通知线程B；线程B调用wait()后一直等待队列中有数据可以取出，收到通知信息后，加锁，检查条件，若条件完成则返回并继续持有锁，若条件不满足则返回并解锁并继续等待。

     - 注意到，线程A使用的时lock_guard，并且在一个小型作用域中可以控制互斥锁；而线程B使用unique_lock，可以灵活的使用加锁解锁。
     - 当等待的线程重新获得互斥锁并检查条件时，如果它不是直接响应来自另一个线程的通知，则称为伪唤醒。所以不建议使用具有较大副作用的函数作为条件检查，否则必须为副作用发生多次做准备。

   ### 使用条件变量构建线程安全队列

   ```c++
   #include <memory>
   template<typename T>
   class threadsafe_queue{
       public:
       	threadsafe_queue();
       	threadsafe_queue(const threadsafe_queue&);
       	threadsafe_queue& operator=(const threadsafe_queue&) = delete;
       	void push(T new_value){
               std::lock_guard<std::mutex> lk(mut);
               data_queue.push(new_value);
               data_cond.notift_once();
           }
       	void wait_and_pop(T& value){
               std::unique_lock<std::mutex> lk(mut);
               data_cond.wait(lk,[this]{return !data_queue.empty();});
               value = data_queue.front();
               data_queue.pop();
           }
       
       	std::shared_ptr<T> wait_and_pop(){
   			std::unique_lock<std::mutex> lk(mut);
               data_cond.wait(lk,[this]{return !data_queue.empty();});
               std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
               data_queue.pop();
               return res;
           }
       
       	bool try_pop(T& value){
               std::lock_guard<std::mutex> lk(mut);
               if(data_queue.empty()){
   				return false;
               }
               value = data_queue.front();
               data_queue.pop();
               return true;
           }
       	
       	std::shared_ptr<T> try_pop(){
   			std::lock_guard<std::mutex> lk(mut);
               if(data_queue.empty()){
                   return shared_ptr<T>();
               }
               std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
               data_queue.pop();
               return res;
           }
       
       	bool empty() const{
               std::lock_guard<std::mutex> lk(mut);
               return data_queue.empty();
           }
       	
   };
   ```

   
