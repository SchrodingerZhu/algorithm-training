export module tree10b;
import std;

export class Solution {
  class CartesianTree {
    struct Node {
      std::size_t index = 0;
      std::size_t parent = std::numeric_limits<std::size_t>::max();
      std::size_t children[2] = {
          std::numeric_limits<std::size_t>::max(),
          std::numeric_limits<std::size_t>::max(),
      };
      std::size_t weight = 1;
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
            nodes[top_idx].weight += nodes[last].weight;
          }
          last = top_idx;
        }
        if (last != std::numeric_limits<std::size_t>::max()) {
          nodes[last].parent = i;
          nodes[i].children[0] = last;
          nodes[i].weight += nodes[last].weight;
        }
        state.push_back(i); // when push in, the right child is always unset
      }
      // finaly, chain the path in remaining stack
      root = state[0];
      for (std::size_t i = state.size() - 1; i > 0; --i) {
        nodes[state[i - 1]].children[1] = state[i];
        nodes[state[i]].parent = state[i - 1];
        nodes[state[i - 1]].weight += nodes[state[i]].weight;
      }
    }

    long long sumSubarrayMinimums() {
      auto weight = [&](std::size_t child) -> long long {
        return child == std::numeric_limits<std::size_t>::max()
                   ? 0LL
                   : static_cast<long long>(nodes[child].weight);
      };
      auto contributions = nodes | std::views::transform([&](const Node &node) {
        return values[node.index] * (weight(node.children[0]) + 1) *
               (weight(node.children[1]) + 1);
      });
      return std::ranges::fold_left(contributions, 0LL, std::plus{});
    }
  };

public:
  long long sumSubarrayMinimums(const std::vector<long long> &values) {
    CartesianTree tree(values);
    return tree.sumSubarrayMinimums();
  }
};
