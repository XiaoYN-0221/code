# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 工具链

- **编译器**: MinGW-w64, `D:\mingw64\bin\g++.exe`
- **调试器**: GDB, `D:\mingw64\bin\gdb.exe`
- **C++ 标准**: C++17
- **构建系统**: 无（VS Code tasks 直调 g++ 编译当前活动 `.cpp` 文件）

## 项目结构

```
D:\code\
├── c++/               # 单文件 C++ 示例（main.cpp, binary_tree.cpp, pi.cpp）
├── least_squares/     # 独立 C++ 程序
├── .vscode/           # VS Code 配置（tasks, launch, settings, Arduino）
├── .claude/           # Claude Code 内部配置和 memory
└── .playwright-mcp/   # Playwright MCP 会话数据
```

仓库没有统一的构建系统。每个 `.cpp` 文件是独立的单文件程序，编译产出同名 `.exe`。

## 编译与运行

**VS Code**: `Ctrl+Shift+B` 触发默认 build task，编译当前活动 `.cpp` 并在同目录生成 `.exe`。
`Ctrl+Shift+P` → "Run Test Task" 可用 "RUN C++" task，先编译再执行。

**手动编译**:
```bash
g++ -g -std=c++17 -fdiagnostics-color=always <file>.cpp -o <file>.exe
```

## Arduino

Arduino 连接 COM3，通过 VS Code Arduino 扩展使用。配置在 `.vscode/arduino.json`（仅端口号）。

## Memory

项目相关的持久记忆存在 `.claude/projects/D--code/memory/` 目录，`MEMORY.md` 是索引。包含用户偏好和工作流约定。
