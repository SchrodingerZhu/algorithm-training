export module tree09;
import std;

export class Solution {
  template <class T> class RMQIndex {
    std::vector<std::vector<T>> min;

    constexpr static std::size_t ilog2(std::size_t length) {
      return std::numeric_limits<std::size_t>::digits -
             std::countl_zero(length) - 1;
    }

  public:
    RMQIndex(std::span<const T> whole) : min{} {
      min.push_back(std::ranges::to<std::vector>(whole));
      std::size_t bits = ilog2(whole.size());
      for (std::size_t i = 1; i <= bits; ++i) {
        std::size_t len = 1uz << i;
        min.emplace_back(whole.size() - len + 1);
        for (std::size_t j = 0; j < min.back().size(); ++j)
          min[i][j] = std::min(min[i - 1][j], min[i - 1][j + len / 2]);
      }
    }
    T query(std::size_t offset, std::size_t length) {
      std::size_t level = ilog2(length);
      std::size_t len = 1uz << level;
      return std::min(min[level][offset], min[level][offset + length - len]);
    }
  };

  struct DFSState {
    std::vector<bool> visited;
    std::vector<std::size_t> euler_tour;
    std::vector<std::size_t> first;
    std::vector<std::size_t> depth;
  };

  static void dfs(const std::vector<std::vector<int>> &children,
                  std::size_t current, std::size_t depth, DFSState &state) {
    if (!state.visited[current]) {
      state.visited[current] = true;
      state.depth[current] = depth;
      state.first[current] = state.euler_tour.size();
    }
    state.euler_tour.push_back(current);
    for (int child : children[current]) {
      dfs(children, child, depth + 1, state);
      state.euler_tour.push_back(current);
    }
  }

public:
  std::vector<int> lca(const std::vector<int> &parent,
                       const std::vector<std::array<int, 2>> &queries) {
    std::vector<std::vector<int>> children(parent.size());
    for (std::size_t i = 1; i < parent.size(); ++i)
      children[parent[i]].push_back(i);
    DFSState state{
        std::vector(parent.size(), false),
        {},
        std::vector(parent.size(), 0uz),
        std::vector(parent.size(), 0uz),
    };
    dfs(children, 0, 0, state);
    using DepthAndIndex = std::pair<std::size_t, std::size_t>;
    std::vector<DepthAndIndex> depth_map =
        state.euler_tour | std::views::transform([&](std::size_t node) {
          return std::make_pair(state.depth[node], node);
        }) |
        std::ranges::to<std::vector>();
    RMQIndex<DepthAndIndex> index(depth_map);
    return queries |
           std::views::transform([&](std::array<int, 2> nodes) -> int {
             std::size_t first_x = state.first[nodes[0]];
             std::size_t first_y = state.first[nodes[1]];
             if (first_x > first_y)
               std::swap(first_x, first_y);
             auto [_, lca_index] = index.query(first_x, first_y - first_x + 1);
             return lca_index;
           }) |
           std::ranges::to<std::vector>();
  }
};
