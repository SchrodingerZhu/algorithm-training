module;

export module tree10;
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

  class CartesianTree {
    struct Node {
      std::size_t index = 0;
      std::size_t parent = std::numeric_limits<std::size_t>::max();
      std::size_t children[2] = {
          std::numeric_limits<std::size_t>::max(),
          std::numeric_limits<std::size_t>::max(),
      };
    };

    std::span<const long long> values;
    std::vector<Node> nodes;
    std::size_t root;

    struct Item {
      std::size_t element_index;
      long long value;
    };

  public:
    CartesianTree(std::span<const long long> values)
        : values(values), nodes(std::views::iota(0uz, values.size()) |
                                std::views::transform(
                                    [](std::size_t idx) { return Node{idx}; }) |
                                std::ranges::to<std::vector>()) {
      std::vector<std::size_t> state;
      state.push_back(0);
      for (std::size_t i = 1; i < values.size(); ++i) {
        long long value = values[i];
        // pop out all previous value that is larger than current node, they
        // form a path 2 4 6   -> 1
        //                       /
        //                      2
        //                       \
        //                        4
        //                         \
        //                          6
        std::size_t last = std::numeric_limits<std::size_t>::max();
        while (!state.empty() && value < values[state.back()]) {
          std::size_t top_idx = state.back();
          state.pop_back();
          if (last != std::numeric_limits<std::size_t>::max()) {
            nodes[last].parent = top_idx;
            nodes[top_idx].children[1] = last;
          }
          last = top_idx;
        }
        if (last != std::numeric_limits<std::size_t>::max()) {
          nodes[last].parent = i;
          nodes[i].children[0] = last;
        }
        state.push_back(i); // when push in, the right child is always unset
      }
      // finaly, chain the path in remaining stack
      root = state[0];
      for (std::size_t i = 1; i < state.size(); ++i) {
        nodes[state[i - 1]].children[1] = state[i];
        nodes[state[i]].parent = state[i - 1];
      }
    }

    std::vector<int> get_parents() const {
      return nodes | std::views::transform([](const Node &node) -> int {
               return node.parent == std::numeric_limits<std::size_t>::max()
                          ? -1
                          : static_cast<int>(node.parent);
             }) |
             std::ranges::to<std::vector>();
    }
  };

public:
  std::vector<int> argMin(const std::vector<long long> &values,
                          const std::vector<std::array<int, 2>> &queries) {
    CartesianTree tree(values);
    Table table(tree.get_parents());
    return queries | std::views::transform([&](std::array<int, 2> q) {
             return table.get_lca(q[0], q[1]);
           }) |
           std::ranges::to<std::vector>();
  }
};
