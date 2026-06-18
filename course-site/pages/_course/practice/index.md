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

你可以把这里理解成“实践入口”，但不要把它当成题单。

## 先把 lab 仓库拉到本地

读课程网页时，你只需要拉取这个 lab 仓库：

```bash
git clone https://github.com/Luyoung0001/miniCompiler_lab.git
cd miniCompiler_lab
bash scripts/bootstrap-practice.sh
```

后面所有需要你修改的代码，都在这个仓库里。课程网页会用 `labs/...` 描述实验位置；这些路径都以 `miniCompiler_lab` 仓库根目录为起点。

lab 仓库地址是：

```text
https://github.com/Luyoung0001/miniCompiler_lab.git
```

## 你应该怎样使用这里

最推荐的节奏始终是：

1. 先读对应 chapter
2. 再进入对应 lab，读 `TASK.md`
3. 只修改本章指定的 `framework/student.c`
4. 运行 `make clean && make test`

这个顺序很重要。因为这门课不是让你只对着测试输出来猜实现，而是希望你先知道“为什么现在轮到这件事”，再去写代码。

## 这套 practice 怎样组织

`labs/` 把每一章真正关键的子问题单独抽出来，做成边界清晰的 scaffold。你不需要在一个庞大的完整工程里到处找入口，只需要跟着当前 chapter，进入对应 lab，完成 `framework/student.c` 里的 TODO。

也就是说：

- 你会在 `labs/lab02-step2/framework/student.c` 里完成词法分析这一章的核心逻辑
- 你每完成一章，就用该 lab 的 `make clean && make test` 验证一次

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
