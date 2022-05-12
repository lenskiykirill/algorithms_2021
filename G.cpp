#include <iostream>
#include <vector>

#define MODULE 1000000007

/* Ленский Кирилл, 126. https://codeforces.com/group/PVbQ8eK2T4/contest/371629/submission/156940647
 * 
 * До конца света осталось не так много дней, но Третий всадник Апокалипсиса Голод так и не смог понять, какие города стоит наказывать за прегрешения, а какие нет.
 * Тогда он решил потренироваться на одной небольшой стране.
 * Эта страна представляет собой клетчатый прямоугольник размера 𝑛×𝑚
 * , в котором каждая клетка — отдельный город. Голод может либо наслать голод на город, либо пощадить его. При этом есть города, в которых еды хватит и до следующего конца света, и Голод не в силах
 * заставить их голодать, а есть те, в которых еды никогда и не было, и им в любом случае придется голодать.
 *
 * Страшный Суд Голода должен быть ещё и справедлив, а это значит, что в любом квадрате размера 2×2
 * должно быть поровну голодающих и сытых городов. Теперь Голод хочет узнать количество различных вариантов распределения городов этой страны на голодающие и сытые.
 *
 * ------------------------------------------------
 *
 * В качестве dp здесь массив V[i][mask] - число способов голоду замостить начальные i строк так, чтобы маска i-й строки была mask. Понятно, что зная i строку и 1 символ i+1-й строки можно
 * восстановить всю строку целиком. Ну тогда для [i][mask] голод определяет 2 возможные маски след. строки, проверяет, достижимы ли они, и если маска достижима, добавляет к числу в V[i+1][next_mask]
 * число в текущей ячейке. V[0][*] - единицы. Условие выхода - когда заполним таблицу. Результат - сумма в последнем столбце
 */

unsigned long long solve (unsigned int n, unsigned int m, char** M) {

      unsigned long long **V = new unsigned long long* [2];
      
      unsigned long long N = (1 << n);
      
      for (unsigned int i = 0; i < 2; ++i)
            V[i] = new unsigned long long [N];

      std::vector<unsigned long long> zeros (N);
      std::vector<unsigned long long> ones (N);

      for (unsigned long long d = 0; d < N; ++d) {
            bool z_flag = true, o_flag = true;
            unsigned long long zero = 0, one = 1;

            for (unsigned int k = 1; k < n; ++k) {
                  unsigned int used = 0;
                  unsigned int used_ones = 0, used_zeros = 0;

                  used += (d >> (k-1))&1;
                  used += (d >> k)&1;
                  used_ones += (one >> (k-1))&1;
                  used_zeros += (zero >> (k-1))&1;
                  
                  if (used + used_ones == 1)
                        one += (1 << k);
                  else if (used + used_ones == 2) {}
                  else
                        o_flag = false;

                  if (used + used_zeros == 1)
                        zero += (1 << k);
                  else if (used + used_zeros == 2) {}
                  else
                        z_flag = false;
            }

            if (z_flag)
                  zeros[d] = zero;
            else
                  zeros[d] = -1;

            if (o_flag)
                  ones[d] = one;
            else
                  ones[d] = -1;
      }

      std::vector<unsigned long long> p_rows (m); // has 1 on every +. If p_row | d != d, d is unsuitable

      for (unsigned int i = 0; i < m; ++i) {
            p_rows[i] = 0;
            for (unsigned j = 0; j < n; ++j)
                  if (M[j][i] == '+')
                        p_rows[i] += (1 << j);
      }

      std::vector<unsigned long long> n_rows (m); // has 0 on every minus. If n_row & d != d -- d is unsuitable

      for (unsigned int i = 0; i < m; ++i) {
            n_rows[i] = 0;
            for (unsigned j = 0; j < n; ++j)
                  if (M[j][i] == '-')
                        n_rows[i] += (1 << j);
            n_rows[i] ^= (1 << n) - 1;
      }

      for (unsigned long long d = 0; d < N; ++d) {
            if ((d | p_rows[0]) == d && (d & n_rows[0]) == d)
                  V[0][d] = 1;
            else
                  V[0][d] = 0;
      }

      for (unsigned int i = 0; i < m-1; ++i) {
            for (unsigned long long d = 0; d < N; ++d)
                  V[(i+1)&1][d] = 0;

            for (unsigned long long d = 0; d < N; ++d) {
                  if (zeros[d] != -1) {
                        unsigned long long z = zeros[d];
                        if ((z | p_rows[i+1]) == z && (z & n_rows[i+1]) == z) {
                              V[(i+1)&1][z] = (V[(i+1)&1][z] + V[i&1][d]) % MODULE;
                        }
                  }

                  if (ones[d] != -1) {
                        unsigned long long o = ones[d];
                        if ((o | p_rows[i+1]) == o && (o & n_rows[i+1]) == o)
                              V[(i+1)&1][o] = (V[(i+1)&1][o] + V[i&1][d]) % MODULE;
                  }
            }
      }

      unsigned long long res = 0;
      for (unsigned int i = 0; i < N; ++i) {
            res += V[(m+1)&1][i];
            res %= MODULE;
      }

      for (size_t i = 0; i < 2; ++i)
            delete [] V[i];
      delete [] V;

      return res;
}

int main () {
      unsigned int n, m;
      std::cin >> n >> m;

      char** M = new char* [n];
      for (unsigned int i = 0; i < n; ++i)
            M[i] = new char [m];

      for (unsigned int i = 0; i < n; ++i)
            for (unsigned j = 0; j < m; ++j)
                  std::cin >> M[i][j];

      auto res = solve (n, m, M);

      for (size_t i = 0; i < n; ++i)
            delete [] M[i];
      delete [] M;

      std::cout << res << std::endl;

      return 0;
}
