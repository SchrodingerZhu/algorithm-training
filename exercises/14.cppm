export module tree14;
import std;
import common.task;

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
  pathMinUpdates(const std::vector<int> &parent,
                 const std::vector<long long> &values,
                 const std::vector<std::array<long long, 3>> &operations) {
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
    for (auto [kind, x, y] : operations) {
      if (kind == 0) {
        Assignment chain_info = assignment[x];
        trees[chain_info.chain].update(chain_info.index, y);
        continue;
      }
      long long result = std::numeric_limits<long long>::max();
      while (assignment[x].chain != assignment[y].chain) {
        if (statistics[heads[assignment[x].chain]].depth <
            statistics[heads[assignment[y].chain]].depth)
          std::swap(x, y);

        Assignment info = assignment[x];
        result = std::min(result, trees[info.chain].query(0, info.index + 1));
        x = parent[heads[info.chain]];
      }
      auto [first, last] =
          std::minmax(assignment[x].index, assignment[y].index);
      result = std::min(
          result, trees[assignment[x].chain].query(first, last - first + 1));
      results.push_back(result);
    }
    return results;
  }
};
