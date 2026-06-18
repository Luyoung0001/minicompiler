---
title: 'Chapter 0 — 出发前的准备'
layout: 'course_page'
permalink: '/course/00-orientation/'
prev_page:
  title: 'minicc 课程主页'
  url: '/course/'
next_page:
  title: 'Practice 与实验说明'
  url: '/course/practice/'
source_path: 'course/00-orientation.md'
---

> **预计时间**：45 ~ 90 分钟  
> **本章目标**：确认 `gcc`、RISC-V 工具链、QEMU 和 lab 仓库都已经处于可继续推进的状态

正式课程会从下一章开始进入编译器本身，但在那之前，你得先把起跑线摆正。这里的“准备”不是为了多看一页导言，而是为了确保你后面每次读 chapter、做 lab、看到测试输出时，都知道自己到底在看什么、改什么、以及哪里出了问题。

做完这一章，你还不需要已经懂词法分析、AST 或 RISC-V 调用约定。你只需要进入一个很明确的工作状态：机器上的工具是通的，`miniCompiler_lab` 已经在本地，第一次 smoke test 已经跑过，接下来可以直接进入 `Chapter 1`。

## 0.1 课程网页和 lab 仓库分别干什么

先把两个角色分清楚，不然后面很容易改错地方。

1. 课程网页：讲解知识点、给出章节顺序、解释每个 lab 为什么存在。
2. `miniCompiler_lab`：学员实践仓库。后面的每个 lab 都在这里完成。

也就是说，这门课的节奏是：先读 chapter，再进入 lab 仓库里对应的实验目录，补这一章该写的代码。你不需要在一个完整工程里到处找入口，当前 chapter 会告诉你应该进入哪个 `labs/...` 目录。

## 0.2 这一章结束时，你应该达到什么状态

本章完成后，你应当同时满足下面四条：

1. 你的机器上能找到 `gcc`
2. 你的机器上能找到 `riscv64-unknown-linux-gnu-gcc` 或兼容前缀的 RISC-V 工具链
3. 你的机器上能找到 `qemu-riscv32`
4. 你已经把 `miniCompiler_lab` 拉取到本地
5. 你已经在 `miniCompiler_lab/labs/lab01-step1/` 跑过第一次 smoke test

最后一条尤其重要。因为这门课不是“下载一堆讲义，然后自己猜实践边界”，而是要求你从一份已经准备好的实验骨架出发，沿着章节逐步完成编译器。

## 0.3 先检查最小工具链

请先在终端执行：

```bash
gcc --version
riscv64-unknown-linux-gnu-gcc --version
qemu-riscv32 --version
```

如果三条命令都能打印版本信息，说明你的基础环境已经够用了。

如果这里出现 `command not found`，先不要继续读后面的章节。优先把工具补齐：

- Ubuntu / Debian：

```bash
sudo apt install gcc-riscv64-unknown-elf qemu-user
```

或者使用仓库 README 里给出的 Linux 发行版对应包名。

此刻你不需要深入研究交叉编译器内部怎么工作。先记住一件事：这门课的最终目标不是只在主机上打印一些调试信息，而是要把生成的 RISC-V 汇编交给工具链和 QEMU 真正跑起来。

## 0.4 拉取 lab 仓库

课程网页负责讲解，lab 仓库负责让你写代码。现在打开一个你准备用来放实验的目录，执行：

```bash
git clone https://github.com/Luyoung0001/miniCompiler_lab.git
cd miniCompiler_lab
```

## 0.5 看一眼 lab 仓库，不要急着深入

在 `miniCompiler_lab` 根目录执行：

```bash
pwd
ls
```

你应该至少能看到这些内容：

- `README.md`
- `labs/`
- `scripts/`

这里先不要急着把所有源文件都翻完。你现在真正需要建立的，只是一个非常粗的地图：

- `labs/` 是你真正下手写代码的地方
- 每个 `labs/labXX-.../` 都有自己的 `TASK.md`、`Makefile` 和 `framework/student.c`
- `scripts/bootstrap-practice.sh` 是 Chapter 0 的统一检查入口

后面每章都会把这张粗地图收紧到当前这一章相关的 2~4 个文件上。

## 0.6 运行第一次 practice smoke test

现在进入课程实践区，执行：

```bash
bash scripts/bootstrap-practice.sh
```

这个脚本会做三件事：

1. 检查 `gcc`、RISC-V 工具链、QEMU 是否存在
2. 进入 `labs/lab01-step1/`
3. 自动执行一次 `make clean && make test`

如果你想手动确认它最后做了什么，也可以自己继续执行：

```bash
cd labs/lab01-step1
make clean && make test
```

## 0.7 第一次看到 FAIL，为什么反而是正常的

第一次进入 `Lab01`，你看到的通常不是全通过，而是若干个 `[FAIL]`。这并不意味着环境损坏，恰恰说明三件事已经成立：

1. lab 骨架已经能编译
2. 自动验证程序已经能运行
3. 本章对应的待实现函数确实还没写，所以失败被清楚地暴露出来了

这门课真正的出发点不是“你已经拥有一个完成的编译器”，而是“你已经进入一个可以逐章把它做出来的状态”。

## 0.8 如果 smoke test 跑不起来，先查什么

如果这里没有跑到预期输出，先按这个顺序排查：

| 现象 | 先检查什么 |
| --- | --- |
| `gcc: command not found` | 主机编译器没有装好 |
| `riscv64-unknown-linux-gnu-gcc: command not found` | 交叉工具链没装好 |
| `qemu-riscv32: command not found` | QEMU 用户态模拟器没装好 |
| `No such file or directory` | 当前目录是不是 `miniCompiler_lab` 仓库根目录 |
| `make` 阶段失败 | 先看是编译错误、链接错误，还是验证程序报错 |

如果你只是第一次看到若干 `[FAIL]`，那不属于故障，不要在这里过度排错，直接进入下一章即可。

## 0.9 本章小结

这一章没有让你开始写 lexer，也没有带你看文法规则。它只完成了一件更基础的事：把你准确送到正式课程入口。

现在你已经知道：

- 工具链是否齐全
- 课程网页和 lab 仓库分别扮演什么角色
- 第一个 lab 在哪里
- 第一次 smoke test 为什么会失败，以及为什么这正是正常现象

这就够了。真正的知识点，从下一章开始。

## 0.10 下一步

现在直接进入：

1. [Practice 与实验说明](/course/practice/)
2. [Chapter 1 — 编译器最小驱动与文件 I/O](/course/chapters/ch01-step1-driver/)
