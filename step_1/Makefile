# minicc - Mini C Compiler
# Step 1: 基础框架 Makefile

# 主机编译器 (编译 minicc 本身)
CC = gcc
CFLAGS = -Wall -Wextra -g -O2

# RISC-V 交叉编译工具链
CROSS_PREFIX ?= riscv64-unknown-linux-gnu-
RV_CC = $(CROSS_PREFIX)gcc
RV_AS = $(CROSS_PREFIX)as
RV_LD = $(CROSS_PREFIX)ld
RV_OBJDUMP = $(CROSS_PREFIX)objdump

# RISC-V 32 编译选项
RV_ARCH = -march=rv32i -mabi=ilp32

# QEMU (使用 system 模式需要额外配置，这里用 user 模式)
QEMU = qemu-riscv32

# 目标文件
MINICC = minicc
TEST_SRC = test.c
TEST_ASM = out.s
TEST_ELF = test.elf

.PHONY: all clean run test check-env help

all: $(MINICC)

# 编译 minicc 编译器
$(MINICC): minicc.c
	$(CC) $(CFLAGS) -o $@ $<

# 使用 minicc 编译测试程序
compile: $(MINICC) $(TEST_SRC)
	./$(MINICC) -v $(TEST_SRC) -o $(TEST_ASM)

# 将汇编代码编译成 RISC-V 可执行文件
$(TEST_ELF): $(TEST_ASM)
	$(RV_AS) $(RV_ARCH) -o test.o $(TEST_ASM)
	$(RV_LD) -m elf32lriscv -o $@ test.o

# 在 QEMU 上运行
run: $(TEST_ELF)
	$(QEMU) $(TEST_ELF)

# 完整测试流程
test: $(MINICC) $(TEST_SRC)
	@echo "=== Step 1: 使用 minicc 编译 ==="
	./$(MINICC) -v $(TEST_SRC) -o $(TEST_ASM)
	@echo ""
	@echo "=== Step 2: 汇编为 RISC-V 目标文件 ==="
	$(RV_AS) $(RV_ARCH) -o test.o $(TEST_ASM)
	@echo ""
	@echo "=== Step 3: 链接为可执行文件 ==="
	$(RV_LD) -o $(TEST_ELF) test.o
	@echo ""
	@echo "=== Step 4: 反汇编查看 ==="
	$(RV_OBJDUMP) -d $(TEST_ELF)
	@echo ""
	@echo "=== Step 5: 在 QEMU 上运行 ==="
	$(QEMU) $(TEST_ELF)

# 检查开发环境
check-env:
	@echo "检查开发环境..."
	@echo ""
	@echo "主机 GCC:"
	@$(CC) --version | head -1
	@echo ""
	@echo "RISC-V 工具链 ($(CROSS_PREFIX)):"
	@$(RV_CC) --version 2>/dev/null | head -1 || echo "  未安装 - 请运行: sudo apt install gcc-riscv64-linux-gnu"
	@echo ""
	@echo "QEMU:"
	@$(QEMU) --version 2>/dev/null | head -1 || echo "  未安装 - 请运行: sudo apt install qemu-user"

# 清理
clean:
	rm -f $(MINICC) $(TEST_ASM) $(TEST_ELF) test.o

# 帮助
help:
	@echo "minicc - Mini C Compiler (Step 1)"
	@echo ""
	@echo "使用方法:"
	@echo "  make            - 编译 minicc"
	@echo "  make compile    - 使用 minicc 编译 test.c"
	@echo "  make test       - 完整测试流程"
	@echo "  make run        - 在 QEMU 上运行"
	@echo "  make check-env  - 检查开发环境"
	@echo "  make clean      - 清理生成的文件"
	@echo ""
	@echo "环境变量:"
	@echo "  CROSS_PREFIX    - RISC-V 工具链前缀 (默认: riscv64-linux-gnu-)"
