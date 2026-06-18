---
title: '附录 A3：参考资料与下一步'
layout: 'course_page'
permalink: '/course/appendices/A3-references/'
source_path: 'course/appendices/A3-references.md'
---

如果你已经走完整条主线，接下来最值得做的不是盲目加功能，而是带着课程里已经建立的直觉去读更完整的编译器项目。

## A3.1 先回读你已经完成的 lab

主线 lab 是有意缩小边界后的练习。如果你做完一章后觉得“我知道核心动作了，但还没有完全把前后关系串起来”，最适合先回读的就是自己刚完成的 lab：

- `miniCompiler_lab/labs/lab02-step2/framework/student.c`
- `miniCompiler_lab/labs/lab03-step3/framework/student.c`
- `miniCompiler_lab/labs/lab04-step4/framework/student.c`
- `miniCompiler_lab/labs/lab05-step5/framework/student.c`

回读时不要只看最终代码。更有效的方式是打开对应 `TASK.md`，把“本章目标 -> TODO 函数 -> 验证输出”重新连一遍。这样你能确认自己记住的不是某几行实现，而是这一章在整个编译器链路中的位置。

这时候再回看，会比一开始直接硬啃有效得多。

## A3.2 推荐继续看的方向

如果你想继续扩展 `minicc`，比较自然的下一步是：

1. 增加更多表达式和语句形式
2. 完整支持函数参数与调用约定
3. 增加更严谨的类型系统
4. 把错误恢复和错误定位做得更好
5. 引入中间表示，而不是直接从 AST 落到汇编

## A3.3 参考材料

- 《编译原理》相关教材，用来补系统化术语
- RISC-V calling convention 文档，用来补参数、返回值和栈帧规则
- 更完整的教学编译器项目，例如 `chibicc`、`8cc` 一类

但顺序很重要：先带着你在这门课里已经做过的代码经验去看，再去吸收这些材料，效果会比先背定义好很多。
