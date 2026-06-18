---
title: 'Lab03 - step_3 语法分析与 AST'
layout: 'course_page'
permalink: '/course/practice/labs/lab03-step3/'
lab: 'Lab03'
step: 'step_3'
hours: '4h'
deliverable: '补完递归下降解析器中的关键结构节点'
source_path: 'course/practice/labs/lab03-step3/TASK.md'
---


> **实验编号** Lab03 &nbsp;&nbsp; **预计耗时** 4h &nbsp;&nbsp; **对应参考实现** [`step_3/`](https://github.com/Luyoung0001/minicompiler/tree/main/step_3) &nbsp;&nbsp; **本节产出** 让 token 流生长成一棵结构正确的 AST

## 本章任务

主要修改 `framework/student.c`，补完：

1. `student_parse_primary`
2. `student_parse_if`
3. `student_parse_function`

验证命令：

```bash
make clean && make test
```

## 思考题

1. 为什么表达式优先级会体现在 parser 的函数层次里？
2. 为什么 AST 要保留结构，而不是保留源码原样排版？
