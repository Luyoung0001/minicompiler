---
title: 'Lab04 - step_4 语义分析'
layout: 'course_page'
permalink: '/course/practice/labs/lab04-step4/'
lab: 'Lab04'
step: 'step_4'
hours: '4h'
deliverable: '用符号表支撑最小语义错误检测'
source_path: 'course/practice/labs/lab04-step4/TASK.md'
---


> **实验编号** Lab04 &nbsp;&nbsp; **预计耗时** 4h &nbsp;&nbsp; **对应参考实现** [`step_4/`](https://github.com/Luyoung0001/minicompiler/tree/main/step_4) &nbsp;&nbsp; **本节产出** 发现未定义变量和重复定义

## 本章任务

主要修改 `framework/student.c`，补完：

1. `student_symtab_add`
2. `student_symtab_lookup`
3. `student_semantic_analyze`

验证命令：

```bash
make clean && make test
```

## 思考题

1. 为什么“当前作用域重定义”和“父作用域同名遮蔽”不能混为一谈？
2. 为什么语义分析必须带着上下文遍历 AST？
