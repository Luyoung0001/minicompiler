---
title: Lab01 - step_1 最小驱动
lab: Lab01
step: step_1
hours: 2h
deliverable: 完成参数解析、文件读取和固定汇编输出骨架
---

> **实验编号** Lab01 &nbsp;&nbsp; **预计耗时** 2h &nbsp;&nbsp; **对应参考实现** [`step_1/`](../../../step_1/) &nbsp;&nbsp; **本节产出** 跑通一个最小但真实的 `input.c -> out.s` 编译器驱动

## 实验目的

完成本 lab 后，你应当能够：

1. 解释为什么编译器项目第一章就需要命令行参数和文件 I/O
2. 写出最小参数解析逻辑，支持 `-o` 和输入文件
3. 把整个源文件读入内存
4. 输出一份固定的 RISC-V 汇编骨架

## 实验环境

- **主要修改文件**：`framework/student.c`
- **不要改**：`framework/verify.c`
- **命令**：

```bash
make clean && make test
```

## 实验内容

### task 1.1 跑通 baseline

```bash
make clean && make test
```

第一次运行时，看到 FAIL 是正常的。因为 `student.c` 里三个关键函数都还是占位实现。

### task 1.2 实现 `student_parse_args`

目标：

- 识别 `-o output.s`
- 识别输入文件
- 拒绝未知选项和缺失输入

这里不要追求完整的工业级参数系统。先把课程需要的最小边界站稳。

### task 1.3 实现 `student_read_file`

目标：

- 打开文件
- 读取完整内容
- 返回一个以 `'\0'` 结尾的缓冲区

这一步后面的每一章都会继续受益，因为 lexer 的输入起点就是这里。

### task 1.4 实现 `student_emit_stub_program`

目标：

- 向输出文件写一份固定的 RISC-V 汇编骨架
- 至少生成 `_start` 和一次 `exit`

这一章不要求你从源码推导汇编，只要求先把“编译器对外的输出形态”建立起来。

## 目标现象

完成后再次执行：

```bash
make clean && make test
```

你应当看到类似：

```text
[TEST 1] ... [PASS]
[TEST 2] ... [PASS]
[TEST 3] ... [PASS]
All tests passed!
```

## 常见失败

| 现象 | 原因 | 解决 |
| --- | --- | --- |
| `input file missing` | 没有记录输入文件 | 检查 `student_parse_args` |
| 输出文件为空 | 没真正写出汇编 | 检查 `student_emit_stub_program` |
| 读文件崩溃 | 缓冲区或文件长度处理不对 | 先打印文件大小再看分配 |

## 思考题

1. 为什么这一章即使输出固定汇编，也已经算编译器工作流的一部分？
2. 如果 `read_file` 不在末尾补 `'\0'`，后面的 lexer 会有什么风险？
