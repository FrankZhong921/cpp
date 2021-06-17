# Hello,World!

```go
//第一个go程序
package main
import "fmt"
func main(){
	fmt.Println("Hello,World!") //没有分号
}
```

```shell
go run hello.go		//执行go语言代码
go build hello.go	//生成二进制目标文件, ./hello 执行
```

**每个Go程序都是由包构成，程序从main包开始运行。**

**按照约定，包名与导入路径的最后一个元素相同**

# 导入导出

```go
//多个包可以使用分组形式来导入
import (
	"fmt"
    "math"
)
```

在 Go 中，如果一个名字以大写字母开头，那么它就是已导出的。例如，`Pizza` 就是个已导出名，`Pi` 也同样，它导出自 `math` 包。在导入一个包时，你只能引用其中已导出的名字。任何“未导出”的名字在该包外均无法访问。

`cannot refer to unexported name math.pi`



# 语言基础

- 推荐一个语句一行，从而无需写行分隔符，若多语句一行需要人工添加行分隔符
- ![](C:\Users\921-\AppData\Roaming\Typora\typora-user-images\image-20210511220225795.png)

- 数据类型

  1. 布尔型：只可以是true或者是false。例：`var b bool = true`

  2. 数字类型

     | uint8  | float32               | byte   类似uint8                 |
     | ------ | --------------------- | -------------------------------- |
     | uint16 | float64               | rune   类似int32                 |
     | uint32 | complex64   复数类型  | uint     32或64位                |
     | uint64 | complex126   复数类型 | int        与uint一样            |
     | int8   |                       | uintptr 无符号整数类型用于存指针 |
     | int16  |                       |                                  |
     | int32  |                       |                                  |
     | int64  |                       |                                  |

     `int`, `uint` 和 `uintptr` 在 32 位系统上通常为 32 位宽，在 64 位系统上则为 64 位宽。 当你需要一个整数值时应使用 `int` 类型，除非你有特殊的理由使用固定大小或无符号的整数类型。

# 语言变量

## 变量声明

变量声明使用 ***var***  关键字

```go
var identifier type					//单变量声明
var identifire1, identifier2 type	//多变量声明
```

1. 指定变量类型，但没初始化，则变量为系统默认值（零值）

   - **数值类型为0，布尔为false,字符串为“”**

   - 以下几种类型默认为 **nil**

     ```go
     var a *int
     var a []int
     var a map[string] int
     var a chan int
     var a func(string) int
     var a error // error 是接口
     ```

2. 根据值自行判断变量类型

   ```go
   var v_name = value
   ```

3. **省略 var ，注意 :=  是声明语句，若该变量已经声明就产生编译错误**

   **在函数内，可用:=在类型明确的地方代替 `var` 声明，在函数外必须以关键字开头**

   ```go
   v_name := value
   ```

   不过当右边包含未指明类型的数值常量时，新变量的类型就可能是 `int`, `float64` 或 `complex128` 了，这取决于常量的精度

4. 多变量声明

   ```
   //类型相同多个变量, 非全局变量
   var vname1, vname2, vname3 type
   vname1, vname2, vname3 = v1, v2, v3
   
   var vname1, vname2, vname3 = v1, v2, v3 // 和 python 很像,不需要显示声明类型，自动推断
   
   vname1, vname2, vname3 := v1, v2, v3 // 出现在 := 左侧的变量不应该是已经被声明过的，否则会导致编译错误
   
   
   // 这种因式分解关键字的写法一般用于声明全局变量
   var (
       vname1 v_type1
       vname2 v_type2
   )
   ```


- **为什么go的变量声明与C不同?**

  - 从该形式触发

    ```go
    x: int
    p: pointer to int
    a: array[3] of int
    ```

    go做了一些修改

    ```go
    x int
    p *int
    a [3]int
    ```

    使用从左到右的声明在面对较复杂类型可读性较强，并且当声明时即使去掉变量名也由较好的可读性。

    但是 * 号是个例外，由于继承了c语言的*，而不是Pascal的 ^，导致增加了一些不一致的语法

    ```go
    var p *int
    x = *p
    //而不是 x = p*
    //对于Pascal
    var p ^int
    x = p^
    ```



- 函数可以返回任意数量的返回值

  ```go
  func swap(x , y string ) (string , string){
  	return y , x
  }
  ```

- Go 的返回值可被命名，它们会被视作**定义在函数顶部的变量**。

  没有参数的 `return` 语句返回已命名的返回值。也就是 `直接` 返回。

  ```go
  func split(sum int) (x, y int) {
  	x = sum * 4 / 9
  	y = sum - x
  	return
  }
  
  func main() {
  	fmt.Println(split(17))
  }
  
  ```

  ## 类型转换

  与 C 不同的是，Go 在不同类型的项之间赋值时需要显式转换。

  ```go
  var i int = 42
  var f float64 = float64(i)
  var u uint = uint(f)
  ```

  ## 常量

  常量的声明与变量类似，只不过是使用 `const` 关键字。

  常量可以是字符、字符串、布尔值或数值。

  常量不能用 `:=` 语法声明。

  ` const Pi = 3.14`



# 流程控制语句

## 循环控制

Go 只有一种循环结构：`for` 循环。

基本的 `for` 循环由三部分组成，它们用分号隔开：

- 初始化语句：在第一次迭代前执行
- 条件表达式：在每次迭代前求值
- 后置语句：在每次迭代的结尾执行

初始化语句通常为一句短变量声明，该变量声明仅在 `for` 语句的作用域中可见。

一旦条件表达式的布尔值为 `false`，循环迭代就会终止。

**注意**：和 C、Java、JavaScript 之类的语言不同，**Go 的 for 语句后面的三个构成部分外没有小括号， 大括号 `{ }` 则是必须的。**

```go
func main() {
	sum := 0
	for i := 0; i < 10; i++ {
		sum += i
	}
	fmt.Println(sum)
}
```

**初始化语句和后置语句是可选的。**此时可以去掉条件表示式的分号，等价C语言的while

```go
func main() {
	sum := 1
	for sum < 1000 {
		sum += sum
	}
	fmt.Println(sum)
}

```

**如果省略循环条件，该循环就不会结束，因此无限循环可以写得很紧凑**

```go
func main(){
	for{
	}
}
```

## 条件控制

### if

Go 的 `if` 语句与 `for` 循环类似，表达式外无需小括号 `( )` ，而大括号 `{ }` 则是必须的。

```
func sqrt(x float64) string {
	if x < 0 {
		return sqrt(-x) + "i"
	}
	return fmt.Sprint(math.Sqrt(x))
}
```

同 `for` 一样， `if` 语句可以在条件表达式前执行一个简单的语句。

该语句声明的变量作用域仅在 `if` 之内。在 `if` 的简短语句中声明的变量同样可以在任何对应的 `else` 块中使用。

- 这种写法使得无需像C语言一样先声明定义一个变量，再将该变量作为if的条件。

  c++17也支持使用这种语法

```go
func pow(x, n, lim float64) float64 {
	if v := math.Pow(x, n); v < lim {
		return v
	}
	return lim
}
```

### switch

`switch` 是编写一连串 `if - else` 语句的简便方法。它运行第一个值等于条件表达式的 case 语句。

Go 的 switch 语句类似于 C、C++、Java、JavaScript 和 PHP 中的，不过 Go 只运行选定的 case，而非之后所有的 case。 

- 实际上，Go 自动提供了在这些语言中每个 case 后面所需的 `break` 语句。 
- 除非以 `fallthrough` 语句结束，否则分支会自动终止。 
- Go 的另一点重要的不同在于 switch 的 case 无需为常量，且取值不必为整数。

- 没有条件的 switch 同 `switch true` 一样。

  ```go
  func main() {
  	t := time.Now()
  	switch {
  	case t.Hour() < 12:
  		fmt.Println("Good morning!")
  	case t.Hour() < 17:
  		fmt.Println("Good afternoon.")
  	default:
  		fmt.Println("Good evening.")
  	}
  }
  
  ```

## defer

defer 语句会将函数推迟到外层函数返回之后执行。

**推迟调用的函数其参数会立即求值**，但直到外层函数返回前该函数都不会被调用。

推迟的函数调用会被压入一个栈 ***defer 栈***中。当外层函数返回时，被推迟的函数会按照后进先出的顺序调用。

# 指针

类型 `*T` 是指向 `T` 类型值的指针。其零值为 `nil`。

- 注意：是`*T`而不是`T*`

`&` 操作符会生成一个指向其操作数的指针。`*` 操作符表示指针指向的底层值。

- 与 C 不同，**Go 没有指针运算。**

```go
func main(){
    i , j:= 1, 2
    p := &i
    *p = 3
    fmt.Println(i)
    
}
```

# 结构体

一个结构体（`struct`）就是一组字段（field）。结构体字段使用点号来访问。

结构体字段可以通过结构体指针来访问`(*p).X`。所以语言**也允许我们使用隐式间接引用**，直接写 `p.X` 就可以

```go
type Vertex struct {
	X int
	Y int
}

func main() {
	fmt.Println(Vertex{1, 2})
    
    var tmp = Vertex{1,2} //而不是 struct tmp Vertex = {1,2}
	fmt.Println(tmp.X)
    p := &tmp;
    fmt.Println(p.X)
    fmt.Println((*p).X)
}

```

结构体文法通过直接列出字段的值来新分配一个结构体。使用 `Name:` 语法可以仅列出部分字段。（字段名的顺序无关。）特殊的前缀 `&` 返回一个指向结构体的指针。

```go
var (
	v1 = Vertex{1, 2}  // 创建一个 Vertex 类型的结构体
	v2 = Vertex{X: 1}  // Y:0 被隐式地赋予
	v3 = Vertex{}      // X:0 Y:0
	p  = &Vertex{1, 2} // 创建一个 *Vertex 类型的结构体（指针）
)

func main() {
	fmt.Println(v1, p, v2, v3)
}
```

# 数组

类型 `[n]T` 表示拥有 `n` 个 `T` 类型的值的数组。

表达式

```
var a [10]int
```

会将变量 `a` 声明为拥有 10 个整数的数组。

**数组的长度是其类型的一部分，因此数组不能改变大小**。这看起来是个限制，**不过没关系，Go 提供了更加便利的方式来使用数组。**

```go
func main() {
	var a [2]string
	a[0] = "Hello"
	a[1] = "World"
	fmt.Println(a[0], a[1])
	fmt.Println(a)

	primes := [6]int{2, 3, 5, 7, 11, 13}	//使用这种方法就不用提前确定好大小
	fmt.Println(primes)
}
```

## 切片

- 每个数组的大小都是固定的。而切片则为数组元素提供动态大小的、灵活的视角。在实践中，切片比数组更常用。类型 `[]T` 表示一个元素类型为 `T` 的切片。

- 切片通过两个下标来界定，即一个上界和一个下界，二者以冒号分隔：

```
a[low : high]
```

- 它会选择一个**半开区间**，包括第一个元素，但排除最后一个元素。

  以下表达式创建了一个切片，它包含 `a` 中下标从 1 到 3 的元素（小标仍然是从0开始）：

```
a[1:4]
```

- 切片并不存储任何数据，它只是描述了底层数组中的一段。

  **更改切片的元素会修改其底层数组中对应的元素。**

  **与它共享底层数组的切片都会观测到这些修改。**

```go
func main() {
	name := [6]int{1,2,3,4,5,6}
	a := name[0:3]
	b := name[2:4]
    a[2] = 7	//没有 a ={6,7,8 }的语法
	fmt.Println(name)
	fmt.Println(a)
	fmt.Println(b)
}
```

- 切片文法类似于没有长度的数组文法。

```go
[3]bool{true, true, false}
//下面这样则会创建一个和上面相同的数组，然后构建一个引用了它的切片：
[]bool{true, true, false}
```

```go
func main() {
	q := []int{2, 3, 5, 7, 11, 13}
	fmt.Println(q)

	r := []bool{true, false, true, true, false, true}
	fmt.Println(r)

	s := []struct {
		i int
		b bool
	}{
		{2, true},
		{3, false},
		{5, true},
		{7, true},
		{11, false},
		{13, true},
	}
	fmt.Println(s)
}
```

- 在进行切片时，你可以利用它的默认行为来忽略上下界。

  切片下界的默认值为 `0`，**上界则是该切片的长度。**

```go
//对于数组
var a [10]int
//以下切片等价
a[0:10]
a[:10]
a[0:]
a[:]
```

- ​	切片拥有 **长度** 和 **容量**。

  - 切片的长度就是它所包含的元素个数。`len(s)`

  - 切片的容量是从它的第一个元素开始数，到其底层数组元素末尾的个数。` cap(s)`

  - 可以通过重新切片来扩展一个切片，给它提供足够的容量。

    ```go
    func main() {
    	s := []int{2, 3, 5, 7, 11, 13}
    	printSlice(s)
    
    	// 截取切片使其长度为 0
    	s = s[:0]
    	printSlice(s)	//len=0 cap=6 []
    
    	// 拓展其长度
    	s = s[:4]
    	printSlice(s)	//len=4 cap=6 [2 3 5 7]
    
    	// 舍弃前两个值
    	s = s[2:]
    	printSlice(s)	//len=2 cap=4 [5 7]
    }
    
    ```

- 切片的零值是 `nil`。**nil 切片的长度和容量为 0 且没有底层数组。**

### 用make创建切片

切片可以用内建函数 `make` 来创建，这也是你**创建动态数组的方式。**

`make` 函数会分配一个元素为零值的数组并返回一个引用了它的切片：

```
a := make([]int, 5)  // len(a)=5
```

要指定它的容量，需向 `make` 传入第三个参数：

```
b := make([]int, 0, 5) // len(b)=0, cap(b)=5

b = b[:cap(b)] // len(b)=5, cap(b)=5
b = b[1:]      // len(b)=4, cap(b)=4
```

切片操作并不复制切片指向的元素。它创建一个新的切片并复用原来切片的底层数组。

### 切片的切片

切片可包含任何类型，甚至包括其它的切片。

```go
board := [][]string{
		[]string{"_", "_", "_"},
		[]string{"_", "_", "_"},
		[]string{"_", "_", "_"},
	}
```

### 向切片追加元素

为切片追加新的元素是种常用的操作，为此 Go 提供了内建的 `append` 函数。

```
func append(s []T, vs ...T) []T
```

`append` 的第一个参数 `s` 是一个元素类型为 `T` 的切片，其余类型为 `T` 的值将会追加到该切片的末尾。

`append` 的结果是一个包含原切片所有元素加上新添加元素的切片。

当 `s` 的底层数组太小，不足以容纳所有给定的值时，它就会分配一个更大的数组。返回的切片会指向这个新分配的数组。

## Range

`for` 循环的 `range` 形式可遍历切片或映射。

当使用 `for` 循环遍历切片时，每次迭代都会返回两个值。**第一个值为当前元素的下标，第二个值为该下标所对应元素的一份副本。**

```go
func main() {
	for i, v := range pow {
		fmt.Printf("2**%d = %d\n", i, v)
	}
}
```

可以将下标或值赋予 `_` 来忽略它。

```
for i, _ := range pow
for _, value := range pow
```

若你只需要索引，忽略第二个变量即可。

```go
for i := range pow
```

```go
func main() {
	pow := make([]int, 10)
	for i := range pow {
		pow[i] = 1 << uint(i) // == 2**i
	}
	for _, value := range pow {
		fmt.Printf("%d\n", value)
	}
}

```

### 二维切片

```go
	arr :=make([][]uint8,dy)
	for i :=0; i< dx ; i++{
		line := make([]uint8,dx)
		for j :=0; j< dy;j++{
			line[j]= uint8(i*j)
		}
		arr[i] = line
	}
	return arr
```

# 映射

映射将键映射到值。

映射的零值为 `nil` 。`nil` 映射既没有键，也不能添加键。

`make` 函数会返回给定类型的映射，并将其初始化备用。

```go
type Vertex struct {
	Lat, Long float64
}

var m map[string]Vertex

func main() {
	m = make(map[string]Vertex)
	m["Bell Labs"] = Vertex{
		40.68433, -74.39967,
	}
	fmt.Println(m["Bell Labs"])
}

var f = map[string]Vertex{
	"Bell Labs": Vertex{
		40.68433, -74.39967,
	},
	"Google": Vertex{
		37.42202, -122.08408,
	},
}
```

若顶级类型**只是一个类型名**，你可以在文法的元素中**省略**它。

```go
var f = map[string]Vertex{
	"Bell Labs": {40.68433, -74.39967},
	"Google":    {37.42202, -122.08408},
}
```

## 修改映射

在映射 `m` 中插入或修改元素：

```
m[key] = elem
```

获取元素：

```
elem = m[key]
```

**删除元素：**

```
delete(m, key)
```

**通过双赋值检测某个键是否存在：**

```
elem, ok = m[key]
```

若 `key` 在 `m` 中，`ok` 为 `true` ；否则，`ok` 为 `false`。

若 `key` 不在映射中，那么 `elem` 是该**映射元素类型的零值**。

同样的，当从映射中读取某个不存在的键时，结果是映射的元素类型的零值。

**注** ：若 `elem` 或 `ok` 还未声明，你可以使用短变量声明：

```
elem, ok := m[key]
```

- 案列：wordcount

  ```go
  func WordCount(s string) map[string]int {
  	var res = make(map[string]int)
  	for _ , v :=range strings.Fields(s){
  		res[v]++
  	}
  	return res
  }
  //strings包
  ```

  

# 函数值

函数也是值。它们可以像其它值一样传递。

函数值可以用作函数的参数或返回值。

```go
func compute(fn func(float64, float64) float64) float64 {
	return fn(3, 4)
}
```



## 函数的闭包

Go 函数可以是一个闭包。闭包是一个函数值，它引用了其函数体之外的变量。该函数可以访问并赋予其引用的变量的值，换句话说，该函数被这些变量“绑定”在一起。

例如，函数 `adder` 返回一个闭包。每个闭包都被绑定在其各自的 `sum` 变量上。

```go
func adder() func(int) int {
	sum := 0
	return func(x int) int {
		sum += x
		return sum
	}
}

func main() {
	pos, neg := adder(), adder()
	for i := 0; i < 10; i++ {
		fmt.Println(
			pos(i),
			neg(-2*i),
		)
	}
}

/* 输出
0 0
1 -2
3 -6
6 -12
10 -20
15 -30
21 -42
28 -56
36 -72
45 -90
*/
```

- 斐波那契闭包

```

// 返回一个“返回int的函数”
func fibonacci() func() int {
	val1 := -1
	val2 := -2
	sum  := 0
	return func ()int {
		sum = val1 + val2
		val1++
		val2++
		if sum < -1{
			return 0
		}else if sum == -1{
			return 1
		}
		return sum
	}
}

func main() {
	f := fibonacci()
	for i := 0; i < 10; i++ {
		fmt.Println(f())
	}
}

```

# 方法

**Go 没有类。不过可以为结构体类型定义方法。**

方法就是一类带特殊的 **接收者** 参数的函数。

方法接收者在它自己的参数列表内，位于 `func` 关键字和方法名之间。

```go
type Vertex struct {
	X, Y float64
}

func (v Vertex) Abs() float64 {		//对比一般函数的声明 func Abs() float64{}
	return math.Sqrt(v.X*v.X + v.Y*v.Y)
}
func main() {
	v := Vertex{3, 4}
	fmt.Println(v.Abs())
}
```

- **方法只是个带接收者参数的函数。**

```go
func Abs(v Vertex) float64 {
	return math.Sqrt(v.X*v.X + v.Y*v.Y)
}

func main() {
	v := Vertex{3, 4}
	fmt.Println(Abs(v))
}
```

- **接收者的类型定义和方法声明必须在同一包内；不能为内建类型声明方法。**

- 也可以为非结构体类型声明方法。

  ```go
  type MyFloat float64
  
  func (f MyFloat) Abs() float64 {
  	if f < 0 {
  		return float64(-f)
  	}
  	return float64(f)
  }
  ```

  

## 指针接收者

对于某类型 `T`，接收者的类型可以用 `*T` 的文法。（此外，`T` 不能是像 `*int` 这样的指针。）

```go
func (v Vertex) Abs() float64 {
	return math.Sqrt(v.X*v.X + v.Y*v.Y)
}

func (v* Vertex) Scale(f float64) {
	v.X = v.X * f
	v.Y = v.Y * f
}

func main() {
	v := Vertex{3, 4}
	v.Scale(10)	//如果Scale是值接收者，那么v将不会改变
	fmt.Println(v.Abs())
}
```

指针接收者的方法可以修改接收者指向的值，若使用值接收者，那么 `Scale` 方法会对原始 `Vertex` 值的**副本**进行操作。

### 方法与指针重定向

比较前两个程序，你大概会注意到带指针参数的函数必须接受一个指针：

```
var v Vertex
ScaleFunc(v, 5)  // 编译错误！
ScaleFunc(&v, 5) // OK
```

而以指针为接收者的方法被调用时，接收者既能为值又能为指针：

```
var v Vertex
v.Scale(5)  // OK
p := &v
p.Scale(10) // OK
```

对于语句 `v.Scale(5)`，即便 `v` 是个值而非指针，带指针接收者的方法也能被直接调用。 也就是说，由于 `Scale` 方法有一个指针接收者，**为方便起见，Go 会将语句 `v.Scale(5)` 解释为 `(&v).Scale(5)`。**

- 接受一个值作为参数的**函数**必须接受一个指定类型的值，不会发生指针的自动解释
- 而以值为接收者的**方法**被调用时，接收者既能为值又能为指针，会自动进行解释

使用指针接收者的原因有二：

首先，**方法能够修改其接收者指向的值。**

其次，这样**可以避免在每次调用方法时复制该值。若值的类型为大型结构体时，这样做会更加高效。**

**通常来说，所有给定类型的方法都应该有值或指针接收者，但并不应该二者混用。**

# 接口

**接口类型** 是由一组方法签名定义的集合。

接口类型的变量可以**保存任何实现了这些方法的值**。

```go
type MyFloat float64

func (f MyFloat) Abs() float64 {
	if f < 0 {
		return float64(-f)
	}
	return float64(f)
}

type Vertex struct {
	X, Y float64
}

func (v *Vertex) Abs() float64 {
	return math.Sqrt(v.X*v.X + v.Y*v.Y)
}
type Abser interface {
	Abs() float64
}

func main() {
	var a Abser
	f := MyFloat(-math.Sqrt2)
	v := Vertex{3, 4}

	a = f  // a MyFloat 实现了 Abser
	a = &v // a *Vertex 实现了 Abser

	// 下面一行，v 是一个 Vertex（而不是 *Vertex）
	// 所以没有实现 Abser。
	//a = v

	fmt.Println(a.Abs())
}
//输出 5
//如果注释掉 v:= Vertex{3,4} 输出1.4142135623730951
```

- 类型**通过实现一个接口的所有方法来实现该接口**。既然**无需专门显式声明**，也就没有“implements”关键字。

```go
type I interface {
	M()
}

type T struct {
	S string
}

// 此方法表示类型 T 实现了接口 I，但我们无需显式声明此事。
func (t T) M() {
	fmt.Println(t.S)
}
```



### 接口值

接口也是值。它们可以像其它值一样传递。

接口值可以用作函数的参数或返回值。

在内部，接口值可以看做包含值和具体类型的元组：

```
(value, type)
```

接口值保存了一个具体底层类型的具体值。

接口值调用方法时会执行其底层类型的同名方法。

```go
type Intface interface {
	M()
}

type T struct {
	S string
}

func (t *T) M() {
	fmt.Println(t.S)
}

type F float64

func (f F) M() {
	fmt.Println(f)
}

func main() {
	var i Intface

	i = &T{"Hello"}
	describe(i)
	i.M()

	i = F(math.Pi)
	describe(i)
	i.M()
}

func describe(i I) {
	fmt.Printf("(%v, %T)\n", i, i)
}
```

### 底层为nil的接口值

即便接口内的具体值为 nil，方法仍然会被 nil 接收者调用。

在一些语言中，这会触发一个空指针异常，但在 Go 中通常会写一些方法来优雅地处理它（如本例中的 `M` 方法）。

**注意:** 保存了 nil 具体值的接口其自身并不为 nil。

```go
type I interface {
	M()
}

type T struct {
	S string
}

func (t *T) M() {
	if t == nil {
		fmt.Println("<nil>")
		return
	}
	fmt.Println(t.S)
}

func main() {
	var i I

	var t *T
	i = t
	describe(i)
	i.M()

	i = &T{"hello"}
	describe(i)
	i.M()
}

func describe(i I) {
	fmt.Printf("(%v, %T)\n", i, i)
}
/* 结果
(<nil>, *main.T)
<nil>
(&{hello}, *main.T)
hello
*/
```

### nil接口值

nil 接口值既不保存值也不保存具体类型。

为 nil 接口调用方法会产生运行时错误，因为接口的元组内并未包含能够指明该调用哪个 **具体** 方法的类型。

```go
type I interface {
	M()
}

func main() {
	var i I
	describe(i)
	i.M()
}

func describe(i I) {
	fmt.Printf("(%v, %T)\n", i, i)
}
```

### 空接口

指定了零个方法的接口值被称为 *空接口：*

```
interface{}
```

空接口**可保存任何类型的值**。（**因为每个类型都至少实现了零个方法。**）

空接口被用来处理未知类型的值。例如，`fmt.Print` 可接受类型为 `interface{}` 的任意数量的参数。

```go
func main() {
	var i interface{}
	describe(i)

	i = 42
	describe(i)

	i = "hello"
	describe(i)
}

func describe(i interface{}) {
	fmt.Printf("(%v, %T)\n", i, i)
}
```

# 类型断言

**类型断言** 提供了访问接口值底层具体值的方式。

```
t := i.(T)
```

该语句断言接口值 `i` 保存了具体类型 `T`，并将其底层类型为 `T` 的值赋予变量 `t`。

若 `i` 并未保存 `T` 类型的值，该语句就会触发一个恐慌。

为了 **判断** 一个接口值是否保存了一个特定的类型，类型断言可返回两个值：其底层值以及一个报告断言是否成功的布尔值。

```
t, ok := i.(T)
```

若 `i` 保存了一个 `T`，那么 `t` 将会是其底层值，而 `ok` 为 `true`。

否则，`ok` 将为 `false` 而 `t` 将为 `T` 类型的零值，程序并不会产生恐慌。

请注意这种语法和读取一个映射时的相同之处。

```go
func main() {
	var i interface{} = "hello"

	f, ok := i.(float64)
	fmt.Println(f, ok)

	f = i.(float64) // 报错(panic)
	fmt.Println(f)
}
/*
0 false
panic: interface conversion: interface {} is string, not float64
*/
```

# 类型选择

**类型选择** 是一种按顺序从几个类型断言中选择分支的结构。

类型选择与一般的 switch 语句相似，不过类型选择中的 case 为类型（而非值）， 它们针对给定接口值所存储的值的类型进行比较。

```
switch v := i.(type) {
case T:
    // v 的类型为 T
case S:
    // v 的类型为 S
default:
    // 没有匹配，v 与 i 的类型相同
}
```

类型选择中的声明与类型断言 `i.(T)` 的语法相同，只是具体类型 `T` 被替换成了关键字 `type`。

# Stringer

[`fmt`](https://go-zh.org/pkg/fmt/) 包中定义的 [`Stringer`](https://go-zh.org/pkg/fmt/#Stringer) 是最普遍的接口之一。

```
type Stringer interface {
    String() string
}
```

`Stringer` 是一个可以用字符串描述自己的类型。`fmt` 包（还有很多包）都通过此接口来打印值。

```go
type Person struct {
	Name string
	Age  int
}

func (p Person) String() string {
	return fmt.Sprintf("%v (%v years)", p.Name, p.Age) 		//fmt.Sprintf而不是fmt.Printf
}

func main() {
	a := Person{"Arthur Dent", 42}
	z := Person{"Zaphod Beeblebrox", 9001}
	fmt.Println(a, z)
}
```

# 错误

Go 程序使用 `error` 值来表示错误状态。

与 `fmt.Stringer` 类似，`error` 类型是一个内建接口：

```
type error interface {
    Error() string
}
```

（与 `fmt.Stringer` 类似，`fmt` 包在打印值时也会满足 `error`。）

通常函数会返回一个 `error` 值，调用的它的代码应当判断这个错误是否等于 `nil` 来进行错误处理。

```
i, err := strconv.Atoi("42")
if err != nil {
    fmt.Printf("couldn't convert number: %v\n", err)
    return
}
fmt.Println("Converted integer:", i)
```

**`error` 为 nil 时表示成功；非 nil 的 `error` 表示失败。**

- 练习

  `Sqrt` 接受到一个负数时，应当返回一个非 nil 的错误值。复数同样也不被支持。

  **注意:** 在 `Error` 方法内调用 `fmt.Sprint(e)` 会让程序陷入死循环。可以通过先转换 `e` 来避免这个问题：`fmt.Sprint(float64(e))`。这是为什么呢？

  修改 `Sqrt` 函数，使其接受一个负数时，返回 `ErrNegativeSqrt` 值。

  ```go
  type ErrNegativeSqrt float64;
  func ( e ErrNegativeSqrt) Error() string{
  	return fmt.Sprintf("cannot Sqrt negative number: %v",float64(e))	
  }
  func Sqrt(x float64) (float64, error) {
  	if x < 0{
  		return 0,ErrNegativeSqrt(x)
  	}
  	z:= float64(1)
  	for i:=0;i < 10;i++{
  		z -= (z * z -x )/(2 * z)		
  	}
  	return z,nil;
  }
  
  func main() {
  	fmt.Println(Sqrt(2))
  	fmt.Println(Sqrt(-2))
  }
  ```

  

# Reader

## Reader

`io` 包指定了 `io.Reader` 接口，它表示从数据流的末尾进行读取。

Go 标准库包含了该接口的[许多实现](https://go-zh.org/search?q=Read#Global)，包括文件、网络连接、压缩和加密等等。

`io.Reader` 接口有一个 `Read` 方法：

```
func (T) Read(b []byte) (n int, err error)
```

`Read` 用数据填充给定的字节切片并返回填充的字节数和错误值。在遇到数据流的结尾时，它会返回一个 `io.EOF` 错误。

示例代码创建了一个 [`strings.Reader`](https://go-zh.org/pkg/strings/#Reader) 并以每次 8 字节的速度读取它的输出。

```go
import (
	"fmt"
	"io"
	"strings"
)

func main() {
	r := strings.NewReader("Hello, Reader!")

	b := make([]byte, 8)
	for {
		n, err := r.Read(b)
		fmt.Printf("n = %v err = %v b = %v\n", n, err, b)
		fmt.Printf("b[:n] = %q\n", b[:n])
		if err == io.EOF {
			break
		}
	}
}
```

- 练习：实现一个 `Reader` 类型，它产生一个 ASCII 字符 `'A'` 的无限流。

  ```go
  type MyReader struct{}
  
  // TODO: 给 MyReader 添加一个 Read(a []byte ) (int, error) 方法
  func (mr MyReader) Read(a []byte) (int,error){
  	for i := range a{
  		a[i] = 'A'	
  	}
  	return len(a),nil;
  }
  func main() {
  	reader.Validate(MyReader{})
  }
  ```

  

# Go程

Go 程（goroutine）是由 Go 运行时管理的轻量级线程。

```
go f(x, y, z)
```

会启动一个新的 Go 程并执行

```
f(x, y, z)
```

Go 程在相同的地址空间中运行，因此在访问共享的内存时必须进行同步。[`sync`](https://go-zh.org/pkg/sync/) 包提供了这种能力，不过在 Go 中并不经常用到，因为还有其它的办法

```go
import (
	"fmt"
	"time"
)

func say(s string) {
	for i := 0; i < 5; i++ {
		time.Sleep(100 * time.Millisecond)
		fmt.Println(s)
	}
}

func main() {
	go say("world")
	say("hello")
}
```

# 信道

信道是带有类型的管道，你可以通过它用信道操作符 `<-` 来发送或者接收值。

```
ch <- v    // 将 v 发送至信道 ch。
v := <-ch  // 从 ch 接收值并赋予 v。
```

（“箭头”就是数据流的方向。）

和映射与切片一样，信道在使用前必须创建：

```
ch := make(chan int)
```

默认情况下，发送和接收操作在另一端准备好之前都会阻塞。这使得 Go 程可以在没有显式的锁或竞态变量的情况下进行同步。

- 练习

  ```go
  import "fmt"
  
  func sum(s []int, c chan int) {
  	sum := 0
  	for _, v := range s {
  		sum += v
  	}
  	c <- sum // 将和送入 c
  }
  
  func main() {
  	s := []int{7, 2, 8, -9, 4, 0}
  
  	c := make(chan int)
  	go sum(s[:len(s)/2], c)
  	go sum(s[len(s)/2:], c)
  	x, y := <-c, <-c // 从 c 中接收
  
  	fmt.Println(x, y, x+y)
  }
  // -5 17 12
  ```

  

## 带缓冲的信道

信道可以是 *带缓冲的*。将缓冲长度作为第二个参数提供给 `make` 来初始化一个带缓冲的信道：

```
ch := make(chan int, 100)
```

仅当信道的缓冲区填满后，向其发送数据时才会阻塞。当缓冲区为空时，接受方会阻塞。

```go
func main() {
	ch := make(chan int, 2)
	ch <- 1
	ch <- 2
	fmt.Println(<-ch)
	fmt.Println(<-ch)
}
//当带缓冲的信道大小为1时，发生死锁。为什么？
```



## range和close

发送者可通过 `close` 关闭一个信道来表示没有需要发送的值了。接收者可以通过为接收表达式分配第二个参数来测试信道是否被关闭：若没有值可以接收且信道已被关闭，那么在执行完

```
v, ok := <-ch
```

之后 `ok` 会被设置为 `false`。

循环 `for i := range c` 会不断从信道接收值，直到它被关闭。

*注意：* **只有发送者才能关闭信道，而接收者不能**。向一个已经关闭的信道发送数据会引发程序恐慌（panic）。

*还要注意：* 信道与文件不同，通常情况下无需关闭它们。只有在必须告诉接收者不再有需要发送的值时才有必要关闭，例如终止一个 `range` 循环。

- 例：这个例子很好的说明了管道通信

  ```go
  func fibonacci(n int, c chan int) {
  	x, y := 0, 1
  	for i := 0; i < n; i++ {
  		c <- x
  		x, y = y, x+y
  	}
  	close(c)
  }
  
  func main() {
  	c := make(chan int, 10)
  	go fibonacci(cap(c), c)
  	for i := range c {
  		fmt.Println(i)
  	}
  }
  ```

  

## select

`select` 语句使一个 Go 程可以等待多个通信操作。

`select` 会阻塞到某个分支可以继续执行为止，这时就会执行该分支。当多个分支都准备好时会随机选择一个执行。

- 例

  ```go
  func fibonacci(c, quit chan int) {
  	x, y := 0, 1
  	for {
  		select {
  		case c <- x:
  			x, y = y, x+y
  		case <-quit:
  			fmt.Println("quit")
  			return
  		}
  	}
  }
  
  func main() {
  	c := make(chan int)
  	quit := make(chan int)
  	go func() {
  		for i := 0; i < 10; i++ {
  			fmt.Println(<-c)
  		}
  		quit <- 0
  	}()
  	fibonacci(c, quit)
  }
  ```

- 当 `select` 中的其它分支都没有准备好时，`default` 分支就会执行。

  为了在尝试发送或者接收时不发生阻塞，可使用 `default` 分支：

  ```go
  select {
  case i := <-c:
      // 使用 i
  default:
      // 从 c 中接收会阻塞时执行
  }
  ```



- 练习：等价二叉树

  ```go
  // Walk 步进 tree t 将所有的值从 tree 发送到 channel ch。
  func Walk(t *tree.Tree, ch chan int){
  	if t == nil{
  		return
  	}
  	Walk(t.Left,ch)
  	ch <- t.Value
  	Walk(t.Right,ch)
  }
  
  // Same 检测树 t1 和 t2 是否含有相同的值。
  func Same(t1, t2 *tree.Tree) bool{
  	ch1 := make( chan int)
  	ch2 := make( chan int)
  	s1 := make( []int,1)
  	s2 := make( []int,1)
  	go Walk(t1,ch1)
  	go Walk(t2,ch2)
  	select{
  		case c1 := <-ch1:
  			s1 = append(s1,c1)
  		case c2 := <-ch2:
  			s2 = append(s2,c2)
  	}
  	
  	for _,v1 :=range s1{
  		for _,v2 :=range s2{
  			if v1 != v2{
  				return false
  			}
  		}
  	}
  	return true
  			
  }
  
  func main() {
  	t1 := tree.New(1)
  	t2 := tree.New(1)
  	var res bool = Same(t1,t2)
  	fmt.Println(res)
  }
  ```

  

## Sync

互斥锁保证每次只有一个 Go 程能够访问一个共享的变量

Go 标准库中提供了 [`sync.Mutex`](https://go-zh.org/pkg/sync/#Mutex) 互斥锁类型及其两个方法：

- `Lock`
- `Unlock`

我们可以通过在代码前调用 `Lock` 方法，在代码后调用 `Unlock` 方法来保证一段代码的互斥执行。参见 `Inc` 方法。

我们也可以用 `defer` 语句来保证互斥锁一定会被解锁。参见 `Value` 方法。

- 练习：Web爬虫

  修改 `Crawl` 函数来并行地抓取 URL，并且保证不重复。

  *提示*：你可以用一个 map 来缓存已经获取的 URL，但是要注意 map 本身并不是并发安全的！

  ```go
  ```

# 如果想进一步学习

如果你需要标准库方面的帮助，请参考[包手册](https://go-zh.org/pkg/)。如果是语言本身的帮助，阅读[语言规范](https://go-zh.org/ref/spec)是件令人愉快的事情。

进一步探索 Go 的并发模型，参阅 [Go 并发模型](https://www.youtube.com/watch?v=f6kdp27TYZs)([幻灯片](https://talks.go-zh.org/2012/concurrency.slide))以及[深入 Go 并发模型](https://www.youtube.com/watch?v=QDDwwePbDtw)([幻灯片](https://talks.go-zh.org/2013/advconc.slide))并阅读[通过通信共享内存](https://go-zh.org/doc/codewalk/sharemem/)的代码之旅。

想要开始编写 Web 应用，请参阅[一个简单的编程环境](https://vimeo.com/53221558)([幻灯片](https://talks.go-zh.org/2012/simple.slide))并阅读[编写 Web 应用](https://go-zh.org/doc/articles/wiki/)的指南。

[函数：Go 中的一等公民](https://go-zh.org/doc/codewalk/functions/)展示了有趣的函数类型。

[Go 博客](https://blog.go-zh.org/)有着众多关于 Go 的文章和信息。

[mikespook 的博客](https://www.mikespook.com/tag/golang/)中有大量中文的关于 Go 的文章和翻译。

