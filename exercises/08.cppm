export module tree08;
import std;

export class Solution {
  class Table {
    std::vector<std::vector<int>> lookup;
    std::vector<int> depth;

  public:
    Table(std::span<const int> parent)
        : lookup{std::ranges::to<std::vector>(parent)} {
      for (std::size_t i = 1; (1uz << i) <= parent.size(); i++) {
        std::size_t prev = i - 1;
        lookup.push_back(lookup[prev] |
                         std::views::transform([&](int current) -> int {
                           return current == -1 ? -1 : lookup[prev][current];
                         }) |
                         std::ranges::to<std::vector>());
      }
      auto compute_depth = [&](int i) {
        int depth = 0;
        for (int j = static_cast<int>(lookup.size()) - 1; j >= 0; j--) {
          if (lookup[j][i] != -1) {
            depth += 1 << j;
            i = lookup[j][i];
          }
        }
        return depth;
      };
      depth = std::views::iota(0, static_cast<int>(parent.size())) |
              std::views::transform(compute_depth) |
              std::ranges::to<std::vector>();
    }
    int get_parent(int x, int level) {
      if (static_cast<std::size_t>(level) >= lookup[0].size())
        return -1;
      int k = 0;
      while (level != 0 && x != -1) {
        if (level & 1)
          x = lookup[k][x];
        level >>= 1;
        k++;
      }
      return x;
    }
    int get_lca(int x, int y) {
      int dx = depth[x];
      int dy = depth[y];
      if (dx < dy) {
        std::swap(dx, dy);
        std::swap(x, y);
      }
      x = get_parent(x, dx - dy);
      if (x == y)
        return x;
      for (int j = static_cast<int>(lookup.size()) - 1; j >= 0; j--) {
        if (lookup[j][x] != lookup[j][y]) {
          x = lookup[j][x];
          y = lookup[j][y];
        }
      }
      return lookup[0][x];
    }
  };

public:
  std::vector<int> lca(const std::vector<int> &parent,
                       const std::vector<std::array<int, 2>> &queries) {
    Table table(parent);
    return queries | std::views::transform([&](std::array<int, 2> q) {
             return table.get_lca(q[0], q[1]);
           }) |
           std::ranges::to<std::vector>();
  }
};
