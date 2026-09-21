module;
#include <cassert>

export module tree16;
import std;
import common.task;

export class Solution {
  struct Node {
    long long value;
    long long sum;
    std::size_t count;
    bool reversed;
    Node *parent;
    Node *children[2];
  };

  //    A
  //   / \
  //  P  B
  // /\
  // C D
  //     P
  //    / \
  //  C    A
  //     / \
  //    D    B   -> Give children in the reverse direction to parent, then
  //    become parent's parent
  static void push_down(Node *node) {
    if (node && node->reversed) {
      std::swap(node->children[0], node->children[1]);
      if (node->children[0])
        node->children[0]->reversed ^= true;
      if (node->children[1])
        node->children[1]->reversed ^= true;
      node->reversed = false;
    }
  }
  static void maintain_invariant(Node *node) {
    if (!node)
      return;
    node->sum = node->value;
    node->count = 1;
    if (node->children[0]) {
      node->sum += node->children[0]->sum;
      node->count += node->children[0]->count;
    }
    if (node->children[1]) {
      node->sum += node->children[1]->sum;
      node->count += node->children[1]->count;
    }
  }
  static std::size_t direction(Node *p) {
    Node *parent = p->parent;
    return parent->children[0] == p ? 0 : 1;
  }
  static void rotate(Node *&root, Node *pivot) {
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

    if (!grandparent)
      root = pivot;
  }
  static void splay(Node *&root, Node *node) {
    while (node != root) {
      if (node->parent == root) {
        push_down(root);
        push_down(node);
        rotate(root, node);
        break;
      }
      Node *parent = node->parent;
      Node *grandparent = parent->parent;
      push_down(grandparent);
      push_down(parent);
      push_down(node);
      std::size_t node_dir = direction(node);
      std::size_t parent_dir = direction(parent);
      rotate(root, node_dir == parent_dir ? parent : node);
      rotate(root, node);
    }
  }

  static std::size_t count(Node *node) { return node ? node->count : 0; }

  class SplayTree {
    Node *root;
    static task<Node *> construct(Node *parent,
                                  std::span<const long long> values) {
      if (values.empty())
        co_return nullptr;
      if (values.size() == 1)
        co_return new Node{values[0], values[0], 1,
                           false,     parent,    {nullptr, nullptr}};
      std::size_t half = values.size() / 2;
      std::span<const long long> left = values.subspan(0, half);
      long long mid = values[half];
      std::span<const long long> right =
          values.subspan(half + 1, values.size() - half - 1);
      Node *node = new Node{mid, mid, 1, false, parent, {nullptr, nullptr}};
      Node *lchild = co_await construct(node, left);
      Node *rchild = co_await construct(node, right);
      if (lchild) {
        node->children[0] = lchild;
        node->sum += lchild->sum;
        node->count += lchild->count;
      }
      if (rchild) {
        node->children[1] = rchild;
        node->sum += rchild->sum;
        node->count += rchild->count;
      }
      co_return node;
    }
    static task<std::monostate> destruct(Node *node) {
      if (node) {
        co_await destruct(node->children[0]);
        co_await destruct(node->children[1]);
      }
      delete node;
      co_return {};
    }

    Node *node_at_index(Node *node, std::size_t idx) {
      while (true) {
        if (!node)
          return nullptr;
        push_down(node);
        std::size_t node_idx = count(node->children[0]);
        if (node_idx == idx)
          return node;
        if (idx < node_idx) {
          node = node->children[0];
        } else {
          node = node->children[1];
          idx -= node_idx + 1;
        }
      }
    }

  public:
    SplayTree(std::span<const long long> values)
        : root(construct(nullptr, values).run()) {}
    ~SplayTree() { destruct(root).run(); }
    long long query_sum(std::size_t l, std::size_t r) {
      Node *left_end = node_at_index(root, l);
      splay(root, left_end);
      if (l == r)
        return root->value;
      Node *right_end = node_at_index(root, r);
      splay(left_end->children[1], right_end);
      return left_end->value + right_end->value +
             (right_end->children[0] ? right_end->children[0]->sum : 0);
    }
    void reverse(std::size_t l, std::size_t r) {
      if (l == r)
        return;
      Node *left_end = node_at_index(root, l);
      splay(root, left_end);
      Node *right_end = node_at_index(root, r);
      splay(left_end->children[1], right_end);
      std::swap(left_end->value, right_end->value);
      maintain_invariant(right_end);
      maintain_invariant(left_end);
      if (right_end->children[0])
        right_end->children[0]->reversed ^= true;
    }
  };

public:
  std::vector<long long>
  reverseSum(const std::vector<long long> &values,
             const std::vector<std::array<int, 3>> &operations) {
    SplayTree splay(values);
    std::vector<long long> result;
    for (auto [kind, l, r] : operations) {
      if (kind == 0) {
        splay.reverse(l, r);
        continue;
      }
      result.push_back(splay.query_sum(l, r));
    }
    return result;
  }
};
