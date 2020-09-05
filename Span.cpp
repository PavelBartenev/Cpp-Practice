#include <iostream>


template <typename T>
class Span {
	T* data_m = nullptr;
	size_t size_m = 0;

public:

	Span() = default;

	Span(T* data, size_t size) : data_m{ data }, size_m{ size }{};

	Span(const Span& other) = default;

	Span& operator =(const Span& old_span) {
		data_m = old_span.data_m;
		size_m = old_span.size_m;
	}

	~Span() = default;

	T& operator [](size_t index) {
		return data_m[index];
	}

	const T& operator[](size_t index) const {
		return data_m[index];
	}

	const T& front() const {
		return data_m[0];
	}

	const T& end() const {
		return data_m[size_m - 1];
	}

	Span<T> subspan(size_t offset, size_t size) const {
		return Span(data_m + offset, size);
	}

};


int main()
{
	int* arr = new int[100]();

	for (int i = 0; i < 100; ++i)
		arr[i] = i;

	Span<int> my_span{ arr, 100 };

	Span<int> other_span = my_span;

	std::cout << other_span[4];
	std::cout << other_span.end();
	std::cout << other_span.front();

	return 0;
}

