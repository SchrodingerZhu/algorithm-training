#pragma once
namespace oracle {
inline long long minimum(const std::vector<long long> &a, int l, int r) {
  return *std::min_element(a.begin() + l, a.begin() + r + 1);
}
inline std::vector<long long>
rangeMin(const std::vector<long long> &a,
         const std::vector<std::array<int, 2>> &q) {
  std::vector<long long> out;
  for (auto [l, r] : q)
    out.push_back(minimum(a, l, r));
  return out;
}
inline std::vector<int> parentTree(int n, std::mt19937 &rng) {
  std::vector<int> p(n, -1), perm(n);
  std::iota(perm.begin(), perm.end(), 0);
  std::shuffle(perm.begin() + 1, perm.end(), rng);
  for (int i = 1; i < n; ++i)
    p[perm[i]] = perm[rng() % i];
  return p;
}
inline int lca(const std::vector<int> &p, int u, int v) {
  std::vector<bool> seen(p.size());
  for (int x = u; x != -1; x = p[x])
    seen[x] = true;
  while (!seen[v])
    v = p[v];
  return v;
}
inline std::vector<int> path(const std::vector<int> &p, int u, int v) {
  int a = lca(p, u, v);
  std::vector<int> left, right;
  for (int x = u; x != a; x = p[x])
    left.push_back(x);
  left.push_back(a);
  for (int x = v; x != a; x = p[x])
    right.push_back(x);
  left.insert(left.end(), right.rbegin(), right.rend());
  return left;
}
inline std::vector<int> reroot(const std::vector<int> &p, int r) {
  std::vector<std::vector<int>> g(p.size());
  for (int i = 1; i < (int)p.size(); ++i) {
    g[i].push_back(p[i]);
    g[p[i]].push_back(i);
  }
  std::vector<int> result(p.size(), -2), queue{r};
  result[r] = -1;
  for (std::size_t k = 0; k < queue.size(); ++k)
    for (int v : g[queue[k]])
      if (result[v] == -2) {
        result[v] = queue[k];
        queue.push_back(v);
      }
  return result;
}
struct Forest {
  std::vector<std::set<int>> g;
  std::vector<long long> val;
  explicit Forest(std::vector<long long> v) : g(v.size()), val(std::move(v)) {}
  std::vector<int> path(int u, int v) const {
    std::vector<int> prev(g.size(), -1), queue{u};
    prev[u] = u;
    for (std::size_t i = 0; i < queue.size() && prev[v] == -1; ++i)
      for (int w : g[queue[i]])
        if (prev[w] == -1) {
          prev[w] = queue[i];
          queue.push_back(w);
        }
    if (prev[v] == -1)
      return {};
    std::vector<int> out{v};
    while (v != u) {
      v = prev[v];
      out.push_back(v);
    }
    return out;
  }
  void link(int u, int v) {
    if (path(u, v).empty()) {
      g[u].insert(v);
      g[v].insert(u);
    }
  }
  void cut(int u, int v) {
    g[u].erase(v);
    g[v].erase(u);
  }
  std::optional<long long> sum(int u, int v) const {
    auto route = path(u, v);
    if (route.empty())
      return std::nullopt;
    long long s = 0;
    for (int x : route)
      s += val[x];
    return s;
  }
};
} // namespace oracle
