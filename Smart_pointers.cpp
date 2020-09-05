#include <iostream>


template <class T, class Deleter = std::default_delete<T>>
class unique_ptr {
	T* ptr;
	Deleter deleter;

public:
	unique_ptr() :
		ptr{ nullptr } {}

	explicit unique_ptr(T* pointer) :
		ptr{ pointer } {}

	explicit unique_ptr(T* pointer, const Deleter& deleter):
		ptr{pointer},
		deleter{deleter}{}

	unique_ptr(unique_ptr&& other) :
		ptr{ other.ptr } {
		other.ptr = nullptr;
	}

	unique_ptr& operator =(unique_ptr&& other) {
		deleter(ptr);

		ptr = other.ptr;
		deleter = std::move(other.deleter);

		other.ptr = nullptr;

		return *this;
	}
	

	unique_ptr(const unique_ptr& other) = delete;

	unique_ptr& operator =(const unique_ptr& other) = delete;

	T* get() const {
		return ptr;
	}

	T& operator *() const {
		return *ptr;
	}

	T* operator ->() const {
		return ptr;
	}

	~unique_ptr() {
		if (ptr != nullptr)
			deleter(ptr);
	}

	T* release() {
		T* ptr_copy = ptr;
		ptr = nullptr;

		return ptr_copy;
	}

	void reset(T* new_ptr = nullptr) {
		std::swap(ptr, new_ptr);

		if (new_ptr != nullptr)
			deleter(new_ptr);
	}
};

template <class T, class... Args>
unique_ptr<T> make_unique(Args&&... args) {
	return unique_ptr<T>(new T(std::forward<Args>(args)...));
}





struct Counter {
	int shared_counter;
	int weak_counter;

	Counter(int shared, int weak):
		shared_counter{shared},
		weak_counter{weak}{}
};


template <class T>
class weak_ptr {
	T* ptr;
	Counter* counter;
public:
	void destruct() {
		if (counter->shared_counter == 0 && counter->weak_counter == 0)
			delete counter;
		else
			--counter->weak_counter;
	}
	weak_ptr(T* pointer) :
		ptr{pointer},
		counter(new Counter(0, 1)){}

	weak_ptr(const weak_ptr& weak) :
		ptr{ weak.ptr },
		counter{ weak.counter } {
		++counter->weak_counter;
	}

	weak_ptr(weak_ptr&& weak) :
		ptr{weak.ptr},
		counter{weak.counter}{}                         //what should they do?

	weak_ptr& operator =(const weak_ptr& weak) {
		destruct();

		ptr = weak.ptr;
		counter = weak.counter;

		++counter->weak_counter;

		return *this;
	}

	~weak_ptr() {
		destruct();
	}

	size_t use_count() const noexcept {
		return counter->shared_counter;
	}

	bool expired() const noexcept {
		return use_count();
	}

	/*shared_ptr<T> lock() {
		return expired() ? shared_ptr<T>() : shared_ptr<T>(*this);
	}*/
};













int main()
{
    std::cout << "Hello World!\n";
}