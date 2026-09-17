# 第二组：RMQ → 倍增与 LCA → 树链剖分 → LCT

这是独立于筛法的树与区间查询题组，共 18 道主线题，另有 10-a、10-b 两道扩展题。当前分支 `rmq-lca-lct`；筛法与原代码在 `sieves` 分支。先做 01，再逐题推进。已经熟练的基础题可通过测试与解释不变量快速跳过。

## 打开项目

```bash
cd /home/schrodingerzy/Documents/algorithm-training
nix develop
./judge smoke
./judge 01
```

第一题代码：`/home/schrodingerzy/Documents/algorithm-training/exercises/01.cppm`。
第一题讲义：`/home/schrodingerzy/Documents/algorithm-training/lessons/01.md`。

每题只修改对应 `.cppm` 的 Solution；接口完整给出，不需要 main 或输入输出。`./judge 02` 等选择题号，扩展题用 `./judge 10-a` 或 `./judge 10-b`；`./judge all` 运行本分支全部题目（包括扩展题）。待实现题显示 TODO 失败是正常现象。题目均为本题组自编，不冒用平台题号。

## 递进路线

| 题号 | 内容 | 类型 | 讲义 | 代码 |
|---|---|---|---|---|
| 01 | RMQ 基线：直接扫描 | 数组 | [题面与提示](lessons/01.md) | [练习](exercises/01.cppm) |
| 02 | Sparse Table：幂等 RMQ | 数组 | [题面与提示](lessons/02.md) | [练习](exercises/02.cppm) |
| 03 | 固定窗口 RMQ：单调队列 | 数组 | [题面与提示](lessons/03.md) | [练习](exercises/03.cppm) |
| 04 | 点修改与区间最小值 | 数组 | [题面与提示](lessons/04.md) | [练习](exercises/04.cppm) |
| 05 | 区间加与区间最小值 | 数组 | [题面与提示](lessons/05.md) | [练习](exercises/05.cppm) |
| 06 | 离线 RMQ：单调栈与并查集 | 数组 | [题面与提示](lessons/06.md) | [练习](exercises/06.cppm) |
| 07 | 倍增祖先表：第 k 个祖先 | 树 | [题面与提示](lessons/07.md) | [练习](exercises/07.cppm) |
| 08 | 二进制提升求 LCA | 树 | [题面与提示](lessons/08.md) | [练习](exercises/08.cppm) |
| 09 | Euler Tour 把 LCA 变为 RMQ | 树 | [题面与提示](lessons/09.md) | [练习](exercises/09.cppm) |
| 10 | Cartesian Tree：把 RMQ 变为 LCA | 树 | [题面与提示](lessons/10.md) | [练习](exercises/10.cppm) |
| 10-a | Cartesian Tree：直方图中的最大矩形 | 树与子树统计 | [题面与提示](lessons/10-a.md) | [练习](exercises/10-a.cppm) |
| 10-b | Cartesian Tree：所有子数组的最小值之和 | 树与贡献计数 | [题面与提示](lessons/10-b.md) | [练习](exercises/10-b.cppm) |
| 11 | 树上 RMQ：倍增聚合路径边最小值 | 树 | [题面与提示](lessons/11.md) | [练习](exercises/11.cppm) |
| 12 | 换根 LCA：不重建整棵树 | 树 | [题面与提示](lessons/12.md) | [练习](exercises/12.cppm) |
| 13 | 子树 RMQ：DFS 序与点修改 | 树 | [题面与提示](lessons/13.md) | [练习](exercises/13.cppm) |
| 14 | 树链剖分：路径最小点权 | HLD | [题面与提示](lessons/14.md) | [练习](exercises/14.cppm) |
| 15 | 树链剖分：路径加与路径和 | HLD | [题面与提示](lessons/15.md) | [练习](exercises/15.cppm) |
| 16 | LCT 前置：隐式 Splay 与区间翻转 | 动态树 | [题面与提示](lessons/16.md) | [练习](exercises/16.cppm) |
| 17 | LCT：动态森林连通性 | 动态树 | [题面与提示](lessons/17.md) | [练习](exercises/17.cppm) |
| 18 | LCT 综合：动态路径点权和 | 动态树 | [题面与提示](lessons/18.md) | [练习](exercises/18.cppm) |

## 这些变式分别练什么

- **数组、不需要 LCA**：01 扫描、02 ST 倍增、03 单调队列、04～05 线段树、06 离线并查集。既有通用 RMQ，也有利用窗口或离线条件的专用方法。
- **树与倍增**：07 存祖先函数复合；08 求 LCA；11 在祖先表上同时维护路径最小值。
- **RMQ 与 LCA 互相归约**：09 把 LCA 变成 Euler Tour 上的 RMQ；10 用 Cartesian Tree 把 RMQ 变成 LCA。二者不能与只记录一次的 DFS 序混淆。
- **直接使用笛卡尔树的子树**：10-a 用子树大小计算矩形面积；10-b 统计各节点对所有子数组最小值之和的贡献。两题均为 `O(n)`，无需 LCA 查询表。
- **固定树上的变化**：12 换根查询，13 子树修改查询，14～15 路径修改查询。13 无需 LCA；HLD 的跨链过程可直接处理路径，无需另外调用 LCA。
- **拓扑也会变化**：16 先练 Splay 旋转、排名和翻转标记；17 连边、断边与连通性；18 用 LCT 维护动态路径和。

这是一份逐步推进的题库，不是要求一次完成的作业。每题讲义先解释新维护量，再给题面、例子、目标复杂度和可展开的提示。16～18 建议拆成原语、组合操作、随机对拍三次练习。

## 基础约定与容易混淆的地方

区间默认闭区间；DFS 子树常用半开区间，转换时要减一。点权路径包含两个端点和 LCA 一次；边权路径不包含 LCA 的入边。ST 的两块覆盖可以重叠，是因为 min 幂等，不能直接改成 sum。倍增祖先表和 ST 形状相似，但转移含义不同。

测试包含小规模朴素对照、固定种子的随机操作、负值和重复值、单点、长链、星形树以及动态森林的无效连断操作。正确性评测不能强制你使用指定算法，也不能单独证明复杂度；需要结合题面目标复盘。

## 环境与分支

环境沿用已验证的 Nix、Clang/libc++ 22.1.8、C++26、import std、GoogleTest 和 clangd。保留 fish 的 `unset shellHook` 修复。当前 flake 支持 x86_64-linux。

保存或提交修改后，可以 `git switch sieves` 回到第一组，或 `git switch rmq-lca-lct` 回来。切换后先 `nix develop`，再 `cmake --fresh --preset default`；两分支复用 build 目录时需更新编译信息。不要在未保存修改时强制切换或清理文件。

VS Code 打开整个仓库；clangd 启动脚本会进入 Nix 环境。新建构建信息后执行 **clangd: Restart language server**。只编译当前题可运行 `cmake --build build --target stage_01`。

进一步阅读：[Sparse Table](https://oi-wiki.org/ds/sparse-table/)、[LCA](https://oi-wiki.org/graph/lca/)、[树链剖分](https://oi-wiki.org/graph/hld/)、[LCT](https://oi-wiki.org/ds/lct/)。先按本题组的接口与边界约定实现，再对照参考资料。
