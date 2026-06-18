---
title: 'Practice 与实验说明'
layout: 'course_page'
permalink: '/course/practice/'
prev_page:
  title: 'Chapter 0 — 出发前的准备'
  url: '/course/00-orientation/'
next_page:
  title: 'Chapter 1 — step_1 编译器最小驱动与文件 I/O'
  url: '/course/chapters/ch01-step1-driver/'
source_path: 'course/practice/README.md'
---

从 `Chapter 1` 开始，这门课就不再只是阅读材料，而是要求你真的动手写代码。后面的每一章都会配一个对应 lab。chapter 负责把这章为什么存在、这个概念到底解决什么问题讲清楚，lab 负责把这一章真正该补的代码边界交给你。

你可以把这里理解成“实践入口”，但不要把它当成题单。最推荐的节奏始终是：

1. 先读对应 chapter
2. 再进入对应 lab，读 `TASK.md`
3. 只修改本章指定的 `framework/student.c`
4. 运行 `make clean && make test`

这个顺序很重要。因为这门课不是让你只对着测试输出来猜实现，而是希望你先知道“为什么现在轮到这件事”，再去写代码。

## 这套 practice 和参考实现是什么关系

根目录的 `step_1/` 到 `step_5/` 是参考实现，它们展示了编译器每个阶段的完成状态。  
`course/practice/labs/` 不是那套实现的副本，而是把每一章真正关键的子问题单独抽出来，做成边界清晰的 scaffold。

也就是说：

- 你不会直接在 `step_2/lexer.c` 里练习
- 你会在 `course/practice/labs/lab02-step2/framework/student.c` 里完成和它同主题的核心逻辑

这样做的好处是，lab 的代码范围会小很多，你能更快把注意力放在这一章真正想教的点上。

## 目录长什么样

最重要的是 `labs/`：

```text
labs/
├── lab01-step1/
├── lab02-step2/
├── lab03-step3/
├── lab04-step4/
└── lab05-step5/
```

每个 lab 都是一个最小练习单元。典型结构是：

```text
lab02-step2/
├── Makefile
├── TASK.md
├── sample.c
└── framework/
    ├── student.c
    ├── student.h
    ├── verify.c
    └── verify.h
```

这几份文件的角色很固定：

- `TASK.md`：这一章实验到底要做什么
- `framework/student.c`：你主要修改的文件
- `framework/verify.c`：自动验证程序，不改
- `Makefile`：这一章的编译与测试入口

## 五个 lab 分别在练什么

| Lab | 对应 Chapter | 练习主题 | 主要验证现象 |
| --- | --- | --- | --- |
| `lab01-step1` | Chapter 1 | 命令行参数、文件读取、固定汇编输出 | 能解析参数、读文件、写出最小汇编骨架 |
| `lab02-step2` | Chapter 2 | 词法分析器基础 | token 流类型和值正确 |
| `lab03-step3` | Chapter 3 | 递归下降解析与 AST | AST 结构符合程序层次 |
| `lab04-step4` | Chapter 4 | 符号表与语义检查 | 能报出未定义变量、重复定义等错误 |
| `lab05-step5` | Chapter 5 | RISC-V 代码生成 | 汇编能生成并在 QEMU 中运行 |

## 推荐工作流

从 `Lab01` 开始，后面每一章都尽量保持同样的工作法：

1. 读 chapter，先建立这一章的直觉
2. 打开对应 `TASK.md`
3. 只改指定的 `student.c`
4. 运行 `make clean && make test`
5. 对照验证输出和常见失败继续修正

不要一上来就把整个 `student.c` 一次性写完。对于编译器项目来说，最危险的不是“代码短”，而是“看起来每一行都彼此有关”。把一章的任务拆开、跑通、验证，才是最稳的推进方式。

## 现在从哪里开始

如果你刚完成 `Chapter 0`，下一步就是：

1. 读 [Chapter 1](/course/chapters/ch01-step1-driver/)
2. 打开 [Lab01 的任务说明](/course/practice/labs/lab01-step1/)
3. 开始修改 `labs/lab01-step1/framework/student.c`
