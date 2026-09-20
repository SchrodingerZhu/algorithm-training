export module tree13;
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

  struct NodeState {
    std::optional<std::size_t> in = std::nullopt;
    std::optional<std::size_t> out = std::nullopt;
  };

  std::generator<std::size_t>
  dfs(std::vector<NodeState> &state,
      const std::vector<std::vector<std::size_t>> &out_edges, std::size_t node,
      std::size_t &counter) {
    if (!state[node].in) {
      state[node].in = counter;
      counter += 1;
      co_yield node;
    }
    for (std::size_t child : out_edges[node])
      co_yield std::ranges::elements_of(dfs(state, out_edges, child, counter));
    state[node].out = counter;
  }

public:
  std::vector<long long>
  subtreeMin(const std::vector<int> &parent,
             const std::vector<long long> &values,
             const std::vector<std::array<long long, 3>> &operations) {
    std::vector<std::vector<std::size_t>> out_edges(parent.size());
    for (std::size_t i = 1; i < parent.size(); ++i)
      out_edges[parent[i]].push_back(i);
    std::vector<NodeState> state(parent.size());
    std::size_t counter = 0;
    auto dfs_order =
        dfs(state, out_edges, 0, counter) | std::ranges::to<std::vector>();
    std::vector<long long> ordered_values =
        dfs_order |
        std::views::transform([&](std::size_t i) { return values[i]; }) |
        std::ranges::to<std::vector>();
    Tree tree(ordered_values);
    std::vector<long long> result;
    for (auto [kind, node, value] : operations) {
      if (kind == 0) {
        std::size_t index = *state[node].in;
        tree.update(index, value);
        continue;
      }
      std::size_t start = *state[node].in;
      std::size_t end = *state[node].out;
      result.push_back(tree.query(start, end - start));
    }
    return result;
  }
};
