# minicc 课程主页

这门课的目标很直接：带你把一个迷你 C 编译器真正拆开、看懂、再亲手做出来。你不会只停在“知道编译器大概分成词法、语法、语义、代码生成”这种概念层面，而是会沿着 `step_1` 到 `step_5` 的真实项目台阶，把命令行驱动、词法分析、抽象语法树、语义检查和 RISC-V 代码生成一段一段接起来。

如果你以前学过一些编译原理名词，却总觉得它们飘在 PPT 和文法规则里，没有真正落到代码里，这门课就是为这个缺口准备的。课程的主线一直保持同一个节奏：先把这一章为什么存在讲清楚，再进入对应 lab，在代码骨架里把这一章真正关键的部分补出来，然后用可观察的输出确认你做对了。

## 你最后会做出什么

学完整条主线后，你应当能独立解释并亲手走完下面这条链路：

1. 从命令行读取一个 `input.c`
2. 把源文件切成 token 流
3. 用递归下降解析器把 token 流变成 AST
4. 用符号表和作用域规则做语义检查
5. 把语义合法的 AST 生成为 RISC-V 32 汇编
6. 用交叉工具链汇编、链接，再交给 `qemu-riscv32` 运行

更重要的是，你会知道每一步为什么要存在，它和前一步、后一步是什么关系，以及一旦某一步写错，错误会在什么地方暴露出来。

## 学习路线

第一次进入课程时，不要急着跳到词法分析或语法分析。按这个顺序走：

1. 先读 [Chapter 0](00-orientation.md)，把工具链、仓库结构和第一次 smoke test 跑通。
2. 再进入 [Practice 与实验说明](practice/README.md)，确认这一门课的 lab 怎么使用。
3. 从 [Chapter 1](chapters/ch01-step1-driver.md) 开始正式课程。
4. 每读完一章，立刻去做对应 lab。

如果你只想记住最短路径，那就是：

```text
Chapter 0 -> Practice 说明 -> Chapter 1 -> Lab01 -> Chapter 2 -> Lab02 -> ... -> Chapter 5 -> Lab05
```

## 主线章节

| 阶段 | Chapter | 对应 Lab | 你会解决的问题 |
| --- | --- | --- | --- |
| 起步 | [Chapter 0](00-orientation.md) | 准备阶段 | 把工具链、仓库和 smoke test 进入可工作的状态 |
| 驱动 | [Chapter 1](chapters/ch01-step1-driver.md) | `lab01-step1` | 编译器最小驱动、命令行、文件读取、固定汇编输出 |
| 前端 | [Chapter 2](chapters/ch02-step2-lexer.md) | `lab02-step2` | 把源代码切成 token 流 |
| 前端 | [Chapter 3](chapters/ch03-step3-parser.md) | `lab03-step3` | 递归下降解析器与 AST |
| 中端 | [Chapter 4](chapters/ch04-step4-semantic.md) | `lab04-step4` | 符号表、作用域、语义错误检测 |
| 后端 | [Chapter 5](chapters/ch05-step5-codegen.md) | `lab05-step5` | 生成 RISC-V 汇编并在 QEMU 中运行 |

## 附录

附录不属于主线，只有在你需要时再跳回来看：

- [A1：环境、工具链与预备知识](appendices/A1-prerequisites.md)
- [A2：常见陷阱与调试办法](appendices/A2-pitfalls.md)
- [A3：参考资料与下一步](appendices/A3-references.md)

## 现在就开始

如果你准备开始这门课，下一步就是：

1. 进入 [Chapter 0](00-orientation.md)
2. 跑通里面的环境检查和 practice smoke test
3. 然后继续进入 [Chapter 1](chapters/ch01-step1-driver.md)
