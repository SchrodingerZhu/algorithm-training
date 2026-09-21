export module tree17;
import std;

export class Solution {
  struct Node {
    bool evert = false;
    Node *path_parent = nullptr;
    Node *parent = nullptr;
    Node *children[2] = {nullptr, nullptr};
  };
  static void push_down(Node *node) {
    if (node && node->evert) {
      std::swap(node->children[0], node->children[1]);
      if (node->children[0])
        node->children[0]->evert ^= true;
      if (node->children[1])
        node->children[1]->evert ^= true;
      node->evert = false;
    }
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

    // rotated to root of the path
    if (!grandparent) {
      pivot->path_parent = parent->path_parent;
      parent->path_parent = nullptr;
    }
  }
  static void splay(Node *node) {
    while (node->parent) {
      if (node->parent->parent == nullptr) {
        push_down(node->parent);
        push_down(node);
        rotate(node);
        break;
      }
      Node *parent = node->parent;
      Node *grandparent = parent->parent;
      push_down(grandparent);
      push_down(parent);
      push_down(node);
      std::size_t node_dir = direction(node);
      std::size_t parent_dir = direction(parent);
      rotate(node_dir == parent_dir ? parent : node);
      rotate(node);
    }
    push_down(node);
  }
  static void expose(Node *node) {
    while (true) {
      splay(node);
      // make node the deepest one in its own tree
      if (node->children[1]) {
        node->children[1]->parent = nullptr;
        node->children[1]->path_parent = node;
        node->children[1] = nullptr;
      }
      if (!node->path_parent)
        return;
      Node *path_parent = node->path_parent;
      splay(path_parent);
      if (path_parent->children[1]) {
        path_parent->children[1]->parent = nullptr;
        path_parent->children[1]->path_parent = path_parent;
      }
      path_parent->children[1] = node;
      node->path_parent = nullptr;
      node->parent = path_parent;
    }
  }
  static void make_root(Node *node) {
    expose(node);
    node->evert ^= 1;
  }
  static bool is_connected(Node *u, Node *v) {
    if (u == v)
      return true;
    make_root(u);
    expose(v);
    Node *cursor = v;
    while (true) {
      if (cursor == u)
        break;
      push_down(cursor);
      if (!cursor->children[0])
        break;
      cursor = cursor->children[0];
    }
    splay(cursor);
    return cursor == u;
  }
  static void connect(Node *u, Node *v) {
    if (is_connected(u, v))
      return;
    make_root(v);
    v->path_parent = u;
  }
  static void disconnect(Node *u, Node *v) {
    if (!is_connected(u, v))
      return;
    // after is_connected, if there is an edge between
    // u and v, v must be u's immediately right child
    push_down(u);
    if (u->children[1] != v)
      return;

    // however, v must have no left subtree
    push_down(v);
    if (v->children[0])
      return;

    u->children[1] = nullptr;
    v->parent = nullptr;
  }

public:
  std::vector<int>
  dynamicConnectivity(int n,
                      const std::vector<std::array<int, 3>> &operations) {
    std::vector<Node> nodes(n);
    std::vector<int> res;
    for (auto [kind, u, v] : operations) {
      switch (kind) {
      case 0:
        connect(&nodes[u], &nodes[v]);
        break;
      case 1:
        disconnect(&nodes[u], &nodes[v]);
        break;
      case 2:
        res.push_back(is_connected(&nodes[u], &nodes[v]));
        break;
      default:
        std::abort();
      }
    }
    return res;
  }
};
