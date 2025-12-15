# minicc - Mini C Compiler
# Step 5: 代码生成 Makefile

CC = gcc
CFLAGS = -Wall -Wextra -g -O2

# RISC-V 交叉编译工具链
CROSS_PREFIX ?= riscv64-unknown-linux-gnu-
RV_AS = $(CROSS_PREFIX)as
RV_LD = $(CROSS_PREFIX)ld
RV_OBJDUMP = $(CROSS_PREFIX)objdump

RV_ARCH = -march=rv32im -mabi=ilp32

QEMU = qemu-riscv32

# 源文件
SRCS = minicc.c lexer.c ast.c parser.c symbol.c semantic.c codegen.c
OBJS = $(SRCS:.c=.o)
MINICC = minicc

TEST_SRC = test.c
TEST_ASM = out.s
TEST_ELF = test.elf

.PHONY: all clean test tokens ast run help test-error

all: $(MINICC)

# 编译 minicc
$(MINICC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# 依赖关系
minicc.o: minicc.c lexer.h parser.h ast.h semantic.h codegen.h token.h
lexer.o: lexer.c lexer.h token.h
ast.o: ast.c ast.h token.h
parser.o: parser.c parser.h lexer.h ast.h token.h
symbol.o: symbol.c symbol.h
semantic.o: semantic.c semantic.h ast.h symbol.h token.h
codegen.o: codegen.c codegen.h ast.h symbol.h token.h

# 输出 token 列表
tokens: $(MINICC) $(TEST_SRC)
	./$(MINICC) --tokens $(TEST_SRC)

# 输出 AST
ast: $(MINICC) $(TEST_SRC)
	./$(MINICC) --ast $(TEST_SRC)

# 编译测试程序
compile: $(MINICC) $(TEST_SRC)
	./$(MINICC) -v $(TEST_SRC) -o $(TEST_ASM)

# 生成可执行文件
$(TEST_ELF): $(TEST_ASM)
	$(RV_AS) $(RV_ARCH) -o test.o $(TEST_ASM)
	$(RV_LD) -m elf32lriscv -o $@ test.o

# 运行
run: $(TEST_ELF)
	@$(QEMU) $(TEST_ELF); echo "程序退出码: $$?"

# 完整测试
test: $(MINICC)
	@echo "=== 测试代码生成 ==="
	./$(MINICC) -v $(TEST_SRC) -o $(TEST_ASM)
	@echo ""
	@echo "=== 生成的汇编代码 ==="
	@cat $(TEST_ASM)

clean:
	rm -f $(MINICC) $(OBJS) $(TEST_ASM) $(TEST_ELF) test.o

help:
	@echo "minicc - Step 5: 代码生成"
	@echo ""
	@echo "使用方法:"
	@echo "  make          - 编译 minicc"
	@echo "  make tokens   - 输出 test.c 的 token 列表"
	@echo "  make ast      - 输出 test.c 的抽象语法树"
	@echo "  make compile  - 编译 test.c 生成汇编"
	@echo "  make run      - 编译并在 QEMU 上运行"
	@echo "  make test     - 运行完整测试"
	@echo "  make clean    - 清理"
