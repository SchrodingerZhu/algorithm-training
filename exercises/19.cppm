export module tree19;
import std;

export class Solution {
  struct Node {
    std::size_t weight = 1;
    Node *parent = nullptr;
    Node *children[2] = {nullptr, nullptr};
  };
  static void maintain_invariant(Node *node) {
    if (!node)
      return;
    node->weight = 1;
    if (node->children[0])
      node->weight += node->children[0]->weight;
    if (node->children[1])
      node->weight += node->children[1]->weight;
  }
  static std::size_t direction(Node *p) {
    Node *parent = p->parent;
    return parent->children[0] == p ? 0 : 1;
  }
  static void rotate(Node *pivot) {
    if (!pivot)
      throw std::runtime_error("pivot must be nonnull");
    if (!pivot->parent)
      throw std::runtime_error("pivot's parent must be nonnull");

    Node *parent = pivot->parent;
    Node *grandparent = parent->parent;
    std::size_t side = direction(pivot);
    std::size_t child_side = 1 - side;
    Node *child = pivot->children[child_side];

    // update child <-> parent
    if (child)
      child->parent = parent;
    parent->children[side] = child;

    // update pivot <-> grandparent
    pivot->parent = grandparent;
    if (grandparent) {
      std::size_t parent_side = direction(parent);
      grandparent->children[parent_side] = pivot;
    }

    // update pivot <-> parent
    parent->parent = pivot;
    pivot->children[child_side] = parent;

    maintain_invariant(parent);
    maintain_invariant(pivot);
  }
  static void splay(Node *node, Node *root_parent = nullptr) {
    while (node->parent != root_parent) {
      if (node->parent->parent == root_parent) {
        rotate(node);
        break;
      }
      Node *parent = node->parent;
      std::size_t node_dir = direction(node);
      std::size_t parent_dir = direction(parent);
      rotate(node_dir == parent_dir ? parent : node);
      rotate(node);
    }
  }
  static void make_root(Node *node) {
    splay(node);
    Node *left = node->children[0];
    if (left) {
      node->children[0] = nullptr;
      maintain_invariant(node);
      Node *max = node;
      while (max->children[1])
        max = max->children[1];
      splay(max);
      max->children[1] = left;
      left->parent = max;
      maintain_invariant(max);
    }
  }
  static bool is_connected(Node *u, Node *v) {
    make_root(u);
    splay(v);
    Node *min = v;
    while (min->children[0])
      min = min->children[0];
    splay(min);
    return min == u;
  }

  using EdgeHandle = std::tuple<Node *, Node *, Node *>;

  static std::optional<EdgeHandle> link(Node *u, Node *v) {
    if (is_connected(u, v))
      return std::nullopt;
    make_root(v);
    splay(u);
    splay(v);
    Node *uv = new Node{};
    Node *vu = new Node{};
    Node *right = u->children[1];
    u->children[1] = vu;
    vu->parent = u;
    v->parent = vu;
    vu->children[0] = v;
    if (right) {
      right->parent = uv;
      uv->children[0] = right;
    }
    uv->parent = v;
    v->children[0] = uv;
    maintain_invariant(uv);
    maintain_invariant(v);
    maintain_invariant(vu);
    maintain_invariant(u);
    return std::make_optional<EdgeHandle>(u, uv, vu);
  }

  static void cut(EdgeHandle edge) {
    auto [u, uv, vu] = edge;
    make_root(u);
    splay(uv);
    Node *left = uv->children[0];
    while (left->children[1])
      left = left->children[1];
    splay(left, uv);
    splay(vu, uv);
    Node *v = vu->children[0];
    v->parent = nullptr;
    left->parent = nullptr;
    left->children[1] = vu->children[1];
    if (left->children[1])
      left->children[1]->parent = left;
    maintain_invariant(left);
    delete uv;
    delete vu;
  }

  class Graph {
    std::vector<Node> nodes;
    std::vector<std::unordered_map<std::size_t, EdgeHandle>> handles;

  public:
    ~Graph() {
      for (auto [_, b] : handles | std::views::join) {
        delete std::get<1>(b);
        delete std::get<2>(b);
      }
    }
    Graph(std::size_t n) : nodes(n), handles(n) {}
    void link(std::size_t u, std::size_t v) {
      if (std::optional<EdgeHandle> handle =
              Solution::link(&nodes[u], &nodes[v]))
        handles[u][v] = *handle;
    }
    void cut(std::size_t u, std::size_t v) {
      if (auto iter = handles[u].find(v); iter != handles[u].end()) {
        Solution::cut(iter->second);
        handles[u].erase(iter);
      }
      if (auto iter = handles[v].find(u); iter != handles[v].end()) {
        Solution::cut(iter->second);
        handles[v].erase(iter);
      }
    }

    Graph(const Graph &) = delete;
    Graph &operator=(const Graph &) = delete;
    bool is_connected(std::size_t u, std::size_t v) {
      return Solution::is_connected(&nodes[u], &nodes[v]);
    }
  };

public:
  std::vector<int>
  dynamicConnectivity(int n,
                      const std::vector<std::array<int, 3>> &operations) {
    Graph graph(n);
    std::vector<int> res;
    for (auto [kind, u, v] : operations) {
      switch (kind) {
      case 0:
        graph.link(u, v);
        break;
      case 1:
        graph.cut(u, v);
        break;
      case 2:
        res.push_back(graph.is_connected(u, v));
        break;
      default:
        std::abort();
      }
    }
    return res;
  }
};
