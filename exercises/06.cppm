export module tree06;
import std;

export class Solution {
  class UnionFind {
    struct Node {
      std::size_t larger;
      std::size_t parent;
      std::size_t size;
    };
    std::vector<Node> nodes;

  public:
    UnionFind(std::size_t n) : nodes(n) {
      for (std::size_t i = 0; i < n; ++i) {
        nodes[i].larger = i;
        nodes[i].parent = i;
        nodes[i].size = 1;
      }
    }
    std::size_t find(std::size_t idx) {
      std::size_t cursor = idx;
      while (nodes[cursor].parent != cursor)
        cursor = nodes[cursor].parent;
      std::size_t root = cursor;
      cursor = idx;
      while (nodes[cursor].parent != root) {
        std::size_t update = cursor;
        cursor = nodes[cursor].parent;
        nodes[update].parent = root;
      }
      return root;
    }
    std::size_t find_min_idx(std::size_t idx) {
      return nodes[find(idx)].larger;
    }
    void link(std::size_t a, std::size_t b) {
      std::size_t pa = find(a);
      std::size_t pb = find(b);
      if (pb == pa)
        return;
      if (nodes[pa].size < nodes[pb].size)
        std::swap(pa, pb);
      nodes[pb].parent = pa;
      nodes[pa].size += nodes[pb].size;
      nodes[pa].larger = std::max(nodes[pa].larger, nodes[pb].larger);
    }
  };

public:
  std::vector<long long>
  rangeMin(const std::vector<long long> &values,
           const std::vector<std::array<int, 2>> &queries) {
    UnionFind uf(values.size() + 1);
    std::vector<std::vector<std::pair<int, std::size_t>>> bucket(values.size() +
                                                                 1);
    std::vector<long long> answer(queries.size());
    for (std::size_t i = 0; i < queries.size(); ++i) {
      auto [l, r] = queries[i];
      bucket[r].emplace_back(l, i);
    }
    std::stack<std::pair<std::size_t, long long>> state;
    for (std::size_t i = 0; i < values.size(); ++i) {
      long long current = values[i];
      // for positions before the current idx and has min value
      // that was larger than current element, the new min
      // position is now updated to current element
      while (!state.empty() && state.top().second > current) {
        std::size_t idx = state.top().first;
        state.pop();
        uf.link(idx, i);
      }
      state.emplace(i, current);
      for (auto [l, pos] : bucket[i]) {
        std::size_t min_idx = uf.find_min_idx(static_cast<std::size_t>(l));
        answer[pos] = values[min_idx];
      }
    }
    return answer;
  }
};
