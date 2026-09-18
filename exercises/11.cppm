export module tree11;
import std;

export class Solution {
  class Table {
    struct Statistic {
      int parent;
      long long min;
    };
    std::vector<std::vector<Statistic>> lookup;
    std::vector<int> depth;

  public:
    Table(std::span<const int> parent, std::span<const long long> weights)
        : lookup{std::views::zip(parent, weights) |
                 std::views::transform([](auto pair) {
                   return Statistic{std::get<0>(pair), std::get<1>(pair)};
                 }) |
                 std::ranges::to<std::vector>()} {
      for (std::size_t i = 1; (1uz << i) <= parent.size(); i++) {
        std::size_t prev = i - 1;
        lookup.push_back(
            lookup[prev] |
            std::views::transform([&](Statistic current) -> Statistic {
              if (current.parent == -1)
                return current;
              Statistic parent_stat = lookup[prev][current.parent];
              return {parent_stat.parent,
                      std::min(current.min, parent_stat.min)};
            }) |
            std::ranges::to<std::vector>());
      }
      auto compute_depth = [&](int i) {
        int depth = 0;
        for (int j = static_cast<int>(lookup.size()) - 1; j >= 0; j--) {
          if (lookup[j][i].parent != -1) {
            depth += 1 << j;
            i = lookup[j][i].parent;
          }
        }
        return depth;
      };
      depth = std::views::iota(0, static_cast<int>(parent.size())) |
              std::views::transform(compute_depth) |
              std::ranges::to<std::vector>();
    }
    Statistic get_parent(int x, int level) {
      long long min = std::numeric_limits<long long>::max();
      int k = 0;
      while (level != 0 && x != -1 &&
             static_cast<std::size_t>(level) < lookup[0].size()) {
        if (level & 1) {
          min = std::min(min, lookup[k][x].min);
          x = lookup[k][x].parent;
        }
        level >>= 1;
        k++;
      }
      return {x, min};
    }
    Statistic get_lca(int x, int y) {
      if (x == y)
        return {x, 0};
      int dx = depth[x];
      int dy = depth[y];
      if (dx < dy) {
        std::swap(dx, dy);
        std::swap(x, y);
      }
      Statistic stat = get_parent(x, dx - dy);
      x = stat.parent;
      if (x == y)
        return stat;
      for (int j = static_cast<int>(lookup.size()) - 1; j >= 0; j--) {
        if (lookup[j][x].parent != lookup[j][y].parent) {
          stat.min =
              std::ranges::min({stat.min, lookup[j][x].min, lookup[j][y].min});
          x = lookup[j][x].parent;
          y = lookup[j][y].parent;
        }
      }
      stat.parent = lookup[0][x].parent;
      stat.min =
          std::ranges::min({stat.min, lookup[0][x].min, lookup[0][y].min});
      return stat;
    }
  };

public:
  std::vector<long long>
  pathMin(const std::vector<int> &parent,
          const std::vector<long long> &edgeWeight,
          const std::vector<std::array<int, 2>> &queries) {
    Table table{parent, edgeWeight};
    return queries | std::views::transform([&](auto q) {
             auto [x, y] = q;
             return table.get_lca(x, y).min;
           }) |
           std::ranges::to<std::vector>();
  }
};
