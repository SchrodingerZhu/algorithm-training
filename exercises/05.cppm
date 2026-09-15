export module tree05;
import std;

export class Solution {
  class Tree {
    struct Node {
      long long delta;
      long long min;
    };
    std::vector<Node> tree;
    std::size_t length;

    void push_down(std::size_t node, std::size_t node_length) {
      if (tree[node].delta == 0 || node_length == 0)
        return;
      tree[node].min += tree[node].delta;
      if (node_length > 1) {
        tree[2 * node + 1].delta += tree[node].delta;
        tree[2 * node + 2].delta += tree[node].delta;
      }
      tree[node].delta = 0;
    }

    template <class F>
    long long visit(std::size_t offset, std::size_t length, std::size_t node,
                    std::size_t node_offset, std::size_t node_length,
                    F &&visitor) {
      push_down(node, node_length);
      std::size_t end = offset + length;
      std::size_t node_end = node_offset + node_length;

      if (length == 0 || node_length == 0 || offset >= node_end ||
          end <= node_offset)
        return std::numeric_limits<long long>::max();

      if (offset <= node_offset && end >= node_end) {
        visitor(tree[node]);
        // parents need to read in-range node's updated value, push down
        // immediately at this level
        push_down(node, node_length);
        return tree[node].min;
      }

      std::size_t half = node_length / 2;
      long long res = std::min(
          visit(offset, length, node * 2 + 1, node_offset, half, visitor),
          visit(offset, length, node * 2 + 2, node_offset + half,
                node_length - half, visitor));
      tree[node].min = std::min(tree[2 * node + 1].min, tree[2 * node + 2].min);
      return res;
    }

    void update(std::size_t offset, std::size_t length, long long delta,
                std::size_t node, std::size_t node_offset,
                std::size_t node_length) {
      visit(offset, length, node, node_offset, node_length,
            [delta](Node &node) -> void { node.delta += delta; });
    }

    long long query(std::size_t offset, std::size_t length, std::size_t node,
                    std::size_t node_offset, std::size_t node_length) {
      return visit(offset, length, node, node_offset, node_length,
                   [](const auto &) {});
    }

  public:
    Tree(std::span<const long long> values)
        : tree(4 * values.size()), length(values.size()) {
      auto build = [this](this auto &&self, std::size_t index,
                          std::span<const long long> current) {
        if (current.size() == 0) {
          tree[index] = {0, std::numeric_limits<long long>::max()};
          return;
        }
        if (current.size() == 1) {
          tree[index] = {0, current[0]};
          return;
        }
        std::size_t half = current.size() / 2;
        std::span left = current.subspan(0, half);
        std::span right = current.subspan(half, current.size() - half);
        self(index * 2 + 1, left);
        self(index * 2 + 2, right);
        tree[index] = {
            0, std::min(tree[index * 2 + 1].min, tree[index * 2 + 2].min)};
      };
      build(0, values);
    }

    long long query(std::size_t offset, std::size_t length) {
      return query(offset, length, 0, 0, this->length);
    }

    void update(std::size_t offset, std::size_t length, long long delta) {
      return update(offset, length, delta, 0, 0, this->length);
    }
  };

public:
  std::vector<long long>
  rangeAddMin(const std::vector<long long> &values,
              const std::vector<std::array<long long, 4>> &operations) {
    Tree tree(values);
    std::vector<long long> results;

    for (const auto &[kind, l, r, delta] : operations) {
      if (kind == 0)
        tree.update(l, r - l + 1, delta);
      else
        results.push_back(tree.query(l, r - l + 1));
    }

    return results;
  }
};
