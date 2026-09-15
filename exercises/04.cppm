export module tree04;
import std;

export class Solution {
  class Tree {
    std::vector<long long> tree;
    std::size_t length;

    long long query(std::size_t offset, std::size_t length, std::size_t node,
                    std::size_t node_offset, std::size_t node_length) {
      std::size_t end = offset + length;
      std::size_t node_end = node_offset + node_length;

      if (length == 0 || node_length == 0 || offset >= node_end ||
          end <= node_offset)
        return std::numeric_limits<long long>::max();

      if (offset <= node_offset && end >= node_end)
        return tree[node];

      std::size_t half = node_length / 2;
      return std::min(query(offset, length, node * 2 + 1, node_offset, half),
                      query(offset, length, node * 2 + 2, node_offset + half,
                            node_length - half));
    }

    void update(std::size_t offset, long long value, std::size_t node,
                std::size_t node_length) {
      if (offset >= node_length)
        return;

      if (node_length == 1) {
        tree[node] = value;
        return;
      }

      std::size_t half = node_length / 2;
      std::size_t left = node * 2 + 1;
      std::size_t right = node * 2 + 2;

      if (offset < half)
        update(offset, value, left, half);
      else
        update(offset - half, value, right, node_length - half);
      tree[node] = std::min(tree[left], tree[right]);
    }

  public:
    Tree(std::span<const long long> values)
        : tree(4 * values.size()), length(values.size()) {
      auto build = [this](this auto &&self, std::size_t index,
                          std::span<const long long> current) {
        if (current.size() == 0) {
          tree[index] = std::numeric_limits<long long>::max();
          return;
        }
        if (current.size() == 1) {
          tree[index] = current[0];
          return;
        }
        std::size_t half = current.size() / 2;
        std::span left = current.subspan(0, half);
        std::span right = current.subspan(half, current.size() - half);
        self(index * 2 + 1, left);
        self(index * 2 + 2, right);
        tree[index] = std::min(tree[index * 2 + 1], tree[index * 2 + 2]);
      };
      build(0, values);
    }

    long long query(std::size_t offset, std::size_t length) {
      return query(offset, length, 0, 0, this->length);
    }

    void update(std::size_t offset, long long value) {
      return update(offset, value, 0, this->length);
    }
  };

public:
  std::vector<long long>
  rangeMinUpdates(const std::vector<long long> &values,
                  const std::vector<std::array<long long, 3>> &operations) {
    Tree tree(values);
    std::vector<long long> results;

    for (const auto &[kind, l, r] : operations) {
      if (kind == 0)
        tree.update(l, r);
      else
        results.push_back(tree.query(l, r - l + 1));
    }

    return results;
  }
};
