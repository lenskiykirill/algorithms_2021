/* Ленский Кирилл, Б05-126. https://codeforces.com/group/PVbQ8eK2T4/contest/377095/submission/153592409

Написать алгоритм для решения игры в "пятнашки". Решением задачи является приведение к виду: [ 1 2 3 4 ] [ 5 6 7 8 ] [ 9 10 11 12] [ 13 14 15 0 ], где 0 задает пустую ячейку. Необходимо найти минимальное решение.

Я использовал двусторонний A*, состояния доски хранил либо массивом, либо хитрой кодировкой внутри uint64_t. Эвристика - сумма Манхэтанских расстояний от всех клеток, кроме 0 до мест, где они должны
бы быть.

*/


#include <iostream>
#include <unordered_map>
#include <cassert>
#include <algorithm>
#include <queue>

unsigned int false_count = 0;
unsigned int count = 0;

//  #define fifteen_node_ARRAY
  #define fifteen_node_NUMBER

namespace dst {

      class fifteen {
            public:

#ifdef fifteen_node_ARRAY           
            struct node {
                  node ();
                  node (const node& other);
      
                  node (const std::initializer_list<int>& x);
                  node (const std::vector<int>& x);
      
                  ~node () = default;
      
                  bool parity () const;
                  void to_array (uint8_t* array) const;
      
                  node up() const;
                  node down() const;
      
                  node left() const;
                  node right() const;
      
                  size_t dist (const node& other) const;
                  bool is_none () const;
      
                  uint64_t __array[2];
                  uint8_t* array = reinterpret_cast<uint8_t*> (__array);
                  uint8_t zero_pos;
      
                  node& operator= (const node& other);
      
                  template <typename T>
                        void init (const T& cont) {
                              size_t pos = 0;
                              for (auto it = cont.begin(); it != cont.end(); ++it, ++pos) {
                                    if (*it == 0)
                                          zero_pos = pos;
                                    array[pos] = *it;
                              }
                        }
                  
                  class hash {
                        std::hash<uint64_t> h;

                        public:
                        size_t operator() (const node& x) const {
                              const size_t prime = 19937;
                              size_t res = 0;

                              for (size_t i = 0; i < 2; ++i) {
                                    res ^= x.__array[i] >> 4;
                                    res *= prime;

                                    res ^= (x.__array[i] & 0xffffffff);
                                    res *= prime;
                              }

                              res ^= h(x.zero_pos);
                              return res;
                        }
                  };
            };
#endif
#ifdef fifteen_node_NUMBER
            struct node {
                  node ();
                  node (const node& other);

                  node (const std::initializer_list<int>& x);
                  node (const std::vector<int>& x);

                  ~node () = default;

                  bool parity () const;
                  void to_array (uint8_t* array) const;

                  node up() const;
                  node down() const;

                  node left() const;
                  node right() const;

                  size_t dist (const node& other) const;
                  bool is_none () const;

                  uint64_t state;

                  node& operator= (const node& other);

                  void __set (uint8_t pos, uint8_t value);
                  uint8_t __get (uint8_t pos) const;

                  template <typename T>
                        void init (const T& cont) {
                              state = 0;
                              uint8_t zero_pos = 0;

                              auto it = cont.begin();
                              uint8_t zero_val = *(it++);

                              for (size_t pos = 1; pos < 16; ++pos) {
                                    uint8_t value = *(it++);
                                    if (value == 0) {
                                          zero_pos = pos;
                                          continue;
                                    }
                                    __set (pos, value);
                              }
                              __set (0, zero_pos);
                              __set (zero_pos, zero_val);
                        }

                  class hash {
                        std::hash<uint64_t> h;

                        public:
                        size_t operator() (const node& x) const {
                              return h (x.state);
                        }
                  };
            };
#endif
                  struct A_node {
                        node N;
                        long long f;
                        size_t g;

                        uint8_t move;
                  };

                  std::vector<char> operator() (const node&, const node&);
            private:
                  bool expand_node (const A_node& x, bool forward);

                  node S;
                  node T;

                  std::unordered_map<node,
                        std::pair<uint8_t, size_t>, node::hash> found_f;
                  std::unordered_map<node,
                        std::pair<uint8_t, size_t>, node::hash> found_b;

                  class priority_queue {
                        public:
                              static const uint8_t max_size = 85;

                              priority_queue () = default;
                              ~priority_queue () = default;

                              const A_node& top ();
                              void push (const A_node& value);
                              void pop ();

                              bool empty () { return queue.empty(); }
                        private:
                              std::vector<A_node> array[max_size];
                              std::priority_queue<uint8_t> queue;
                  };

                  priority_queue search_f;
                  priority_queue search_b;

                  node g_left;
                  node g_right;
                  uint64_t m_path;

                  uint8_t bridge;
      };

#ifdef fifteen_node_ARRAY

      fifteen::node::node () {
            for (size_t i = 0; i < 16; ++i)
                  array[i] = 0;
            zero_pos = -1;
      }
      
      fifteen::node::node (const std::initializer_list<int>& x) {
            init<std::initializer_list<int>> (x);
      }

      fifteen::node::node (const std::vector<int>& x) {
            init<std::vector<int>> (x);
      }

      fifteen::node::node (const node& other) {
            for (size_t i = 0; i < 16; ++i)
                  array[i] = other.array[i];

            zero_pos = other.zero_pos;
      }

      bool fifteen::node::parity () const {
            
            bool res = 0;

            for (size_t i = 0; i < 16; ++i)
                  for (size_t j = i+1; j < 16; ++j)
                        res ^= (array[i] > array[j]);

            uint8_t zero_x = zero_pos & 0b11;
            uint8_t zero_y = zero_pos >> 2;

            res ^= (zero_x + zero_y) & 1;

            return res;
      }

      fifteen::node fifteen::node::up () const {
            uint8_t zero_y = zero_pos >> 2;

            if (zero_y == 0)
                  return node ();

            node res (*this);
            res.zero_pos -= 4;

            std::swap (res.array[zero_pos], res.array[res.zero_pos]);
            return res;
      }

      fifteen::node fifteen::node::down () const {
            uint8_t zero_y = zero_pos >> 2;

            if (zero_y == 3)
                  return node ();

            node res (*this);
            res.zero_pos += 4;

            std::swap (res.array[zero_pos], res.array[res.zero_pos]);
            return res;
      }

      fifteen::node fifteen::node::left () const {
            uint8_t zero_x = zero_pos & 0b11;

            if (zero_x == 0)
                  return node ();

            node res (*this);
            res.zero_pos -= 1;

            std::swap (res.array[zero_pos], res.array[res.zero_pos]);
            return res;
      }

      fifteen::node fifteen::node::right () const {
            uint8_t zero_x = zero_pos & 0b11;

            if (zero_x == 3)
                  return node ();

            node res (*this);
            res.zero_pos += 1;

            std::swap (res.array[zero_pos], res.array[res.zero_pos]);
            return res;
      }
      
      size_t fifteen::node::dist (const node& other) const {
            uint8_t first[16], second[16];

            for (size_t i = 0; i < 16; ++i) {
                  uint8_t x = array[i];
                  
                  if (x != 0)
                        first[x] = i;
            }

            for (size_t i = 0; i < 16; ++i) {
                  uint8_t x = other.array[i];

                  if (x != 0)
                        second[x] = i;
            }

            size_t res = 0;
            
            for (uint8_t i = 1; i < 16; ++i) {
                  uint8_t f = first[i] & 0b11;
                  uint8_t s = second[i] & 0b11;

                  if (f > s)
                        res += (f-s);
                  else
                        res += (s-f);

                  f = first[i] >> 2;
                  s = second[i] >> 2;

                  if (f > s)
                        res += (f-s);
                  else
                        res += (s-f);
            }

            return res;
      }

      
      bool fifteen::node::is_none () const {
            return (zero_pos == 255);
      }

      fifteen::node& fifteen::node::operator= (const node& other) {
            __array[0] = other.__array[0];
            __array[1] = other.__array[1];

            zero_pos = other.zero_pos;
            return *this;
      }

      bool operator== (const fifteen::node& left, const fifteen::node& right) {
            if (left.__array[0] != right.__array[0])
                  return false;
            
            if (left.__array[1] != right.__array[1])
                  return false;

            return left.zero_pos == right.zero_pos;
      }

      bool operator!= (const fifteen::node& left, const fifteen::node& right) {
                  return !(left == right);
      }
#endif

#ifdef fifteen_node_NUMBER
      fifteen::node::node () {
            state = -1;
      }

      fifteen::node::node (const std::initializer_list<int>& x) {
            init<std::initializer_list<int>> (x);
      }

      fifteen::node::node (const std::vector<int>& x) {
            init<std::vector<int>> (x);
      }

      fifteen::node::node (const node& other) {
            state = other.state;
      }

      fifteen::node& fifteen::node::operator= (const node& other) {
            state = other.state;
            return *this;
      }

      void fifteen::node::__set (uint8_t pos, uint8_t value) {
            uint8_t b_pos = pos << 2;
            state ^= (1ull * value) << b_pos;
      }

      uint8_t fifteen::node::__get (uint8_t pos) const {
            uint8_t b_pos = ((pos >> 1) << 3);
            uint8_t align = (pos & 1)?4:0;

            return (state >> (align+b_pos)) & 0xf;
      }

      void fifteen::node::to_array (uint8_t* array) const {
            uint8_t zero_pos = __get (0);
            uint8_t zero_val = 0;

            for (uint8_t pos = 1; pos < 16; ++pos) {
                  if (pos == zero_pos) {
                        zero_val = __get (pos);
                        array[pos] = 0;
                        continue;
                  }

                  array[pos] = __get (pos);
            }
            array[0] = zero_val;
      }

      bool fifteen::node::parity () const {
            bool res = 0;
            uint8_t array[16];
            to_array (array);

            for (uint8_t i = 0; i < 16; ++i)
                  for (uint8_t j = i+1; j < 16; ++j)
                        res ^= (array[i] > array[j]);

            uint8_t zero_pos = __get (0);
            uint8_t zero_x = zero_pos & 0b11;
            uint8_t zero_y = zero_pos >> 2;

            res ^= (zero_x + zero_y) & 1;
            return res;
      }

      fifteen::node fifteen::node::up () const {
            node res;
            res.state = state;

            uint8_t zero_pos = __get (0);
            uint8_t zero_y = zero_pos >> 2;

            if (zero_y == 0) {
                  res.state = -1;
                  return res;
            }
            
            if (zero_pos == 4) {
                  res.__set (0, zero_pos);
                  return res;
            }

            uint8_t new_zero = zero_pos - 4;
            res.__set (0, zero_pos ^ new_zero);

            uint8_t zero_value = res.__get (zero_pos);
            uint8_t new_value = res.__get (new_zero);

            res.__set (zero_pos, zero_value ^ new_value);
            res.__set (new_zero, zero_value ^ new_value);

            return res;
      }

      fifteen::node fifteen::node::down () const {
            node res;
            res.state = state;

            uint8_t zero_pos = __get (0);
            uint8_t zero_y = zero_pos >> 2;

            if (zero_y == 3) {
                  res.state = -1;
                  return res;
            }

            uint8_t new_zero = zero_pos + 4;

            if (zero_pos == 0) {
                  res.__set (0, new_zero);
                  return res;
            }

            res.__set (0, zero_pos ^ new_zero);

            uint8_t zero_value = res.__get (zero_pos);
            uint8_t new_value = res.__get (new_zero);

            res.__set (zero_pos, zero_value ^ new_value);
            res.__set (new_zero, zero_value ^ new_value);

            return res;
      }

      fifteen::node fifteen::node::left () const {
            node res;
            res.state = state;

            uint8_t zero_pos = __get (0);
            uint8_t zero_x = zero_pos & 0b11;

            if (zero_x == 0) {
                  res.state = -1;
                  return res;
            }
            
            if (zero_pos == 1) {
                  res.__set (0, zero_pos);
                  return res;
            }

            uint8_t new_zero = zero_pos - 1;
            res.__set (0, zero_pos ^ new_zero);

            uint8_t zero_value = res.__get (zero_pos);
            uint8_t new_value = res.__get (new_zero);

            res.__set (zero_pos, zero_value ^ new_value);
            res.__set (new_zero, zero_value ^ new_value);

            return res;
      }

      fifteen::node fifteen::node::right () const {
            node res;
            res.state = state;

            uint8_t zero_pos = __get (0);
            uint8_t zero_x = zero_pos & 0b11;

            if (zero_x == 3) {
                  res.state = -1;
                  return res;
            }

            uint8_t new_zero = zero_pos + 1;

            if (zero_pos == 0) {
                  res.__set (0, new_zero);
                  return res;
            }

            res.__set (0, zero_pos ^ new_zero);

            uint8_t zero_value = res.__get (zero_pos);
            uint8_t new_value = res.__get (new_zero);

            res.__set (zero_pos, zero_value ^ new_value);
            res.__set (new_zero, zero_value ^ new_value);

            return res;
      }

      size_t fifteen::node::dist (const node& other) const {
            uint8_t first[16], second[16];

            uint8_t zero_pos = __get (0);
            for (uint8_t pos = 1; pos < 16; ++pos) {
                  if (pos == zero_pos) {
                        first[__get (pos)] = 0;
                        continue;
                  }
                  first[__get (pos)] = pos;
            }
            first[0] = zero_pos;

            zero_pos = other.__get (0);
            for (uint8_t pos = 1; pos < 16; ++pos) {
                  if (pos == zero_pos) {
                        second[other.__get (pos)] = 0;
                        continue;
                  }
                  second[other.__get (pos)] = pos;
            }
            second[0] = zero_pos;

            size_t res = 0;

            for (uint8_t i = 1; i < 16; ++i) {
                  uint8_t f = first[i] & 0b11;
                  uint8_t s = second[i] & 0b11;

                  if (f > s)
                        res += (f-s);
                  else
                        res += (s-f);

                  f = first[i] >> 2;
                  s = second[i] >> 2;

                  if (f > s)
                        res += (f-s);
                  else
                        res += (s-f);
            }
            return res;
      }

      bool operator== (const fifteen::node& left, const fifteen::node& right) {
            return left.state == right.state;
      }

      bool operator!= (const fifteen::node& left, const fifteen::node& right) {
            return left.state != right.state;
      }
#endif

}

namespace dst {

      bool operator< (const fifteen::A_node& left, const fifteen::A_node& right) {
            return left.f > right.f;
      }
}

namespace dst {
      long long h (const fifteen::node& x, const fifteen::node& begin, const fifteen::node& end) {
            long long ans = x.dist (end);
            ans -= x.dist (begin);
      
            return ans;
      }
}

namespace dst {
      const fifteen::A_node& fifteen::priority_queue::top () {
            uint8_t top_layer = queue.top();
            return array[top_layer].back();
      }

      void fifteen::priority_queue::pop () {
            uint8_t top_layer = queue.top();
            array[top_layer].pop_back ();

            if (array[top_layer].size() == 0)
                  queue.pop ();
      }

      void fifteen::priority_queue::push (const A_node& value) {
            uint8_t layer = max_size - value.f;
            if (array[layer].size() == 0)
                  queue.push (layer);

            array[layer].push_back (value);
      }
}

namespace dst {
      const uint8_t mov_up    = 0;
      const uint8_t mov_down  = 1;
      const uint8_t mov_left  = 2;
      const uint8_t mov_right = 3;

      bool fifteen::expand_node (const A_node& x, bool forward) {
            std::unordered_map<node, std::pair<uint8_t, size_t>,
                  node::hash>& found = forward ? found_f : found_b;

            std::unordered_map<node, std::pair<uint8_t, size_t>,
                  node::hash>& other = forward ? found_b : found_f;

            priority_queue& search = forward ? search_f : search_b;

            node& left = forward ? g_left : g_right;
            node& right = forward ? g_right : g_left;

            node& s = forward ? S : T;
            node& t = forward ? T : S;

            A_node y;

            count++;

            if (found.find (x.N) != found.end ()) {

                  false_count++;

                  return false;

            }

            found[x.N] = {x.move, x.g};

            auto it = other.find (x.N);
            if (it != other.end ()) {
                  size_t path = (it->second).second;
                  path += x.g;

                  if (path < m_path) {
                        m_path = path;
                        left = x.N;
                        right = x.N;
                  }

                  return true;
            }

            if (x.f > 81)
                  return false;

            node next = x.N.up ();
            if (found.find (next) == found.end ()) {
                  y.N = next;

                  y.move = mov_up;
                  y.g = x.g + 1;
                  y.f = (y.g << 1) + h (next, s, t);

                  it = other.find (next);
                  if (it != other.end ()) {
                        size_t path = y.g + (it->second).second;

                        if (path < m_path) {
                              m_path = path;
                              left  = x.N;
                              right = next;

                              bridge = y.move;
                              if (!forward)
                                    bridge ^= 1;
                        }
                  }

                  search.push (y);
            }

            next = x.N.down ();
            if (found.find (next) == found.end ()) {
                  y.N = next;

                  y.move = mov_down;
                  y.g = x.g + 1;
                  y.f = (y.g << 1) + h (next, s, t);

                  it = other.find (next);
                  if (it != other.end ()) {
                        size_t path = y.g + (it->second).second;

                        if (path < m_path) {
                              m_path = path;
                              left  = x.N;
                              right = next;

                              bridge = y.move;
                              if (!forward)
                                    bridge ^= 1;
                        }
                  }

                  search.push (y);
            }

            next = x.N.left ();
            if (found.find (next) == found.end ()) {
                  y.N = next;

                  y.move = mov_left;
                  y.g = x.g + 1;
                  y.f = (y.g << 1) + h (next, s, t);

                  it = other.find (next);
                  if (it != other.end ()) {
                        size_t path = y.g + (it->second).second;

                        if (path < m_path) {
                              m_path = path;
                              left  = x.N;
                              right = next;

                              bridge = y.move;
                              if (!forward)
                                    bridge ^= 1;
                        }
                  }

                  search.push (y);
            }

            next = x.N.right ();
            if (found.find (next) == found.end ()) {
                  y.N = next;

                  y.move = mov_right;
                  y.g = x.g + 1;
                  y.f = (y.g << 1) + h (next, s, t);

                  it = other.find (next);
                  if (it != other.end ()) {
                        size_t path = y.g + (it->second).second;

                        if (path < m_path) {
                              m_path = path;
                              left  = x.N;
                              right = next;

                              bridge = y.move;
                              if (!forward)
                                    bridge ^= 1;
                        }
                  }

                  search.push (y);
            }

            return false;
      }

      std::vector<char> fifteen::operator() (const node& begin, const node& end) {
            
            if (begin.parity () ^ end.parity ())
                  throw 0;

            if (begin == end)
                  return std::vector<char> ();

            found_f[node()] = {0, 0};
            found_b[node()] = {0, 0};

            S = begin;
            T = end;

            A_node x, y;
            x.g = 0;
            x.f = h (begin, begin, end);
            x.N = begin;

            search_f.push (x);

            y.g = 0;
            y.f = h (end, end, begin);
            y.N = end;

            search_b.push (y);

            m_path = -1;
            bool r_code = false;

            while (!r_code) {
                  if (search_f.empty ())
                        throw "overrun";

                  if (search_b.empty ())
                        throw "overrun";

                  x = search_f.top ();
                  y = search_b.top ();

                  if (x.f < y.f) {
                        search_f.pop ();
                        r_code = expand_node (x, true);
                  } else {
                        search_b.pop ();
                        r_code = expand_node (y, false);
                  }

                  if (r_code)
                        break;
            }

            std::vector<char> res;

            node s;
            s = g_left;

            while (s != begin) {
                  switch (found_f[s].first) {
                        case mov_up:
                              res.push_back ('D');
                              s = s.down ();
                              break;

                        case mov_down:
                              res.push_back ('U');
                              s = s.up ();
                              break;

                        case mov_left:
                              res.push_back ('R');
                              s = s.right ();
                              break;

                        case mov_right:
                              res.push_back ('L');
                              s = s.left ();
                              break;
                  }
            }

            std::reverse (res.begin(), res.end());

            if (g_left != g_right) {
                  switch (bridge) {
                        case mov_up:
                              res.push_back ('D');
                              s = s.down ();
                              break;

                        case mov_down:
                              res.push_back ('U');
                              s = s.up ();
                              break;

                        case mov_left:
                              res.push_back ('R');
                              s = s.right ();
                              break;

                        case mov_right:
                              res.push_back ('L');
                              s = s.left ();
                              break;
                  } 
            }

            s = g_right;

            while (s != end) {
                  switch (found_b[s].first) {
                        case mov_up:
                              res.push_back ('U');
                              s = s.down ();
                              break;

                        case mov_down:
                              res.push_back ('D');
                              s = s.up ();
                              break;

                        case mov_left:
                              res.push_back ('L');
                              s = s.right ();
                              break;

                        case mov_right:
                              res.push_back ('R');
                              s = s.left ();
                              break;
                  }
            }

            return res;
      }
}

int main () {
      dst::fifteen::node end = {1,  2,  3,  4,
                                5,  6,  7,  8,
                                9,  10, 11, 12,
                                13, 14, 15, 0};

      std::vector<int> x(16);
      
      for (size_t i = 0; i < 16; ++i)
            std::cin >> x[i];

      dst::fifteen::node begin (x);
      dst::fifteen F;

      try {
            auto ans = F(begin, end);
            std::cout << ans.size() << std::endl;

            for (auto c : ans)
                  std::cout << c;
            std::cout << std::endl;
      } catch (int) {
            std::cout << "-1" << std::endl;
      }
}
