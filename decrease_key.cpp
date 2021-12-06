/*
Реализуйте двоичную кучу.

Обработайте запросы следующих видов:

	insert x — вставить целое число x в кучу;

	getMin — вывести значение минимального элемента в куче (гарантируется, что к этому моменту куча не пуста);

	extractMin — удалить минимальный элемент из кучи, выводить его не нужно (гарантируется, что к этому моменту куча не пуста);

	decreaseKey i Δ — уменьшить число, вставленное на i-м запросе, на целое число Δ ≥ 0 (гарантируется, что i-й запрос был осуществлён ранее, являлся запросом добавления, а добавленное на этом шаге число всё ещё лежит в куче). Обратите внимание, число i равно номеру запроса среди всех запросов, а не только среди запросов добавления!
*/

#include <bits/stdc++.h>
#include <iostream>
#include <string>

namespace dst {
template <class T>
	bool cmp_default (T first, T second) { return first < second; }

template <class T>
	class heap {
		public:
			heap (bool (*cmp_function) (T, T) = cmp_default<T>);
			
			heap (T* extern_array, unsigned int size,
				bool (*cmp_function) (T, T) = cmp_default<T>);

			~heap (void);

			void heapify (T* extern_array, unsigned int size);
			void heapify (void);
			
			T& extract (void); // Extract minimal element from heap
			unsigned int insert (T& element); // Insert element and return its key
			const T& top (void);

			T* sort (void);
			unsigned int size (void);

			const T& get_by_key (unsigned int key);

			void decrease_key (unsigned int key, const T& delta);

			void display (void);	
		private:
	
			bool (*is_less) (T, T);
			
			T* heap_array;
			unsigned int heap_size;
			unsigned int array_size;

			unsigned int* key_array;
			unsigned int key_size;
			unsigned int last_key;
			
			unsigned int* keys;

			bool is_external = false;

			unsigned int parent (unsigned int idx);

			void sift_up (unsigned int idx);
			void sift_down (unsigned int idx);

			void expand (void);

			void heap_swap (unsigned int first_idx, unsigned int second_idx);
	};

template <class T>
	heap<T>::heap (bool (*cmp_function) (T, T)) {
		
		this->is_less = cmp_function;

		this->heap_array = new T[2];
		this->heap_size = 0;
		this->array_size = 2;

		this->keys = new unsigned int [2];

		this->key_array = new unsigned int [2];
		this->key_size = 2;
		this->last_key = 0;
	
		return;
	}

template <class T>
	heap<T>::heap (T* extern_array, unsigned int size, bool (*cmp_function) (T, T)) {
		
		this->is_less = cmp_function;

		this->heap_array = extern_array;
		this->heap_size = size;
		this->array_size = size;

		this->keys = new unsigned int [size];

		for (unsigned int i=0; i < size; i++) {

			this->keys[i] = i;
		}

		this->key_array = new unsigned int [size];
		this->key_size = size;
		this->last_key = size;

		for (unsigned int i=0; i < size; i++) {
			
			this->key_array[i] = i;
		}

		this->is_external = true;

		return;
	}

template <class T>
	heap<T>::~heap (void) {

		if (!this->is_external) {
		
			delete [] this->heap_array;
		}

		delete this->key_array;

		delete this->keys;

		return;
	}

template <class T>
	unsigned int heap<T>::parent (unsigned int idx) {
		return (idx - 1) / 2;
	}

template <class T>
	void heap<T>::heap_swap (unsigned int first, unsigned int second) {

		T swap = this->heap_array[first];

		unsigned int key_first = this->keys[first];
		unsigned int key_second = this->keys[second];
		unsigned int key_swap = this->key_array[key_first];

		this->heap_array[first] = this->heap_array[second];
		this->key_array[key_first] = this->key_array[key_second];

		this->heap_array[second] = swap;
		this->key_array[key_second] = key_swap;

		this->keys[first] = key_second;

		this->keys[second] = key_first;
		
		return;
	}

template <class T>
	void heap<T>::sift_up (unsigned int idx) {
		
		while ( (idx > 0) && (this->is_less (heap_array[idx],
						heap_array[this->parent (idx)])) ) {

			this->heap_swap (idx, this->parent(idx));
			idx = this->parent(idx);
		}

		return;
	}

template <class T>
	void heap<T>::sift_down (unsigned int idx) {
		
		unsigned int first_child;
		unsigned int second_child;
		unsigned int min_child;

		while ( (second_child = 2 * idx + 2) < this->heap_size ) {
			
			first_child = 2 * idx + 1;
			min_child = first_child;

			if (this->is_less (this->heap_array[second_child],
						this->heap_array[first_child])) {
				
				min_child = second_child;
			}

			if (this->is_less (this->heap_array[idx],
						this->heap_array[min_child])) {
				break;
			}

			this->heap_swap (idx, min_child);
			idx = min_child;
		}

		first_child = 2 * idx + 1;

		if ( (first_child < heap_size) &&
			(this->is_less (this->heap_array[first_child],
					this->heap_array[idx])) ) {
			
			this->heap_swap (idx, first_child);
		}

		return;
	}

template <class T>
	void heap<T>::heapify (T* extern_array, unsigned int size) {

		this->heap_array = extern_array;
		this->heap_size = size;
		this->array_size = size;

		delete this->key_array;

		this->key_array = new unsigned int [size];
		this->key_size = size;
		this->last_key = size;

		for (unsigned int i=0; i < this->heap_size; i++) {

			this->key_array[i] = i;
		}

		for (unsigned int i = this->heap_size; i > 0; i--) {
			
			this->sift_down (i - 1);
		}
		
		return;
	}

template <class T>
	void heap<T>::heapify (void) {
		
		for (unsigned int i = this->heap_size; i > 0; i--) {

			this->sift_down (i - 1);
		}

		return;
	}

template <class T>
	unsigned int heap<T>::insert (T& element) {
		
		this->expand();

		this->heap_array[heap_size] = element;
		this->keys[heap_size] = this->last_key;

		this->key_array[this->last_key] = this->heap_size;

		++(this->heap_size);
		++(this->last_key);

		this->sift_up (this->heap_size - 1);

		return this->last_key - 1;
	}

template <class T>
	T& heap<T>::extract (void) {
		
		this->heap_swap (0, this->heap_size - 1);

		--(this->heap_size);
		this->sift_down(0);

		return this->heap_array[this->heap_size];
	}

template <class T>
	const T& heap<T>::top (void) {
		
		return this->heap_array[0];
	}

template <class T>
	void heap<T>::expand (void) {
		
		if (this->last_key == this->key_size) {

			this->key_size = (this->key_size * 3 + 1) / 2;

			unsigned int* new_keys = new unsigned int [this->key_size];

			for (unsigned int i=0; i < this->last_key; i++) {

				new_keys[i] = this->key_array[i];
			}

			delete this->key_array;

			this->key_array = new_keys;
		}

		if (this->is_external) {
			return;
		}

		if (this->heap_size == this->array_size) {

			this->array_size = (this->array_size * 3 + 1) / 2;

			T* new_array = new T[this->array_size];
			unsigned int* new_keys = new unsigned int [this->array_size];

			for (unsigned int i=0; i < this->heap_size; i++) {

				new_array[i] = this->heap_array[i];
				new_keys[i] = this->keys[i];
			}

			delete [] this->heap_array;
			delete [] this->keys;

			this->heap_array = new_array;
			this->keys = new_keys;
		}

		return;
	}

template <class T>
	T* heap<T>::sort (void) {

		while (this->heap_size > 0) {
			this->extract();
		}

		return this->heap_array;
	}

template <class T>
	unsigned int heap<T>::size (void) {

		return this->heap_size;
	}

template <class T>
	const T& heap<T>::get_by_key (unsigned int key) {

		unsigned int idx = this->key_array[key];

		return this->heap_array[idx];
	}

template <class T>
	void heap<T>::decrease_key (unsigned int key, const T& delta) {

		unsigned int idx = this->key_array[key];

		this->heap_array[idx] -= delta;

		this->sift_up (idx);
//			this->sift_down (idx);

		return;
	}

}


namespace dst {
template <class T>
	void heap<T>::display (void) {

		for (unsigned int n=1; n <= this->heap_size; n <<= 1) {
			std::cout << n << ": ";
			for (int i=n-1; (i < (n << 1) - 1) && (i < this->heap_size); i++)
				std::cout << heap_array[i] << ' ';
			std::cout << '\n';
		}

	}
}

int main () {
std::ios_base::sync_with_stdio(false);
std::cin.tie(NULL);

unsigned int q;
std::cin >> q;


std::string quiery;
dst::heap<long long> heap;
long long d;
unsigned int *keys = new unsigned int [q];

for (unsigned int i=0; i < q; i++) {
	std::cin >> quiery;
	if (quiery == "insert") {
		std::cin >> d;
		unsigned int k = heap.insert(d);
		keys[i] = k;
	}
	else if (quiery == "extractMin") {
		heap.extract() << '\n';
	}
	else if (quiery == "getMin") {
		std::cout << heap.top() << '\n';
	}
	else if (quiery == "decreaseKey") {
		long long key;
		std::cin >> key >> d;

		--key;
		heap.decrease_key(keys[key], d);
	}
}

}
