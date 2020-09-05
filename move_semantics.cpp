#include <iostream>
#include <string>





template<typename T, typename... Args>
class is_func_here {
private:
	template<typename...>
	static int f(...) {}

	template<typename TT, typename... AArgs>
	static decltype(std::declval<TT>().func(std::declval<AArgs>()...), char()) f(int) {}

public:
	static const bool value = std::is_same<decltype(f<T, Args...>(0)), char>::value;
};

struct S {
	int func(int x, int y) {
		return 0;
	}
};


int main() {
	std::cout << is_func_here<S, int, int>::value;
}








template <typename T>
constexpr typename std::remove_reference<T>::type&& move(T&& obj) noexcept {
	return static_cast<typename std::remove_reference<T>::type&&>(obj);
}

template<typename T>
void swap(T& obj1, T& obj2) {
	T temp = move(obj1);
	obj1 = move(obj2);
	obj2 = move(temp);
}

template <typename T>
constexpr T&& forward(typename std::remove_reference<T>::type& val) noexcept {
	return static_cast<T&&>(val);
}