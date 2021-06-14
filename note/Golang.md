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

