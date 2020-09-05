#include <iostream>

template <class T>
class optional {
	T data;
	bool existence = false;

public:

	optional() = default;

	optional(const T& data) : data{ data }, existence{ true } {};

	optional& operator =(const optional& obj) const {
		existence = obj.existence;

		if (existence)
			data = obj.data;
	}

	~optional() = default;

	T& operator *() const {
		return data;
	}

	T& operator ->() const {
		return data;
	}

	bool has_value() const {
		return existence;
	}

	T value() const {
		return value;
	}

	template<class U>
	T value_or(U&& other_value) const {
		return existence ? data : other_value;
	}

	template <class... Args>
	T& emplace(Args& ... args) {
		if (existence)
			data.~T();
		
		

	}
};



int main()
{
    std::cout << "Hello World!\n";
}
