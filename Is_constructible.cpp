#include <iostream>
#include <utility>
#include <vector>

template <typename T, typename U>
struct is_same {
	static const bool value = false;
};

template <typename T>
struct is_same <T, T> {
	static const bool value = true;
};

template <typename C, typename... Args>
class is_constructible {
	template<typename...>
	static void f(...) {};

	template <typename U>
	static decltype(U(std::declval<Args>()...)) f(const U&) {};

public:
	static const bool value = is_same<decltype(f<C>(std::declval<C>())), C>::value;
};



template<bool B, typename T = void>
struct enable_if{};

template<typename T>
struct enable_if<true, T> {
	using type = T;
};

template< bool B, class T = void >
using enable_if_t = typename enable_if<B, T>::type;

template <class T, typename enable_if<std::is_move_constructible<T>::value, int>::type = 0>
int func() {

}


class Mine {
	int test = 0;
public:
	Mine(int test_):
		test{test_}{}
};


class Test {
public:
	int x;
};

template <int N>
struct Fib {
	static const long long value = Fib<N - 2>::value + Fib<N - 1>::value;
};

template<>
struct Fib<0> {
	static const long long value = 0;
};

template<>
struct Fib<1> {
	static const long long value = 1;
};


template <typename T, template<typename VecType, typename A = std::allocator<VecType>> class vector = std::vector>
struct Stack {
	vector<T> v;

	void push(const T& val){
		v.push_back(val);
	}

	void pop() {
		v.pop_back();
	}

	const T& get() {
		return v.back();
	}
};

int main() {

	Stack<int, std::vector> my_stack;

	my_stack.push(6);
	my_stack.push(10);

	my_stack.pop();

	std::cout << my_stack.get();
	

	//std::cout << is_constructible<Mine, int>::value;
}

