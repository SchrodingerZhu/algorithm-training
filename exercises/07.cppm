export module tree07;
import std;

export class Solution {
  class Table {
    std::vector<std::vector<int>> lookup;

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
  };

public:
  std::vector<int>
  kthAncestors(const std::vector<int> &parent,
               const std::vector<std::array<long long, 2>> &queries) {
    Table table(parent);
    return queries | std::views::transform([&](std::array<long long, 2> q) {
             return table.get_parent(q[0], q[1]);
           }) |
           std::ranges::to<std::vector>();
  }
};
