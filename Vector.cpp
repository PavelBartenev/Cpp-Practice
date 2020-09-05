#include <iostream>

template <class T, class Allocator = std::allocator<T>>
class vector {

	T* start;
	T* end_;
	T* storage;
	Allocator allocator;

	enum vector_const {
		base_size = 8,
	};

public:

	vector(const Allocator& alloc = Allocator()) :
		start     { std::allocator_traits<Allocator>::allocate(allocator, base_size) },
		end_       { start + 1 },
		storage   { start + base_size },
		allocator { alloc } {}

	vector(const vector& other) :
		start     { std::allocator_traits<Allocator>::allocate(allocator, other.storage - other.start) },
		end_       { start + (other.end_ - other.start) },
		storage   { start + (other.storage - other.start)},
		allocator {std::allocator_traits<Allocator>::select_on_container_copy_construction(other.allocator)} {

		for (int i = 0; i < (other.end_ - other.start); ++i)
			std::allocator_traits<Allocator>::construct(allocator, start + i, other.start[i]);
	}
	
	vector& operator =(const vector& other){
		if (this == &other)
			return *this;

		for (int i = 0; i < end_ - start; ++i)
			std::allocator_traits<Allocator>::destroy(allocator, start + i);

		bool allocator_copy_required = std::allocator_traits<Allocator>::propagate_on_container_copy_assignment::value;

		bool realloc_required = (allocator != other.allocator && allocator_copy_required) || (end_ - start < other.end_ - other.start);

		if (realloc_required)
			std::allocator_traits<Allocator>::deallocate(allocator, start, storage - start);

		if (allocator_copy_required)
			allocator = other.allocator;

		if (realloc_required) {
			start = std::allocator_traits<Allocator>::allocate(allocator, other.storage - other.start);
			storage = other.storage;
		}

		end_ = start + (other.end_ - other.start);

		for (int i = 0; i < end_ - start; ++i)
			std::allocator_traits<Allocator>::construct(allocator, start + i, other.start[i]);

		return *this;
	}

	void destruct() {
		for (int i = 0; i < end_ - start; ++i)
			std::allocator_traits<Allocator>::destroy(allocator, start + i);

		std::allocator_traits<Allocator>::deallocate(allocator, start, storage - start);
	}

	~vector() {
		destruct();
	}

	vector(vector&& other) :
		start     {other.start},
		end_       {other.end_},
		storage   {other.storage},
		allocator { std::move(other.allocator)} {

		other.start = other.end_ = other.storage = nullptr;
	}

	vector& operator =(vector&& other) {
		if (this == &other)
			return *this;

		bool alloc_copy_required = std::allocator_traits<allocator>::propagate_on_move_assignment::value;

		if (allocator != other.allocator && alloc_copy_required == false) {
			
			for (int i = 0; i < end_ - start; ++i)
				std::allocator_traits<Allocator>::destroy(allocator, start + i);

			if (other.end_ - other.start > end_ - start) {
				std::allocator_traits<Allocator>::deallocate(allocator, start, storage - start);

				start = std::allocator_traits<Allocator>::allocate(allocator, other.storage - other.start);
			}

			end_ = start + (other.end_ - other.start);
			storage = start + (other.storage - other.start);

			for (int i = 0; i < end_ - start; ++i)
				std::allocator_traits<Allocator>::construct(allocator, start + i, other.start[i]);

			//other.destruct() - for what??
		}
		else {
			destruct();

			start = other.start;
			end_ = other.end_;
			storage = other.storage;

			if (alloc_copy_required)
			    allocator = std::move(other.allocator);

			other.start = other.end_ = other.storage = nullptr;
		}

		return *this;
	}


private:

	template <typename... Arguments>
	void realloc_append(Arguments&& ... elem) {
		size_t new_cap = (storage - start) * 2;

		T* new_start = std::allocator_traits<Allocator>::allocate(allocator, new_cap);

		std::allocator_traits<Allocator>::construct(allocator, new_start + (end_ - start), std::forward<Arguments>(elem)...);

		new (new_start + (end_ - start)) Arguments(std::forward<Arguments>(elem)...);
		for (int i = 0; i < end_ - start; ++i)
			std::allocator_traits<Allocator>::construct(allocator, new_start + i, std::move_if_noexcept(start[i])); // std::move?

		destruct();

		end_ = new_start + (end_ - start) + 1;
		start = new_start;
		storage = start + new_cap;
	}

public:

	void push_back(const T& elem) {
		if (end_ == storage) {
			realloc_append(elem);
			return;
		}

		std::allocator_traits<Allocator>::construct(allocator, end_, elem); // why (end_ - 1) ? 
		++end_;
	}

	void push_back(T&& elem) {
		if (end_ == storage) {
			realloc_append(std::move_if_noexcept(elem));
			return;
		}

		std::allocator_traits<Allocator>::construct(allocator, end_, std::move(elem)); //std::move ?
		++end_;
	}
	
	template <typename... Arguments>
	void emplace_back(Arguments&& ... args) {
		if (end_ == storage) {
			realloc_append(std::forward<Arguments>(args)...);
		}
		else {
			std::allocator_traits<Allocator>::construct(allocator, end_, std::forward<Arguments>(args)...);
			++end_;
		}
	}



	T& operator [](size_t index) {
		return start[index];
	}

	const T& operator [](size_t index) const {
		return start[index];
	}

	T& front() {
		return start[0];
	}

	T& back() {
		return *(end_ - 1);
	}

	const T& front() const {
		return start[0];
	}

	const T& back() const {
		return *(end_ - 1);
	}

	bool empty() const noexcept {
		return start == end_ - 1;
	}

	size_t size() const noexcept {
		return end_ - start - 1;
	}

	size_t capacity() const noexcept {
		return storage - start - 1;
	}
};



template <>
class vector<bool>
{
	using proxy_type = unsigned long;

	proxy_type* start;
	size_t size;
	size_t storage;

	struct BoolProxy
	{
		proxy_type* block;
		size_t index;

		BoolProxy() = delete;

		explicit BoolProxy(proxy_type* block_, size_t index_) :
			block{ block_ },
			index{ index_ }{}

		static void set_bit(proxy_type* block, size_t index, bool new_elem) {
			if (new_elem)
				* block |= (0x01 << (8 * sizeof(proxy_type) - index - 1));
			else
				*block &= (0x00 << (8 * sizeof(proxy_type) - index - 1));
		}

		void set_bit(bool new_elem) {
			set_bit(block, index, new_elem);
		}

		BoolProxy& operator =(bool elem) {
			set_bit(block, index, elem);
			return *this;
		}

		bool bit() {
			return (*block >> index) & 0x01;
		}

		operator bool() {
			return bit();
		}

	};

	enum bool_vector {
		Base_size = 10,
	};

	vector() :
		start   {new proxy_type[Base_size]},
		size    { 0 },
		storage {Base_size * 8}{}

	BoolProxy operator[](size_t index) {
		return BoolProxy(start + (index / sizeof(proxy_type)), index % (8 * sizeof(proxy_type)));
	}
};


int main()
{
	vector<int> my_vec;
	int x = 200;

	my_vec.push_back(100);
	my_vec.push_back(x);

	std::cout << my_vec.back();

	return 0;
}

