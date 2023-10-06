**GO安装**

**定义**

```go
var 变量名 类型 = 初始值
var 变量1,变量2 类型		// 同时声明两个变量
var 变量1,变量2 类型 = 初始值1,初始值2
变量名 := 初始值				// 自动推导类型
变量1,变量2 := 初始值1,值2		// 自动推导类型

// 交换两个变量的值
变量1,变量2 = 变量2,变量1		// 两个变量的值会互相交换
```



# 1、函数

## 1.1 fmt

```go
	fmt.Print("Hello GoLang")		// 输出
	fmt.Println("Hello GoLang")		// 输出并换行,输出参数之间会自动加空格
	fmt.Printf("%s %d", "bum=",num)		// 	类似于C语言的printf()
		// %T,输出变量的类型, %t输出bool类型(true,false)


	fmt.Scan(&num)						// 输入
	fmt.Scanf("%d",&num)					// 格式化输入,类似C语言scanf
```

