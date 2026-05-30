# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 工具链

- **编译器**: MinGW-w64, `D:\mingw64\bin\g++.exe`
- **调试器**: GDB, `D:\mingw64\bin\gdb.exe`
- **C++ 标准**: C++17
- **构建系统**: 无（VS Code tasks 直调 g++ 编译单文件）

## 项目结构

```
D:\code\
├── c++/           # C++ 源码，按语言分目录
├── .vscode/       # VS Code 配置（tasks、launch、Arduino）
```

## 编译与运行

在 VS Code 中通过 `Ctrl+Shift+B` 触发默认 build task，会编译当前活动 `.cpp` 文件并在同目录生成同名 `.exe`。

也可以手动：
```bash
g++ -g -fdiagnostics-color=always <file>.cpp -o <file>.exe
```

用于快速运行的 "RUN C++" task 会先编译再执行。

## Arduino

Arduino 连接在 COM3，通过 VS Code 的 Arduino 扩展使用。配置文件在 `.vscode/arduino.json`。
