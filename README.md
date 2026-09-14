# 筛法训练 · 从欧拉筛到莫比乌斯反演


先学一个原理，再动手修改它，最后把它用于计数题。每次推进一个小目标；今天从 [第一课](lessons/01.md) 开始。完整专题包含六关，不要求一次完成。

## 打开评测环境

```bash
cd /home/schrodingerzy/Documents/algorithm-training
nix develop
./judge smoke     # 环境检查：C++26 + import std + GoogleTest
./judge 01        # 完成 exercises/01.cppm 后评测第一关
```

也可以不进入交互 shell，直接运行 `nix develop --command ./judge smoke`。

环境沿用 `Documents/interview` 示例的 Nixpkgs 锁定版本、LLVM 22 / libc++、CMake toolchain 和模块接口写法，加入同一 libc++ 工具链编译的 GoogleTest。`flake.lock` 随项目保存；不依赖 interview 目录的存在。当前 flake 面向本机的 `x86_64-linux`。

每关只改一个 `exercises/NN.cppm`，保留模块名与 `Solution` 方法签名。不必写 `main`、输入解析或 GoogleTest；文件已能写 `import std;`。运行 `./judge 02` 等选择关卡，默认 `./judge` 等同于第一关。`./judge all` 会评测所有关卡，尚未实现的题会正常报 TODO 失败。环境检查通过不等于算法题完成。

习惯 CMake 的话，可用 `cmake --preset default`，然后 `cmake --build build --target stage_01` 和 `ctest --test-dir build -R '^Stage01$' --output-on-failure`。生成的 `build/compile_commands.json` 可供 clangd 使用。构建目录固定使用 Debug；第一次需构建标准库模块，后续增量编译。

## 学习顺序

| 关卡 | 先理解 | 再练习 | 讲义 |
|---|---|---|---|
| 01 | 质因数分解、埃氏筛、唯一生成、`break` 的位置 | 建立最小质因子表 | [欧拉筛](lessons/01.md) |
| 02 | “添一个新质因子”与“增加原有指数” | 同时统计两种质因子个数 | [改造筛法](lessons/02.md) |
| 03 | 互质、欧拉函数、积性与完全积性的区别 | 建立 φ 表 | [欧拉函数](lessons/03.md) |
| 04 | 质数幂、为什么当前统计值有时不够 | 约数个数与约数和 | [增加辅助状态](lessons/04.md) |
| 05 | 平方因子、容斥的符号 | 建立 μ 表 | [莫比乌斯筛](lessons/05.md) |
| 06 | 把 gcd 条件换成约数求和 | LeetCode 风格互质数对计数 | [从筛法到题目](lessons/06.md) |

第一、二关先用手算表格和转移找回思路。之后逐步隐藏底层筛法，用 `Solution` 的方法解决实际计数问题。每次一道主问题；讲义中的可选变式留有余力时再做，未列为关卡的口头变式没有自动评测。

所有题目均为本专题自编，接口风格类似 LeetCode，不冒用平台题号。测试覆盖小范围独立枚举、0/1 边界、质数幂、混合质因数、百万上界和必要的 64 位计数。测试能检查结果并排除明显过慢的实现，但不能代替线性复杂度证明。

第一至五关：`0 ≤ n ≤ 1,000,000`，输出数组长 `n + 1`。下标 0 是哨兵；`n = 0` 时不能访问下标 1。第六关：两个输入都在此范围内，空区间返回 0。同一个 `Solution` 对象可被重复调用，函数之间不能残留旧数据。

## 工具链说明

已锁定的版本为 Clang / libc++ 22.1.8、CMake 4.3.4 和 GoogleTest 1.17.0。项目要求真实的 `-std=c++26` 与标准库模块；这不表示编译器已实现所有 C++26 提案。CMake 的标准库模块支持仍有版本相关的实验开关，升级时需要同步维护。[CMake 文档](https://cmake.org/cmake/help/latest/prop_tgt/CXX_MODULE_STD.html)

toolchain 根据编译器查询头文件路径，并给出 libc++ 模块清单。与 interview 示例相同，构建时关闭会与标准库模块导出冲突的 fortify 包装。GoogleTest 头文件放在 `import` 之前；标准库 BMI 和使用它的代码统一带 `-pthread`。[libc++ 模块文档](https://libcxx.llvm.org/Modules.html)

如果离开 Nix 环境后重新配置失败，先执行 `nix develop`。更改工具链版本后应换一个干净的构建目录，例如 `SIEVE_BUILD_DIR="$PWD/build-new" ./judge smoke`。

## fish 与编辑器

shellHook 执行完会 `unset shellHook`，避免 fish 的 foreign-env 把多行 hook 中的 `export CXX` 等误读为变量名。如果旧 fish 会话仍保存它，可运行 `set -e shellHook`，然后重新进入 Nix 环境。

用 VS Code 打开本目录 `/home/schrodingerzy/Documents/algorithm-training`。项目保留了 Nix Env Selector 设置，并配置了 clangd 扩展与 `.cppm` 文件类型。`tools/clangd` 自动进入锁定的 Nix 环境，因而从桌面打开编辑器也能找到正确版本。其他支持 clangd 的编辑器也可将语言服务器指向这个脚本。

`.clangd` 指定 `build/compile_commands.json` 并显式添加 C++26 标志，避免 clangd 22 对 `.cppm` 丢失语言标准。模块支持已在启动脚本启用；构建生成模块与依赖信息后，运行 VS Code 命令 **clangd: Restart language server** 即可重新读取配置。[clangd 配置说明](https://clangd.llvm.org/config)

需要手动重建编辑器所需信息时运行：

```bash
nix develop
cmake --preset default
cmake --build --preset default
```

这些命令只编译，不执行尚未完成的算法测试。

现在打开 [第一课](lessons/01.md)。先解释 12 为什么交给 `6 × 2`，再写第一份筛法。

第二组树上查询课程位于本仓库的 `rmq-lca-lct` 分支。切换前保存修改，切换后执行 `nix develop` 和 `cmake --fresh --preset default`。
