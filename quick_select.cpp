// https://codeforces.com/group/PVbQ8eK2T4/contest/347382/submission/132409574

/*
Реализуйте алгоритм QuickSelect (среднее время работы O(N)

Входные данные:
В первой строке записаны N и K
В N последующих строках записаны числа последовательности.

Выходные данные:
K-я порядковая статистика.
*/
#include <stdio.h>

namespace dst {

template <class T>
	void swap(T& left, T& right) {
	
		T swp = left;
	
		left = right;
		right = swp;

		return;
	}

template <class T>
	class __cmp_default {
		public:
			bool operator() (const T& left, const T& right) {
				return left < right;
			}
	};

template <class T, class C = __cmp_default<T>>
            size_t __quick_select (T* array, size_t size, size_t k, C is_less = __cmp_default<T>());

template <class T, class C>
	size_t __quick_partition (T* array, size_t size, size_t mid_index, C is_less) {

		size_t i = 0;
		size_t j = size - 1;
		
		swap (array[mid_index], array[size - 1]);

		while (true) {
			while ( (i < size - 1) && (is_less (array[i], array[size-1]) ) ) { ++i; }

			while ( (j > 0) && !(is_less (array[j - 1], array[size-1]) ) ) { --j; }

			if (i == j) {
				swap (array[j], array[size-1]);
				return j;
			}

			swap (array[i], array[j-1]);
		}

		return 0; // This must mean error
	}

template <class T, class C>
	size_t __quick_s_middle (T* array, size_t size, C is_less = __cmp_default<T>()) {

		for (size_t i = 0; i < size-1; i++) {
			
			for (size_t j = size-1; j > i; j--) {
			
				if ( is_less (array[j], array[j-1]) ) {
			
					dst::swap (array[j], array[j-1]);
				}
			}
		}
		return size / 2;
	}

template <class T, class C>
	size_t __quick_middle (T* array, size_t size, C is_less, const int part_size = 5) {
		
		if (size < part_size) {
			return 0;
		}

		for (int i=0; i < size / part_size; i++) {
			__quick_s_middle<T, C> ( &array [part_size * i], part_size, is_less );
			
			swap(array[part_size * i + (part_size / 2)], array[i]);
		}
		
		return __quick_select<T, C> (array, size / part_size, size / (2*part_size), is_less);
	}

template <class T, class C>
	size_t __quick_r_middle (T* array, size_t size, C is_less) {
		return size / 2;
	}


template <class T, class C>
      size_t __quick_select (T* array, size_t size, size_t k, C is_less) {
            
            size_t left = 0;
            size_t right = size;

            while (right - left > 1) {
                  size_t middle = __quick_middle (array+left, right-left, is_less);
                  middle = left + __quick_partition<T, C> (array+left, right-left, middle, is_less);

                  if (k < middle+1) {
                        right = middle+1;
                        continue;
                  }
/*
                  if (middle == left)
                        return left;
*/
                  left = middle+1;
            }

            return left;
      }
/*
template <class T, class C>
	size_t __quick_select (T* array, size_t size, size_t k, C is_less) {
		
		if (size == 1) {
			return 0;
		}

		size_t middle = __quick_middle (array, size, is_less);
		
		middle = __quick_partition<T, C> (array, size, middle, is_less);

		if (k <= middle) {
			return __quick_select (array, middle+1, k, is_less);
		}

		return __quick_select (&array[middle+1], size-middle-1, k-middle-1, is_less) + middle + 1;
	}
// */

template <class T, class C = __cmp_default<T>>
	const T& quick_select (T* array, size_t size, size_t k, C is_less = __cmp_default<T>()) {
		
		size_t pos = __quick_select (array, size, k, is_less);

		return array[pos];
	}
}

int main() {
int N, k;

scanf("%d %d", &N, &k);

long long* array = new long long [N];

for (int i=0; i < N; i++) {
	scanf("%lld", &array[i]);
}

//printf("Here\n");

printf("%lld\n", dst::quick_select(array, N, k));

delete [] array;

return 0;
}
