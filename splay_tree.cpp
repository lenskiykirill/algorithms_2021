/*
Ленский Кирилл, Б05-126
https://codeforces.com/group/PVbQ8eK2T4/contest/357142/submission/138492803

Реализуйте splay-дерево, которое поддерживает множество S целых чисел, в котором разрешается производить следующие операции:

    add(i) — добавить в множество S число i (если он там уже есть, то множество не меняется);
    sum(l,r) — вывести сумму всех элементов x из S, которые удовлетворяют неравенству l≤x≤r. 
*/

#include <iostream>

namespace dst {
      namespace {
            template <class T>
                  struct cmp_default {
                        bool operator() (const T& left, const T& right) {
                              return left < right;
                        }
                  };
      }

      template <typename T1, typename T2=T1>
            struct pair {
                  T1 first;
                  T2 second;

                  pair (const T1& first, const T2& second) : 
                        first (first), second (second) {}
                  ~pair () {}
            };

      template <typename K, typename V, class C = cmp_default<K>>
            class splay_tree {
                  public:
                        void insert (const K& key);
                        void remove (const K& key);

                        V get_sum (const K& left, const K& right);

                        splay_tree (C cmp_function = cmp_default<K>());
                        ~splay_tree () = default;
                        
                        void display () { __display (std::cout, "", root, false, false); }

                  private:
                        struct Node;
                        
                        C cmp_function;
                        Node* root;

                        void zig_zig (Node* node);
                        void zig_zag (Node* node);

                        void find (const K& key);
                        pair<Node*> split (const K& key);
                        void merge (const pair<Node*>& sub);

                        void __display (std::ostream&, const std::string,
                                        Node*, bool, bool);
            };

      template <typename K, typename V, class C>
            splay_tree<K,V,C>::splay_tree (C cmp_function) :
                  cmp_function (cmp_function), root (nullptr) {}



      //        A   |              |   C       |
      //       / \  |       B      |  / \      |
      //      B   * |      / \     | +   B     |
      //     / \    |     /   \    |    / \    |
      //    C   **  |    C     A   |   ++  A   |
      //   / \      |   / \   / \  |      / \  |
      //  +  ++     |  +  ++ **  * |     **  * |
      //    (rotate right       (rotate right
      //         around B)         around C)

      template <typename K, typename V, class C>
            void splay_tree<K,V,C>::zig_zig (Node* node) {
                  if (node->is_left ()) {
                        node->parent->rotate_right ();
                        node->rotate_right ();
                        
                        return;
                  }

                  node->parent->rotate_left ();
                  node->rotate_left ();
            }

      //    A     |   A       |             |
      //   / \    |  / \      |      C      |
      //  *   B   | *   C     |     / \     |
      //     / \  |    / \    |    /   \    |
      //    C  ** |   +   B   |   A     B   |
      //   / \    |      / \  |  / \   / \  |
      //  +  ++   |     ++ ** | *   + ++ ** |
      //    (rotate right    (rotate left
      //       around C)      around C)

      template <typename K, typename V, class C>
            void splay_tree<K,V,C>::zig_zag (Node* node) {
                  if (node->is_left ()) {
                        node->rotate_right ();
                        node->rotate_left ();
                        return;
                  }

                  node->rotate_left ();
                  node->rotate_right ();
            }

      template <typename K, typename V, class C>
            void splay_tree<K,V,C>::find (const K& key) {
                  if (!root) {
                        root = new Node (nullptr, key);
                  }

                  Node* node = root;

                  while (true) {
                        if (cmp_function (key, node->key)) {
                              if (!node->left)
                                    break;
                              node = node->left;
                              continue;
                        }

                        if (cmp_function (node->key, key)) {
                              if (!node->right)
                                    break;
                              node = node->right;
                              continue;
                        }

                        break;
                  }

                  while (node->parent) {
                        Node* parent = node->parent;
                        if (parent->parent) {
                              (node->is_left() == parent->is_left()) ?
                                    zig_zig (node) :
                                    zig_zag (node) ;
                              continue;
                        }

                        node->is_left() ?
                              node->rotate_right () :
                              node->rotate_left ()  ;
                        break;
                  }

                  root = node;
            }     

      template <typename K, typename V, class C>
            pair<typename splay_tree<K,V,C>::Node*> splay_tree<K,V,C>::split (const K& key) { // split the tree by its root
                  if (!root)
                        return pair<Node*, Node*> (nullptr, nullptr);

                  find (key);
                  
                  if (!cmp_function (root->key, key)) {
                        pair<Node*, Node*> res (root->left, root);
                        res.second->left = nullptr;

                        if (res.first)
                              res.first->parent = nullptr;

                        res.second->fix_value();
                        return res;
                  }

                  pair<Node*, Node*> res (root, root->right);
                  res.first->right = nullptr;

                  if (res.second)
                        res.second->parent = nullptr;

                  res.first->fix_value ();
                        
                  return res;
            }

      template <typename K, typename V, class C>
            void splay_tree<K,V,C>::merge (const pair<splay_tree<K,V,C>::Node*>& sub) {
                  if (!sub.second) {
                        root = sub.first;
                        return;
                  }

                  root = sub.second;

                  if (!sub.first)
                        return;
                  
                  find (sub.first->key); // Find minimal node in second tree and pull it up

                  root->left = sub.first;
                  sub.first->parent = root;

                  root->fix_value ();
            }

      template <typename K, typename V, class C>
            void splay_tree<K,V,C>::insert (const K& key) {
                  find (key);
                  if (root && root->key == key)
                        return;

                  pair<Node*, Node*> sub = split (key);
                  root = new Node (nullptr, key);
                  
                  if (sub.first)
                        sub.first->parent = root;
                  if (sub.second)
                        sub.second->parent = root;

                  root->left = sub.first;
                  root->right = sub.second;

                  root->fix_value ();
            }

      template <typename K, typename V, class C>
            void splay_tree<K,V,C>::remove (const K& key) {
                  find (key);
                  
                  if (!root)
                        return;

                  if (!root->left && !root->right) {
                        delete root;      
                        root = nullptr;
                   
                        return;
                  }
                  
                  pair<Node*, Node*> sub (root->left, root->right);
                  delete root;

                  merge (sub);
                  root->fix_value ();
            }

      template <typename K, typename V, class C>
            V splay_tree<K,V,C>::get_sum (const K& first, const K& last) {

                  pair<Node*> left = split (first);
                  root = left.second;

                  pair<Node*> right = split (last);

                  V ret;

                  if (right.first)
                        ret = right.first->value;
                  else
                        ret = V (0);

                  merge (right);

                  left.second = root;

                  merge (left);

                  return ret;
            }

      template <typename K, typename V, class C>
            struct splay_tree<K,V,C>::Node {
                  Node* parent;
                  Node* left;
                  Node* right;

                  K key;
                  V value;

                  Node (Node* parent, const K& value);
                  ~Node ();

                  void rotate_left ();
                  void rotate_right ();

                  bool is_left ();

                  void fix_value ();
            };


      template <typename K, typename V, class C>
            splay_tree<K,V,C>::Node::Node (Node* parent, const K& key) :
                  parent (parent), left (nullptr), right (nullptr), key (key), value (key) {}

      template <typename K, typename V, class C>
            splay_tree<K,V,C>::Node::~Node () {}

      //      A    |    B      |
      //     / \   |   / \     | Rotate right around B
      //    B   *  |  **  A    |
      //   / \     |     / \   |
      //  ** ***   |   ***  *  |


      template <typename K, typename V, class C>
            void splay_tree<K,V,C>::Node::rotate_right () {
                  
                  if (right)
                        right->parent = parent;
                  parent->left = right;

                  if (parent->parent)
                        (parent->is_left () ? parent->parent->left :
                                              parent->parent->right) = this;

                  right = parent;
                  parent = parent->parent;

                  right->parent = this;

                  right->fix_value ();
                  fix_value ();
            }

      template <typename K, typename V, class C>
            void splay_tree<K,V,C>::Node::rotate_left () {
                  
                  if (left)
                        left->parent = parent;
                  parent->right = left;

                  if (parent->parent)
                        (parent->is_left () ? parent->parent->left :
                                              parent->parent->right) = this;

                  left = parent;
                  parent = parent->parent;

                  left->parent = this;

                  left->fix_value ();
                  fix_value ();
            }

      template <typename K, typename V, class C>
            bool splay_tree<K,V,C>::Node::is_left () {
                  return parent->left == this;
            }


      template <typename K, typename V, class C>
            void splay_tree<K,V,C>::Node::fix_value () {
                  value = V (key);
                  if (left)
                        value += left->value;
                  if (right)
                        value += right->value;
            }

      template <typename K, typename V, class C>
            void splay_tree<K,V,C>::__display (std::ostream& out, const std::string prefix,
                                               Node* node, bool is_left, bool has_right) {
                  if (node) {
                        out << prefix;
                  
                        out << (is_left? "├──" : "└──");
                  
                        out << "(" << node->key << "," << node->value << ")" << std::endl;
                  }

                  else {
                        if (is_left && has_right)
                              out << prefix <<  "│" << std::endl;
                        return;
                  }

                  __display (out, prefix + (is_left ? "│   " : "    "), node->left, true, node->right);
                  __display (out, prefix + (is_left ? "│   " : "    "), node->right, false, true);
            }
}

int main () {
      dst::splay_tree<unsigned long long, unsigned long long> T;
      std::string request;

      size_t N;
      unsigned long long add = 0;
      std::cin >> N;

      for (size_t i = 0; i < N; ++i) {
            std::cin >> request;
            if (request == "?") {
                  unsigned long long l, r;
                  std::cin >> l >> r;

                  std::cout << (add = T.get_sum (l, r+1)) << '\n';
                  continue;
            }

            if (request == "+") {
                  unsigned long long x;
                  std::cin >> x;
                  add = (add + x) % 1000000000;

                  T.insert (add);
                  add = 0;
                  continue;
            }

            if (request == "s") {
                  std::cout << std::endl;
                  T.display ();
                  continue;
            }
      }
}
