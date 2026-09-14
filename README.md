# Algorithm training

两个独立题组共用锁定的 Nix、C++26、import std、GoogleTest 与 clangd 环境。

- `git switch sieves`：第一组筛法课程与已保存代码。
- `git switch rmq-lca-lct`：第二组 RMQ、倍增、LCA、树链剖分与 LCT。

切换分支前保存并提交自己的修改。切换后进入 `nix develop`，执行 `cmake --fresh --preset default` 更新构建信息。
