# minicc - Mini C Compiler Project
# 主 Makefile

# 当前步骤
CURRENT_STEP = step_5

.PHONY: all build test clean check-env install-deps help

# 默认目标: 构建当前步骤
all: build

# 构建当前步骤
build:
	$(MAKE) -C $(CURRENT_STEP)

# 测试当前步骤
test:
	$(MAKE) -C $(CURRENT_STEP) test

# 运行当前步骤
run:
	$(MAKE) -C $(CURRENT_STEP) run

# 检查环境
check-env:
	$(MAKE) -C $(CURRENT_STEP) check-env

# 安装依赖 (需要 sudo)
install-deps:
	@echo "安装 RISC-V 工具链和 QEMU..."
	sudo apt update
	sudo apt install -y gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu
	sudo apt install -y qemu-user qemu-user-static
	@echo ""
	@echo "安装完成! 运行 'make check-env' 验证"

# 清理所有步骤
clean:
	for dir in step_*; do \
		if [ -d "$$dir" ]; then \
			$(MAKE) -C "$$dir" clean; \
		fi \
	done

# 帮助
help:
	@echo "minicc - Mini C Compiler Project"
	@echo ""
	@echo "目标:"
	@echo "  make             - 构建当前步骤 ($(CURRENT_STEP))"
	@echo "  make test        - 测试当前步骤"
	@echo "  make run         - 运行当前步骤"
	@echo "  make check-env   - 检查开发环境"
	@echo "  make install-deps - 安装依赖 (需要 sudo)"
	@echo "  make clean       - 清理所有生成文件"
	@echo ""
	@echo "项目结构:"
	@echo "  step_1/  - 环境搭建与基础框架"
	@echo "  step_2/  - 词法分析器 (待实现)"
	@echo "  step_3/  - 语法分析器 (待实现)"
	@echo "  ..."
