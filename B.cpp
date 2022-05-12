/* Ленский Кирилл, Б05-126
 * https://codeforces.com/group/PVbQ8eK2T4/contest/371629/submission/149627592

Крис Юрьевич Каменев на вокзале решил сыграть в "Трехкарточный Монти", хотя его мать запретила ему это делать. В этой игре участвуют три карты, выставленные в ряд слева направо. Изначально карта с валетом, которую нужно угадать, находится под стоящей посередине чашкой.

Затем, за один ход карточный мошенник меняет местами карту посередине с одной из двух других карт случайным образом (он выберет каждую с одинаковой вероятностью), так что выбранная карта становится посередине. Игра длится n ходов, и в каждом из них мошенник независимо выбирает карту для обмена со средней на каждом ходу.

После n-го хода мошенник просит Криса указать на карту с валетом, и он указывает на стоящую посередине карту, но Крис отвлекся во время игры и не уверен, находится ли там карта или нет.
Поэтому он просит вас сообщить ему вероятность того, что карта с валетом действительно посередине.

Число ходов n может быть очень большим, поэтому Крис не сообщил вам его. Вместо этого он дал вам массив a_1,a_2,...,a_k, такой, что
... теховская формула...
другими словами, n равно произведению чисел данного массива.

Из-за проблем с точностью Крис просит вас сообщить ответ в виде несократимой дроби. Другими словами, найдите ответ в виде p/q, причем gcd(p,q)=1, где gcd — наибольший общий делитель.
Так как p и q могут быть очень большими, достаточно вывести остаток от деления каждого из них на 10^9+7.

Заметьте, что мы хотим, чтобы наибольший общий делитель p и q был равен 1, а не наибольший общий делитель их остатков от деления на 10^9+7.
-------------------------------------------------------

Тут несложно видеть, что вероятность обнаружить валета по середине на k-м ходу равна p/(2^k), а с краю - q/(2^k). Причем если p', q' - соответствующие значения для k+1-го хода,
то p'/2^(k+1) = 1/2 * (q/2^k), q'/(2^k+1) = (p/2^k) + 1/2 * (q/2^k) <=> p' = q, q' = 2p + q. Несложно видеть, что если p и q не делятся на 2, то они так и продолжат, а уже после второго хода
они не делятся. Поэтому сократится с 2^k лишь одна двойка. Заводим матрицу

|p'| = |0 1| |p|
|q'|   |2 1| |q|. Просто возводим матрицы в соотв. степень. Эта степень - сумма каких-то других степеней. Т.е. возводим в них и перемножаем.
*/


#include <iostream>
#include <vector>

#define MODULE 1000000007

class matrix {
      private:
            struct mul_struct;
      public:
            matrix (unsigned long long);
            matrix (const matrix& other);
            ~matrix ();
            matrix& operator= (const matrix& other);
            unsigned long long size () const {return N;}

            void power (unsigned long long p);

            unsigned long long& operator[] (unsigned long long idx) { return mat[idx]; }
            const unsigned long long& operator[] (unsigned long long idx) const { return mat[idx]; }

      private:

            unsigned long long N = 4;
            unsigned long long* mat;

            void power (unsigned long long p, matrix& tmp1, matrix& tmp2);
            void multiply (const matrix& left, const matrix& right);
            static void swap (matrix& left, matrix& right);
};

matrix::matrix (unsigned long long N) {
      this->N = N;
      mat = new unsigned long long [N];
}

matrix::matrix (const matrix& other) {
      N = other.N;
      mat = new unsigned long long [N];

      mat[0] = other.mat[0];
      mat[1] = other.mat[1];
      mat[2] = other.mat[2];
      mat[3] = other.mat[3];
}

matrix::~matrix () {
      delete [] mat;
}

void matrix::multiply (const matrix& left, const matrix& right) {

      mat[0] = left.mat[0]*right.mat[0] + left.mat[1]*right.mat[2];
      mat[1] = left.mat[0]*right.mat[1] + left.mat[1]*right.mat[3];
      mat[2] = left.mat[2]*right.mat[0] + left.mat[3]*right.mat[2];
      mat[3] = left.mat[2]*right.mat[1] + left.mat[3]*right.mat[3];

      mat[0] %= MODULE;
      mat[1] %= MODULE;
      mat[2] %= MODULE;
      mat[3] %= MODULE;
}

void matrix::power (unsigned long long p, matrix& tmp1, matrix& tmp2) {
      if (p == 1)
            return;
      if (p % 2) {
            swap (*this, tmp1);
            tmp1.power (p-1, *this, tmp2);
            multiply (tmp1, tmp2);
            return;
      }

      swap (*this, tmp1);
      tmp1.power (p/2, *this, tmp2);
      multiply (tmp1, tmp1);
}

void matrix::power (unsigned long long p) {
      matrix tmp1 (N);
      matrix tmp2 (*this);

      power (p, tmp1, tmp2);
}

void matrix::swap (matrix& left, matrix& right) {
      std::swap (left.N, right.N);
      std::swap (left.mat, right.mat);
}


unsigned long long power (unsigned long long x, unsigned long long p) {
      if (p == 0)
            return 1;

      if (p == 1)
            return x;
      if (p % 2)
            return (x*power(x, p-1)) % MODULE;
      unsigned long long ans = power(x, p/2);
      ans *= ans;

      return ans % MODULE;
}

std::pair<unsigned long long, unsigned long long> solve (std::vector<unsigned long long> A) {
      matrix M (4);

      M[0] = 0;
      M[1] = 1;
      M[2] = 2;
      M[3] = 1;

      unsigned long long x = 2;

      for (unsigned long long i = 0; i < A.size(); ++i) {
            if (A[i] == 0)
                  return std::pair<unsigned long long, unsigned long long> (1, 1);

            M.power(A[i]);
            x = power(x, A[i]);
      }

      unsigned long long y = M[0];

      y *= ((MODULE+1) / 2);
      y %= MODULE;

      x *= ((MODULE+1) / 2);
      x %= MODULE;

      return std::pair<unsigned long long, unsigned long long> (y, x);
}

int main () {
      unsigned long long n;
      std::cin >> n;

      std::vector<unsigned long long> A (n);

      for (unsigned long long i = 0; i < n; ++i) {
            std::cin >> A[i];
      }

      std::pair<unsigned long long, unsigned long long> ans = solve (A);
      std::cout << ans.first << '/' << ans.second << std::endl;
}
