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

     

# 语言变量

## 变量声明

变量声明使用 ***var***  关键字

```go
var identifier type					//单变量声明
var identifire1, identifier2 type	//多变量声明
```

1. 指定变量类型，但没初始化，则变量为系统默认值

   - 数值类型为0，布尔为false,字符串为“”

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

3. 省略 var ，注意 :=  是声明语句，若该变量已经声明就产生编译错误

   ```go
   v_name := value
   ```

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

   