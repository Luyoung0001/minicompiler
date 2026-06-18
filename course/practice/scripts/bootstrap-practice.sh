#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PRACTICE_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
LAB_ROOT="${PRACTICE_ROOT}/labs/lab01-step1"

check_cmd() {
  local cmd="$1"
  local hint="$2"
  if ! command -v "$cmd" >/dev/null 2>&1; then
    echo "[ERROR] 缺少命令: $cmd"
    echo "        $hint"
    exit 1
  fi
}

echo "=== minicompiler practice bootstrap ==="
check_cmd gcc "请先安装主机编译器 gcc"
check_cmd riscv64-unknown-linux-gnu-gcc "请先安装 RISC-V 交叉工具链"
check_cmd qemu-riscv32 "请先安装 qemu-user / qemu-riscv32"

echo "[OK] gcc / riscv64-unknown-linux-gnu-gcc / qemu-riscv32 已找到"
echo
echo "=== 进入 Lab01 baseline ==="
cd "${LAB_ROOT}"
make clean
make test
