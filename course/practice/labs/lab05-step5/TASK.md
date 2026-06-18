---
title: Lab05 - step_5 代码生成
lab: Lab05
step: step_5
hours: 5h
deliverable: 把最小表达式和变量访问变成 RISC-V 汇编
---

> **实验编号** Lab05 &nbsp;&nbsp; **预计耗时** 5h &nbsp;&nbsp; **对应参考实现** [`step_5/`](../../../step_5/) &nbsp;&nbsp; **本节产出** 生成可运行的最小 RISC-V 汇编片段

## 本章任务

主要修改 `framework/student.c`，补完：

1. `student_get_var_offset`
2. `student_gen_expr`
3. `student_gen_stmt`

验证命令：

```bash
make clean && make test
```

## 思考题

1. 为什么表达式代码生成离不开临时压栈？
2. 为什么 `if` / `while` 最终一定会落成标签和跳转？
