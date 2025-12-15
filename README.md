# minicc - Mini C Compiler

一个面向 RISC-V 32 的迷你 C 语言编译器，用于学习编译原理。

## 项目概述

minicc 是一个从零开始构建的 C 语言子集编译器，将 C 代码编译为 RISC-V 32 位汇编代码，可在 QEMU 模拟器上运行。

### 特性

- 词法分析：标识符、数字、字符串、运算符、关键字
- 语法分析：递归下降解析器，生成抽象语法树 (AST)
- 语义分析：变量作用域检查、类型检查、未定义变量检测
- 代码生成：RISC-V 32 汇编 (rv32im)
- 函数调用：支持带参数的函数定义和调用

## 支持的语法

### 数据类型

```c
int x;          // 整型变量
int x = 10;     // 带初始化
```

### 运算符

| 类型 | 运算符 |
|------|--------|
| 算术 | `+` `-` `*` `/` `%` |
| 比较 | `==` `!=` `<` `<=` `>` `>=` |
| 逻辑 | `&&` `\|\|` `!` |
| 赋值 | `=` |

### 控制流

```c
// if-else 语句
if (condition) {
    // ...
} else {
    // ...
}

// while 循环
while (condition) {
    // ...
}
```

### 函数

```c
// 函数定义（支持最多 8 个参数）
int add(int a, int b) {
    return a + b;
}

// 函数调用
int result = add(3, 4);
```

## 项目结构

```
minicompiler/
├── Makefile            # 主构建文件
├── README.md           # 本文档
├── guide.md            # 开发指南
├── plan.md             # 项目计划
└── step_5/             # 当前版本
    ├── Makefile        # 构建配置
    ├── minicc.c        # 主程序入口
    ├── token.h         # Token 类型定义
    ├── lexer.h/c       # 词法分析器
    ├── ast.h/c         # 抽象语法树
    ├── parser.h/c      # 语法分析器
    ├── symbol.h/c      # 符号表
    ├── semantic.h/c    # 语义分析器
    ├── codegen.h/c     # 代码生成器
    ├── test.c          # 测试文件
    └── test_call.c     # 函数调用测试
```

## 编译器架构

```
源代码 (.c)
    │
    ▼
┌─────────────┐
│  词法分析器  │  lexer.c
│   (Lexer)   │  将源码转换为 Token 流
└─────────────┘
    │
    ▼
┌─────────────┐
│  语法分析器  │  parser.c
│  (Parser)   │  构建抽象语法树 (AST)
└─────────────┘
    │
    ▼
┌─────────────┐
│  语义分析器  │  semantic.c
│ (Semantic)  │  类型检查、作用域分析
└─────────────┘
    │
    ▼
┌─────────────┐
│  代码生成器  │  codegen.c
│  (CodeGen)  │  生成 RISC-V 汇编
└─────────────┘
    │
    ▼
汇编代码 (.s)
```

## 快速开始

### 环境要求

- GCC
- RISC-V 工具链：`riscv64-unknown-linux-gnu-gcc`
- QEMU 用户态模拟器：`qemu-riscv32`

### 安装依赖

```bash
# Ubuntu/Debian
sudo apt install gcc-riscv64-linux-gnu qemu-user
```

### 编译 minicc

```bash
cd step_5
make
```

### 使用方法

```bash
# 编译 C 文件为汇编
./minicc input.c -o output.s

# 查看 Token 列表
./minicc --tokens input.c

# 查看抽象语法树
./minicc --ast input.c

# 详细输出模式
./minicc -v input.c -o output.s
```

### 运行编译结果

```bash
# 编译为汇编
./minicc test.c -o out.s

# 汇编为目标文件
riscv64-unknown-linux-gnu-as -march=rv32im -mabi=ilp32 -o test.o out.s

# 链接为可执行文件
riscv64-unknown-linux-gnu-ld -m elf32lriscv -o test.elf test.o

# 在 QEMU 上运行
qemu-riscv32 ./test.elf
echo $?  # 查看返回值
```

或使用 Makefile：

```bash
make compile  # 编译 test.c
make run      # 在 QEMU 上运行
```

## 示例

### 基础算术

```c
int main() {
    int a = 10;
    int b = 20;
    return a + b;  // 返回 30
}
```

### 条件判断

```c
int main() {
    int x = 15;
    if (x > 10) {
        return 1;
    } else {
        return 0;
    }
}
```

### 循环

```c
int main() {
    int i = 0;
    int sum = 0;
    while (i < 10) {
        sum = sum + i;
        i = i + 1;
    }
    return sum;  // 返回 45
}
```

### 函数调用

```c
int add(int a, int b) {
    return a + b;
}

int square(int x) {
    return x * x;
}

int main() {
    int result = add(3, 4);     // 7
    int sq = square(5);         // 25
    return add(result, sq);     // 32
}
```

## RISC-V 代码生成

### 调用约定

| 寄存器 | 用途 |
|--------|------|
| a0-a7  | 函数参数 / 返回值 (a0) |
| ra     | 返回地址 |
| sp     | 栈指针 |
| s0     | 帧指针 |

### 栈帧布局

```
高地址
    ┌─────────────────┐
    │  调用者的帧     │
    ├─────────────────┤ ← 旧 sp
    │  ra (返回地址)  │  sp + 60
    │  s0 (旧帧指针)  │  sp + 56
    │  局部变量...    │
    │                 │
    ├─────────────────┤ ← sp, s0 - 64
低地址
```

### 生成的汇编示例

```asm
    .globl add
add:
    addi sp, sp, -64      # 分配栈帧
    sw ra, 60(sp)         # 保存返回地址
    sw s0, 56(sp)         # 保存帧指针
    addi s0, sp, 64       # 设置新帧指针
    sw a0, -12(s0)        # 保存参数 a
    sw a1, -16(s0)        # 保存参数 b
    # ... 函数体 ...
    lw ra, 60(sp)         # 恢复返回地址
    lw s0, 56(sp)         # 恢复帧指针
    addi sp, sp, 64       # 释放栈帧
    ret                   # 返回
```

## 开发历程

| 步骤 | 内容 | 状态 |
|------|------|------|
| Step 1 | 环境搭建与基础框架 | ✅ |
| Step 2 | 词法分析器 | ✅ |
| Step 3 | 语法分析器 | ✅ |
| Step 4 | 语义分析 | ✅ |
| Step 5 | 代码生成 | ✅ |
| Step 5+ | 函数调用 | ✅ |

## 限制

- 仅支持 `int` 类型（32 位整数）
- 不支持数组和指针
- 不支持字符串操作
- 不支持全局变量
- 函数最多支持 8 个参数
- 固定 64 字节栈帧大小

## 许可证

本项目仅供学习使用。

## 参考资料

- [RISC-V 指令集手册](https://riscv.org/specifications/)
- [RISC-V 调用约定](https://riscv.org/wp-content/uploads/2015/01/riscv-calling.pdf)
- [编译原理 - 龙书](https://www.amazon.com/Compilers-Principles-Techniques-Tools-2nd/dp/0321486811)
