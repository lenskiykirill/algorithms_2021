/*
Нужно отвечать на запросы вида
• + x
• ? x
– добавить в мультимножество число x.
– посчитать сумму чисел не больших x.

Формат входных данных:

В первой строке содержится число запросов 1 <= q <= 10^5 .
Далее каждая строка содержит один запрос.
Все числа x целые от 0 до 10^9 − 1.

Формат выходных данных:

Ответы на все запросы вида ? x.
*/

#include <iostream>

namespace dst {
  class strange_struct {
        private:
              struct node;
        public:
              strange_struct ();
              ~strange_struct ();

              void insert (long long x);
              long long get_sum (long long x);
        private:
              node* top_node;
  };
  
  struct strange_struct::node {
        long long value;
        long long left_border;
        long long right_border;

        node* left_child;
        node* right_child;

        node(long long left_border, long long right_border) {
              
              value = 0;
              
              this->left_border = left_border;
              this->right_border = right_border;
              

              left_child = nullptr;
              right_child = nullptr;
        }

        ~node() {
              if (left_child) {delete left_child;}
              if (right_child) {delete right_child;}
        }
  };

  strange_struct::strange_struct () {
        top_node = new node (0, 1<<30);
  }

  strange_struct::~strange_struct () {
        delete top_node;
  }

  void strange_struct::insert (long long x) {
        node* parent = top_node;
        
        parent->value += x;

        while (parent->right_border-parent->left_border > 1) {
              long long middle = (parent->left_border + parent->right_border) / 2;
              if (x < middle) {
                    if (parent->left_child == nullptr) {
                          parent->left_child = new node (parent->left_border, middle);
                    }
                    (parent->left_child)->value += x;
                    parent = parent->left_child;
                    continue;
              }
              
              if (parent->right_child == nullptr) {
                    parent->right_child = new node (middle, parent->right_border);
              }
              (parent->right_child)->value += x;
              parent = parent->right_child;
        }

  }

  long long strange_struct::get_sum (long long x) {
        node* parent = top_node;
        long long result = 0;
        
        while (parent && (parent->right_border - parent->left_border > 1)) {

              long long middle = (parent->left_border + parent->right_border)/2;

              if (x >= middle) {
                    if (parent->left_child != nullptr) {
                          result += (parent->left_child)->value;
                    }
                    parent = parent->right_child;
                    continue;
              }
              
              parent = parent->left_child;
        }

        if (parent != nullptr)
              result += parent->value;

        return result;
  }
}

int main () {
  dst::strange_struct st;
  size_t N;
  std::cin >> N;

  for (size_t i = 0; i < N; ++i) {
        char request;
        long long x;
        std::cin >> request;
        switch (request) {
              case '+':
                    std::cin >> x;
                    st.insert(x);
                    break;
              case '?':
                    std::cin >> x;
                    x = st.get_sum(x);
                    std::cout << x << '\n';
        }
  }
}
