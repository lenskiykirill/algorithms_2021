/*
Дано число N и последовательность из N целых чисел. Найти вторую порядковую статистику на заданных диапазонах.

Для решения задачи используйте структуру данных Sparse Table. Требуемое время обработки каждого диапазона O(1). Время подготовки структуры данных O(nlogn).

Входные данные

В первой строке заданы 2 числа: размер последовательности N и количество диапазонов M.

Следующие N целых чисел задают последовательность. Далее вводятся M пар чисел - границ диапазонов.

Выходные данные

Для каждого из M диапазонов напечатать элемент последовательности - 2ю порядковую статистику. По одному числу в строке.
*/

#include <iostream>

struct pair {
  size_t first;
  size_t second;

  const pair& operator= (const pair& other) {first = other.first; second = other.second; return other;}
};

class sparse_table {
  public:
        sparse_table (long long* array, size_t size);
        ~sparse_table ();

        size_t get_idx (size_t left, size_t right);

  private:
        pair** table;
        long long* array;

        size_t x;
        size_t y;

        pair get_min (pair& a, pair& b);
};

pair sparse_table::get_min (pair& a, pair& b) {
  pair ans;

  if (a.first == b.first) {
        ans.first = a.first;

        ans.second = (array[a.second] < array[b.second])?a.second:b.second;
        return ans;
  }

  if (array[a.first] < array[b.first]) {
        ans.first = a.first;
        
        if (a.first == a.second) {
              ans.second = b.first;
              return ans;
        }

        ans.second = (array[a.second] < array[b.first])?a.second:b.first;

        return ans;
  }

  ans.first = b.first;

  if (b.first == b.second) {
        ans.second = a.first;
        return ans;
  }

  ans.second = (array[b.second] < array[a.first])?b.second:a.first;

  return ans;
}

sparse_table::sparse_table (long long* array, size_t size) : x(1), y(1) {

  for (; y < size; ++x)
        y <<= 1;

  table = new pair* [x];
  this->array = new long long[y];

  table[0] = new pair[y];

  size_t sub = 1;
  
  for (size_t j = 0; j < size; ++j) {
        this->array[j] = array[j];
        table[0][j].first = j;
        table[0][j].second = j;
  }

  for (size_t j = size; j < y; ++j) {
        table[0][j].first = 0;
        table[0][j].second = 0;
  }

  for (size_t i = 1; i < x; ++i) {

        y -= sub;
        table[i] = new pair[y];

        for (size_t j = 0; j < y; ++j) {
              table[i][j] = get_min (table[i-1][j], table[i-1][j+sub]);
        }

        sub <<= 1;
  }
}

size_t sparse_table::get_idx (size_t left, size_t right) {
  size_t delta = right - left;
  x = 0;
  for (; delta > 0; delta >>= 1) {
        ++x;
  }
  --x;

  pair res_1 = table[x][left];
  pair res_2 = table[x][right-(1<<x)];

  return get_min(res_1, res_2).second;
}

sparse_table::~sparse_table () {
  delete [] array;
  for (size_t i = 0; i < x; i++)
        delete [] table[i];
  delete [] table;
}

int main() {
  size_t N, M;
  std::cin >> N >> M;    

  long long* array = new long long [N];

  for (size_t i=0; i < N; ++i)
        std::cin >> array[i];

  sparse_table s(array, N);

  for (size_t i=0; i < M; ++i) {
        size_t left, right;

        std::cin >> left >> right;

        long long result = array[s.get_idx(left-1, right)];
        std::cout << result << '\n';
  }

  delete [] array;
}
