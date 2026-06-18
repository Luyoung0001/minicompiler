# Chapter 3 — step_3 递归下降解析器与抽象语法树

> **对应实践**：[Lab03 - step_3 语法分析与 AST](../practice/labs/lab03-step3/TASK.md)  
> **主要修改文件**：`course/practice/labs/lab03-step3/framework/student.c`  
> **参考实现位置**：`step_3/parser.c`、`step_3/ast.c`

到了这一章，编译器终于要开始处理“结构”了。token 流虽然已经比字符流干净很多，但它本质上仍然是一条线。而程序不是一条线。表达式有优先级，`if` 有条件和分支，函数定义有函数名和函数体，代码块里还会再套语句。

词法分析器负责把原材料切好；语法分析器负责把这些片段重新组装成有层次的树。这棵树，就是 AST。

## 3.1 为什么 token 流还不够

看下面这段代码：

```c
int sum = x + y * z;
```

如果你只停在 token 流层面，它只是：

```text
INT IDENT ASSIGN IDENT PLUS IDENT STAR IDENT SEMICOLON
```

但这串 token 里没有直接告诉你一个关键事实：这里应该先算 `y * z`，再和 `x` 做加法。

这说明 token 流只保存了“顺序”，还没有保存“结构关系”。而 parser 的工作，就是把这种结构关系补出来。

## 3.2 AST 为什么叫“抽象”语法树

很多人第一次看到 AST，会疑惑：为什么不是把源码原样存下来，而要“抽象”？

原因是编译器后面的阶段不需要保留所有书写细节。它真正关心的是：

- 这是一个变量声明，还是一个返回语句
- 这是一个二元运算，操作符是什么
- 这是一个 `if`，条件、then 分支、else 分支分别是什么

像多余空格、注释、甚至某些书写层面的细枝末节，都已经不重要了。AST 保留的是对后续语义检查和代码生成有意义的结构，而不是源码的排版外观。

## 3.3 递归下降为什么适合这门课

这门课选择的 parser 风格是递归下降。理由不是它“理论上最强”，而是它特别适合让你把语法结构和代码结构一一对应起来。

你在 `step_3/parser.c` 里看到的一组函数：

- `parse_primary`
- `parse_factor`
- `parse_term`
- `parse_comparison`
- `parse_statement`
- `parse_function`

它们并不是随手分出来的。它们正对应着“从最小表达式单元一路往上包”的过程。

也就是说，递归下降的一个巨大优点是：你几乎能直接从函数名看出 parser 正在哪一层结构上工作。

## 3.4 优先级不是额外补丁，而是函数层次本身

这一章特别值得建立一个直觉：表达式优先级并不是最后临时修一修，而是 parser 分层时就应该体现出来。

例如：

- `parse_factor` 处理乘除
- `parse_term` 在它上面处理加减
- `parse_comparison` 再往上处理 `<`、`>`、`<=`、`>=`

这种分层一旦搭好，优先级几乎是自然长出来的，而不是靠后期打补丁硬拼出来的。

所以当你在 lab 里补 `parse_primary` 或观察 `parse_if` 的结构时，不要只把它当成几个函数分支。你真正该看到的是：**语法层次正在通过函数调用层次显影**。

## 3.5 参考实现里最该看的两类文件

这一章有两个核心文件群：

```text
step_3/
├── parser.c / parser.h
└── ast.c / ast.h
```

`parser.c` 负责“怎样认结构”，`ast.c` 负责“怎样把结构装进节点里”。

阅读时先问自己两个问题：

1. 当 parser 识别出一个结构时，它会创建哪种 AST 节点
2. 这类节点在后续语义分析和代码生成里为什么有用

只要你始终盯着“识别 -> 建树”这条线，parser 和 AST 就不会裂成两份互不相干的知识。

## 3.6 本章 practice 边界

这一章的 practice 路径是：

- `course/practice/labs/lab03-step3/`

主要修改文件：

- `framework/student.c`

验证命令：

```bash
cd course/practice/labs/lab03-step3
make clean && make test
```

本章 lab 故意没有把整份 parser 都掏给你，而是把最有代表性的三个结构节点留成了 TODO：

1. `student_parse_primary`
2. `student_parse_if`
3. `student_parse_function`

为什么是这三个？

- `parse_primary` 是表达式树真正开始生长的地方
- `parse_if` 是控制流结构第一次出现的地方
- `parse_function` 把整个程序的最外层骨架搭起来

只要你把这三个点理解透，递归下降解析器的整体轮廓就已经建立起来了。

## 3.7 本章最值得观察的现象

你在这一章验证时，最该看的是 AST 的形状，而不是单个 token。

尤其要确认：

1. 程序根节点是不是 `Program`
2. `main` 函数是不是被挂在函数定义节点下面
3. `if` 的条件和分支是不是分开保存
4. `x + y * z` 这种表达式的树形是不是体现了优先级

只要树形关系对了，后面的语义分析和代码生成才有可靠的输入。

## 3.8 本章小结

这一章让编译器第一次拥有“结构感”。从这里开始，输入不再只是 token 列表，而是已经能表达程序层次关系的 AST。后面无论做类型检查还是生成汇编，本质上都建立在这棵树上。

## 3.9 下一步

现在去完成：

1. [Lab03 - step_3 语法分析与 AST](../practice/labs/lab03-step3/TASK.md)
2. 对照验证输出，确认 AST 形状正确
3. 然后继续进入 [Chapter 4 — 符号表与语义分析](ch04-step4-semantic.md)
