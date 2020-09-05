#include <iostream>
#include <exception>

template <typename T, size_t Size>
class Array {
private:
	using arr_type = T[Size];

	arr_type array;

public:
	constexpr T& operator [](size_t index) {
		return static_cast<T&>(array[index]);
	}

	constexpr const T& operator [](size_t index) const {
		return static_cast<const T&>(array[index]);
	}

	constexpr T& at(size_t index) {
		if (index >= Size)
			throw std::runtime_error("Index is out of range");

		return operator [](index);
	}

	constexpr const T& at(size_t index) const {
		if (index >= Size)
			throw std::runtime_error("Index is out of range");

		return operator [](index);
	}
	
	constexpr T& front() {
		return operator[](0);
	}

	constexpr const T& front() const {
		return operator[](0);
	}

	constexpr T& back() {
		return operator[](Size - 1);
	}

	constexpr const T& back() const {
		return operator[](Size - 1);
	}

	void fill(const T& value) {
		for (size_t i = 0; i < Size; ++i)
			operator[](i) = value;
	}

	constexpr bool empty() const noexcept {
		return Size;
	}

	constexpr bool size() const noexcept {
		return Size;
	}
};

int main()
{
	Array<int, 10> my_arr;

	my_arr.fill(56);

	my_arr[7] = 9;

	std::cout << my_arr[7] << my_arr[8];

	try {
		my_arr.at(20);
	}
	catch (std::runtime_error& dump) {
		std::cout << dump.what();
	}

	return 0;
}


