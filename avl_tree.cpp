// https://codeforces.com/group/PVbQ8eK2T4/contest/357142/submission/137737210
/*
Реализуйте AVL-дерево. Решения с использованием других структур засчитываться не будут.
Входные данные

Входной файл содержит описание операций с деревом. Операций не больше 10^5.

В каждой строке находится одна из следующих операций:

	insert x — добавить в дерево ключ x. Если ключ x есть в дереве, то ничего делать не надо.

	delete x — удалить из дерева ключ x. Если ключа x в дереве нет, то ничего делать не надо.

	exists x — если ключ x есть в дереве, вывести «true», иначе «false»

	next x — минимальный элемент в дереве, больший x, или «none», если такого нет.

	prev x — максимальный элемент в дереве, меньший x, или «none», если такого нет. 

Все числа во входном файле целые и по модулю не превышают 10^9.

Выходные данные

Выведите последовательно результат выполнения всех операций exists, next, prev.

Следуйте формату выходного файла из примера.
*/

#include <cassert>
#include <string>
#include <iostream>
 
namespace dst {
      template <class T>
            T& max (T& l, T& r) {
                  return (l > r) ? l : r;
            }
}
 
namespace dst {
      template <class T>
            struct cmp_default {
                  bool operator() (const T& l, const T& r) { return l < r; }
            };
}
 
namespace dst {
      template <class K, class V, class C = cmp_default<K>>
            class avl_tree {
                  friend std::ostream& operator<< (std::ostream& out, avl_tree& tree) {
                        tree.display (out, "", tree.root, false);
                        return out;
                  }
 
                  public:
                        avl_tree (C cmp_function = cmp_default<K> ());
                        ~avl_tree () { if (root) {root->node_delete (); delete root;} }
 
                        V& operator[] (const K& key);
                        const V& operator[] (const K& key) const;
 
                        void erase (const K& key);
 
                        const K& next_key (const K& key);
                        const K& prev_key (const K& key);
 
                        bool exists (const K& key);
 
                  private:
                        
                        struct Node;
                        C cmp;
 
                        Node* root;
 
                        void fix_broken (Node* node);
 
                        void display (std::ostream&, const std::string, Node*, bool);
      
            };
 
      template <class K, class V, class C>
            avl_tree<K,V,C>::avl_tree (C cmp_function) :
                  root (nullptr), cmp (cmp_function) {}
 
      template <class K, class V, class C>
            V& avl_tree<K,V,C>::operator[] (const K& key) {
                  if (!root) {
                        root = new Node (nullptr, key);
                        return root->value;
                  }
 
                  Node* node = root;
 
                  while (true) {
                        if (cmp (key, node->key)) {
                              if (!node->left) {
                                    node->left = new Node (node, key);
                                    node = node->left;
                              
                                    break;
                              }
 
                              node = node->left;
                              continue;
                        }
 
                        if (cmp (node->key, key)) {
                              if (!node->right) {
                                    node->right = new Node (node, key);
                                    node = node->right;
 
                                    break;
                              }
 
                              node = node->right;
                              continue;
                        }
 
                        return node->value;
                  }
 
                  fix_broken (node->parent);
 
                  return node->value;
            }
 
      template <class K, class V, class C>
            void avl_tree<K,V,C>::erase (const K& key) {
                  if (!root)
                        return;
 
                  Node* node = root;
 
                  while (node) {
                        if (cmp (key, node->key)) {
                              node = node->left;
                              continue;
                        }
 
                        if (cmp (node->key, key)) {
                              node = node->right;
                              continue;
                        }
 
                        break;
                  }
 
                  if (!node)
                        return;
 
                  if (!node->left) {
                        if (node->parent)
                              (node->is_left() ? (node->parent)->left : (node->parent)->right) = node->right;
                        
                        (node->right ? node->right : node)->parent = node->parent;
                        
                        if (node->parent)
                              fix_broken (node->parent);
                        else
                              root = node->right;
 
                        delete node;
                        return;
                  }
 
                  if (!node->right) {
                        if (node->parent)
                              (node->is_left() ? (node->parent)->left : (node->parent)->right) = node->left;
                        
                        (node->left ? node->left : node)->parent = node->parent;
                        
                        if (node->parent)
                              fix_broken (node->parent);
                        else
                              root = node->left;
 
                        delete node;
                        return;
                  }
 
                  Node* other = (node->left)->max_node ();
 
                  (other->is_left() ? (other->parent)->left : (other->parent)->right) = other->left;
                  if (other->left)
                        (other->left)->parent = other->parent;
 
                  node->key = other->key;
                  node->value = other->value;
 
                  fix_broken (other->parent);
 
                  delete other;
            }
 
      template <class K, class V, class C>
            void avl_tree<K,V,C>::fix_broken (Node* broken) {
 
                  while (broken != root) {
 
                        broken->fix_depth ();
 
                        if (broken->diff() >= 2) {
                              broken = broken->rotate_left ();
                              broken = broken->parent;
 
                              continue;
                        }
 
                        if (broken->diff() <= -2) {
                              broken = broken->rotate_right ();
                              broken = broken->parent;
 
                              continue;
                        }
 
                        broken = broken->parent;
                  }
 
                  root->fix_depth ();
 
                  if (broken->diff() >= 2) {
                        root = broken->rotate_left();
                        return;
                  }
 
                  if (broken->diff() <= -2) {
                        root = broken->rotate_right();
                  }
            }
 
      template <class K, class V, class C>
            const K& avl_tree<K,V,C>::next_key (const K& key) {
                  Node* node = root;
                  Node* ans = nullptr;
                  
                  while (node) {
                        if (cmp (key, node->key)) {
                              ans = node;
                              node = node->left;
                              continue;
                        }
 
                        node = node->right;
                  }
 
                  if (ans)
                        return ans->key;
                  return key;
            }
 
      template <class K, class V, class C>
            const K& avl_tree<K,V,C>::prev_key (const K& key) {
                  Node* node = root;
                  Node* ans = nullptr;
 
                  while (node) {
                        if (cmp (node->key, key)) {
                              ans = node;
                              node = node->right;
                              continue;
                        }
 
                        node = node->left;
                  }
 
                  if (ans)
                        return ans->key;
                  
                  return key;
            }
 
      template <class K, class V, class C>
            bool avl_tree<K,V,C>::exists (const K& key) {
                  Node* node = root;
 
                  while (node) {
                        if (cmp (key, node->key)) {
                              node = node->left;
                              continue;
                        }
 
                        if (cmp (node->key, key)) {
                              node = node->right;
                              continue;
                        }
 
                        return true;
                  }
 
                  return false;
            }
 
      template <class K, class V, class C>
            void avl_tree<K,V,C>::display (std::ostream& out,
                                           const std::string prefix,
                                           Node* node, bool isLeft)
            {
                  if (!node)
                        return;
 
                  out << prefix;
                  out << (isLeft ? "├──" : "└──");
                  
                  //std::cout << node->key << " [" << node->depth << "]" << std::endl;
                  std::cout << node->key << std::endl;
 
                  display (out, prefix + (isLeft ? "│   " : "    "), node->left, true); 
                  display (out, prefix + (isLeft ? "│   " : "    "), node->right, false);
            }
 
      //       Node struct functions
 
      template <class K, class V, class C>
            struct avl_tree<K,V,C>::Node {
                  Node* parent;
 
                  Node* left;
                  Node* right;
 
                  K key;
                  V value;
 
                  unsigned long long depth;
 
                  Node (Node* parent, const K& key);
                  ~Node () {};
                  void node_delete () {
                        if (left) { left->node_delete (); delete left; }
                        if (right) { right->node_delete (); delete right; }
                  };
 
                  int diff ();
                  bool is_left ();
                  void fix_depth ();
 
                  Node* rotate_left ();
                  Node* rotate_right ();
 
                  Node* s_rotate_left ();
                  Node* s_rotate_right ();
 
                  Node* next ();
                  Node* prev ();
 
                  Node* max_node ();
            };
 
      template <class K, class V, class C>
            avl_tree<K,V,C>::Node::Node (Node* parent, const K& key) :
                  parent (parent), left (nullptr), right (nullptr),
                  key (key), depth (1) {}
 
      template <class K, class V, class C>
            int avl_tree<K,V,C>::Node::diff () {
                  unsigned long long left_depth = (left ? left->depth : 0);
                  unsigned long long right_depth = (right ? right->depth : 0);
 
                  return (left_depth > right_depth) ?
                          static_cast <int> (left_depth - right_depth) :
                         -static_cast <int> (right_depth - left_depth);
            }
 
      template <class K, class V, class C>
            bool avl_tree<K,V,C>::Node::is_left () {
                  return this == parent->left;
            }
 
      template <class K, class V, class C>
            void avl_tree<K,V,C>::Node::fix_depth () {
 
                  unsigned long long l_depth = left ? left->depth : 0;
                  unsigned long long r_depth = right ? right->depth : 0;
 
                  depth = max (l_depth, r_depth) + 1;
            }
 
      template <class K, class V, class C>
            typename avl_tree<K,V,C>::Node* avl_tree<K,V,C>::Node::rotate_left () {
                  if (left->diff() < 0)
                        left->s_rotate_right ();
 
                  return s_rotate_left ();
            }
 
      template <class K, class V, class C>
            typename avl_tree<K,V,C>::Node* avl_tree<K,V,C>::Node::rotate_right () {
                  if (right->diff() > 0)
                        right->s_rotate_left ();
 
                  return s_rotate_right();
            }
 
      template <class K, class V, class C>
            typename avl_tree<K,V,C>::Node* avl_tree<K,V,C>::Node::s_rotate_left () {
                  
                  Node* l_node = left;
                  Node* lr_node = left->right;
 
                  if (lr_node)
                        lr_node->parent = this;
 
                  left = lr_node;
 
                  if (parent)
                        (is_left () ? parent->left : parent->right) = l_node;
 
                  l_node->parent = parent;
                  parent = l_node;
                  l_node->right = this;
 
                  fix_depth ();
                  l_node->fix_depth ();
 
                  return l_node;
            }
 
      template <class K, class V, class C>
            typename avl_tree<K,V,C>::Node* avl_tree<K,V,C>::Node::s_rotate_right () {
                  
                  Node* r_node = right;
                  Node* rl_node = right->left;
 
                  if (rl_node)
                        rl_node->parent = this;
 
                  right = rl_node;
 
                  if (parent)
                        (is_left () ? parent->left : parent->right) = r_node;
 
                  r_node->parent = parent;
                  parent = r_node;
                  r_node->left = this;
 
                  fix_depth ();
                  r_node->fix_depth ();
 
                  return r_node;
 
            }
      
      template <class K, class V, class C>
            typename avl_tree<K,V,C>::Node* avl_tree<K,V,C>::Node::next () {
                  Node* node = this;
 
                  if (node->right) {
                        node = node->right;
 
                        while (node->left)
                              node = node->left;
 
                        return node;
                  }
 
                  while (node->parent && !node->is_left()) {
                        node = node->parent;
                  }
 
                  node = node->parent;
 
                  if (!node)
                        return nullptr;
 
                  node = node->right;
 
                  while (node->left)
                        node = node->left;
 
                  return node;
            }
 
      template <class K, class V, class C>
            typename avl_tree<K,V,C>::Node* avl_tree<K,V,C>::Node::prev () {
                  Node* node = this;
 
                  if (node->left) {
                        node = node->left;
 
                        while (node->right)
                              node = node->right;
 
                        return node;
                  }
 
                  while (node->parent && node->is_left()) {
                        node = node->parent;
                  }
 
                  node = node->parent;
 
                  if (!node)
                        return nullptr;
 
                  node = node->left;
 
                  while (node->right)
                        node = node->right;
 
                  return node;
            }
 
      template <class K, class V, class C>
            typename avl_tree<K,V,C>::Node* avl_tree<K,V,C>::Node::max_node () {
                  Node* res = this;
 
                  while (res->right) {
                        res = res->right;
                  }
 
                  return res;
            }
}
/*
#include <random>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
 
void stress_test (int iterations = 5) { //, size_t max_size = 10) {
      std::random_device rd;
      std::mt19937 rng(rd());
 
      //std::ofstream fout;
      //fout.open ("./log.txt", std::ios::trunc);
 
      std::vector<int> test;
      std::vector<int> ins;
      std::map<int, int> M;
      dst::avl_tree<int, int> m;
 
      for (int i = 0; i < iterations; ++i) {
            test.push_back(i);
      }
 
      std::shuffle (std::begin(test), std::end(test), rng);
 
      ins = test;
 
      for (int i = 0; i < iterations; ++i) {
            m[test[i]] = i;
            M[test[i]] = i;
      }
 
      std::shuffle (std::begin(test), std::end(test), rng);
 
      bool error = false;
 
      for (int i = 0; i < iterations; ++i) {
            if (m.exists(test[i]) != M.contains(test[i])) {
                  std::cout << test[i] << "\n";
                  std::cerr << "ERROR\n";
                  for (size_t j = 0; j < iterations; ++j) {
                        std::cout << "insert " << ins[j] << "\n";
                  }
                  for (size_t j = 0; j < i; ++j)
                        std::cout << "delete " << test[j] << "\n";
                  std::cout << "\n";
                  exit(0);
            }
 
            m.erase (test[i]);
            M.erase (test[i]);
      }
 
}
 
int main () {
      for (size_t i = 0; i < 10000; ++i)
            stress_test (20);
}
*/
 
int main () {
      dst::avl_tree<long long, unsigned long long> T;
 
      std::string request;
      long long key;
 
      while (std::cin >> request) {
            
            if (request == "exit")
                  return 0;
 
            if (request == "insert") {
                  std::cin >> key;
                  T[key] = 0;
                  continue;
            }
 
            if (request == "delete") {
                  std::cin >> key;
                  T.erase (key);
                  continue;
            }
 
            if (request == "next") {
                  std::cin >> key;
                  long long next_key = T.next_key (key);
 
                  if (next_key != key)
                        std::cout << next_key << "\n";
                  else
                        std::cout << "none\n";
                  continue;
            }
 
            if (request == "prev") {
                  std::cin >> key;
                  long long prev_key = T.prev_key (key);
 
                  if (prev_key != key)
                        std::cout << prev_key << "\n";
                  else
                        std::cout << "none\n";
 
                  continue;
            }
 
            if (request == "exists") {
                  std::cin >> key;
                  std::cout << (T.exists (key) ? "true\n" : "false\n");
 
                  continue;
            }
 
            if (request == "show") {
                  std::cout << std::endl;
                  std::cout << T << std::endl;
                  continue;
            }
 
            if (request == "enum") {
                  key = 0;
                  long long p_key = T.next_key (0);
 
                  while (p_key != key) {
                        std::cout << p_key << ' ';
                        key = p_key;
                        p_key = T.next_key (key);
                  }
                  std::cout << '\n';
                  continue;
            }
      }
}
