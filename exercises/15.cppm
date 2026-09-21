export module tree15;
import std;
import common.task;

export class Solution {
  class Tree {
    struct Node {
      long long delta;
      long long sum;
    };
    std::vector<Node> tree;
    std::size_t length;

    void push_down(std::size_t node, std::size_t node_length) {
      if (tree[node].delta == 0 || node_length == 0)
        return;
      tree[node].sum += tree[node].delta * static_cast<long long>(node_length);
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
        return 0;

      if (offset <= node_offset && end >= node_end) {
        visitor(tree[node]);
        // parents need to read in-range node's updated value, push down
        // immediately at this level
        push_down(node, node_length);
        return tree[node].sum;
      }

      std::size_t half = node_length / 2;
      long long res =
          visit(offset, length, node * 2 + 1, node_offset, half, visitor) +
          visit(offset, length, node * 2 + 2, node_offset + half,
                node_length - half, visitor);
      tree[node].sum = tree[2 * node + 1].sum + tree[2 * node + 2].sum;
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
        tree[index] = {0, tree[index * 2 + 1].sum + tree[index * 2 + 2].sum};
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

  struct Statistic {
    std::size_t depth;
    std::size_t weight;
  };

  task<std::size_t>
  dfs_populate(std::span<Statistic> stats,
               std::span<const std::vector<std::size_t>> children,
               std::size_t node, std::size_t depth) {
    stats[node].depth = depth;
    std::size_t weight = 1;
    for (std::size_t child : children[node]) {
      weight += co_await dfs_populate(stats, children, child, depth + 1);
    }
    stats[node].weight = weight;
    co_return weight;
  }

  struct Assignment {
    std::size_t chain;
    std::size_t index;
  };

  task<std::monostate> dfs_decomposition(
      std::span<const long long> values, std::span<const Statistic> stats,
      std::span<const std::vector<std::size_t>> children, std::size_t node,
      std::span<Assignment> assignment, std::size_t chain,
      std::size_t index_in_chain, std::vector<std::vector<long long>> &chains,
      std::vector<std::size_t> &heads) {
    assignment[node] = {chain, index_in_chain};
    chains[chain].push_back(values[node]);
    auto max_iter = std::ranges::max_element(
        children[node], {}, [&](auto child) { return stats[child].weight; });
    if (max_iter != children[node].end())
      co_await dfs_decomposition(values, stats, children, *max_iter, assignment,
                                 chain, index_in_chain + 1, chains, heads);
    for (auto child : children[node]) {
      if (child == *max_iter)
        continue;
      std::size_t new_chain = chains.size();
      chains.emplace_back();
      heads.emplace_back(child);
      co_await dfs_decomposition(values, stats, children, child, assignment,
                                 new_chain, 0, chains, heads);
    }
    co_return {};
  }

public:
  std::vector<long long>
  pathAddSum(const std::vector<int> &parent,
             const std::vector<long long> &values,
             const std::vector<std::array<long long, 4>> &operations) {
    std::vector<std::vector<std::size_t>> out_edges(parent.size());
    std::vector<Statistic> statistics(parent.size());
    for (std::size_t i = 1; i < parent.size(); ++i)
      out_edges[parent[i]].push_back(i);
    dfs_populate(statistics, out_edges, 0, 0).run();
    std::vector<Assignment> assignment(parent.size());
    std::vector<std::vector<long long>> chains{{}};
    std::vector<std::size_t> heads{0};
    dfs_decomposition(values, statistics, out_edges, 0, assignment, 0, 0,
                      chains, heads)
        .run();
    std::vector<Tree> trees =
        chains | std::views::transform([](std::span<const long long> span) {
          return Tree(span);
        }) |
        std::ranges::to<std::vector>();
    std::vector<long long> results;
    for (auto [kind, x, y, delta] : operations) {
      long long result = 0;
      while (assignment[x].chain != assignment[y].chain) {
        if (statistics[heads[assignment[x].chain]].depth <
            statistics[heads[assignment[y].chain]].depth)
          std::swap(x, y);

        Assignment info = assignment[x];
        result += trees[info.chain].query(0, info.index + 1);
        if (delta != 0)
          trees[info.chain].update(0, info.index + 1, delta);
        x = parent[heads[info.chain]];
      }
      auto [first, last] =
          std::minmax(assignment[x].index, assignment[y].index);
      result += trees[assignment[x].chain].query(first, last - first + 1);
      if (delta != 0)
        trees[assignment[x].chain].update(first, last - first + 1, delta);
      if (kind == 1)
        results.push_back(result);
    }
    return results;
  }
};
