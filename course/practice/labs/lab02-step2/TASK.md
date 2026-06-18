---
title: Lab02 - step_2 词法分析器
lab: Lab02
step: step_2
hours: 3h
deliverable: 完成最小 lexer 骨架，正确识别空白、标识符、数字
---

> **实验编号** Lab02 &nbsp;&nbsp; **预计耗时** 3h &nbsp;&nbsp; **对应参考实现** [`step_2/`](../../../step_2/) &nbsp;&nbsp; **本节产出** 生成正确的 token 序列

## 实验目的

完成本 lab 后，你应当能够：

1. 理解 lexer 的输入是字符流，输出是 token 流
2. 正确跳过空白
3. 扫描标识符或关键字
4. 扫描整数数字

## 主要修改文件

- `framework/student.c`

## 命令

```bash
make clean && make test
```

## 本章任务

1. 实现 `student_skip_whitespace`
2. 实现 `student_scan_identifier`
3. 实现 `student_scan_number`

## 思考题

1. 为什么 `if` 和 `sum` 都先按“像标识符”的规则扫描，再做关键字判定？
2. 如果不在 lexer 阶段跳过空白和注释，parser 会多承担什么复杂度？
