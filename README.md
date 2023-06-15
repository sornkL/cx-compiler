# CX Compiler

ECNU《编译原理实践》期末大作业，基于LLVM实现带扩展的CX编译器

## Requirements

- C++ 17
- LLVM 16

## Usage

### 构建项目

```shell
cd cx-compiler
make clean && make
export PATH=$PATH:$(pwd)
alias cxc="./cxc.out"
```

### 交互模式

```shell
cxc -i [-O]
```

`-i` 选项启用交互模式，`-O` 选项开启优化

```
>>> { write 1; }
>>> Read top-level expression:
define void @main() {
entry:
  %0 = call i32 (ptr, ...) @printf(ptr @0, i32 1)
  %1 = call i32 (ptr, ...) @printf(ptr @1)
  ret void
}

1
```

### 命令行模式

```shell
cxc -o <output_filename.ll> <input_filename.cx> [-O]
```

`-o` 选项启用命令行模式，`output_filename.ll` 指定输出LLVM中间代码的文件路径，`input_filename.cx` 指定源文件路径，`-O` 选项开启优化

## Examples

### 变量声明与赋值

```c
{
    int i;  // 32位整数，默认0
    float f;  // 32位浮点数，默认0.0
    bool b;  // 布尔值，默认false

    i = 1;
    f = 3.14;
    b = true;
}
```

### 基本运算符

`int` 与 `float` 支持 `+ - * / % < <= > >= == !=`（`%`取模运算仅支持整数）
`bool` 支持`&& || ^ !`

算术运算符要求两边同类型，结果类型不变；比较运算符的结果是 `bool` 类型；逻辑运算符的结果也是 `bool` 类型

### if 语句

```c
{
    if (true) {
      	// ...
    }
  
    if (true) {
      	// ...
    } else {
      	// ...
    }
}
```

支持以上两种形式，`if` 条件要求是 `bool` 类型

### while 语句

```c
{
    while (true) {
      	// ...
    }
}
```

`while` 条件要求是 `bool` 类型

### I/O

```c
{
    int i;
    read i;  // 标准输入，scanf()
    write i; // 标准输出，printf()
}
```

### for 语句

```c
{
    int i;
    for (i=0; i<10; i=i+1) {
      	// ...
    }
}
```

### switch 语句

```c
{
    int x;
    switch (x) {
    case 0:
    	// ...
    case 1:
      	// ...
    default:
        // ...
    }
}
```

要求一定有 `default`，匹配任意 `case` 后不执行其他的 `case`，无法匹配时执行 `default`

### 函数

```c
func fibonacci(n: int) -> int {
    if (n == 0) {
        return 0;
    } else {
        if (n == 1) {
            return 1;
        } else {
            return fibonacci(n-1) + fibonacci(n-2);
        }
    }
}

{
    int n;
    read n;
    write fibonacci(n);
}
```

函数支持参数和返回值，无返回值时定义成 `func function(param: type) { }`

### 中间代码优化

```c
func test(x: float) -> float {
    return (1.0+2.0+x)*(x+(1.0+2.0));
}
```

关闭优化：

```
define float @test(float %x) {
entry:
  %x1 = alloca float, align 4
  store float %x, ptr %x1, align 4
  %x2 = load float, ptr %x1, align 4
  %addtmp = fadd float 3.000000e+00, %x2
  %x3 = load float, ptr %x1, align 4
  %addtmp4 = fadd float %x3, 3.000000e+00
  %multmp = fmul float %addtmp, %addtmp4
  ret float %multmp
}
```

开启优化：

```
define float @test(float %x) {
entry:
  %addtmp = fadd float %x, 3.000000e+00
  %multmp = fmul float %addtmp, %addtmp
  ret float %multmp
}
```
