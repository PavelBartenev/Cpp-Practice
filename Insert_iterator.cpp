#include <iostream>

template <typename Container>
class Insert_iterator {
protected:
	Container* container;
	typename Container::iterator iter;

public:
	explicit Insert_iterator(Container& container, typename Container::iterator iter):
		container{&container},
		iter{iter}{}

	Insert_iterator<Container>& operator =(const typename Container::value_type& value) {
		iter = container->insert(iter, value);
		++iter;
		return *this;
	}

	Insert_iterator<Container>& operator =(typename Container::value_type&& value) {
		iter = container->insert(iter, std::move(value));
		++iter;
		return *this;
	}

	Insert_iterator<Container>& operator ++() {
		return *this;
	}

	Insert_iterator<Container>& operator ++(int) {
		return *this;
	}

	Insert_iterator<Container>& operator *() {
		return *this;
	}
};


template <typename Container>
Insert_iterator<Container> inserter(Container& container, typename Container::iterator& iter) {
	return Insert_iterator<Container>(container, iter);
}


template <class It, class Distance>
constexpr void advance(It& iter, Distance n) {
	using category = typename std::iterator_traits<It>::iterator_category;

	static_assert(std::is_base_of(std::input_iterator_tag, category));

	auto dist = typename std::iterator_traits<It>::difference_type(n);
	if constexpr (std::is_base_of(std::random_access_iterator_tag, category)) {
		iter += dist;
	}
	else {
		while (dist > 0) {
			++iter;
			--dist;
		}

		if constexpr (std::is_base_of(std::bidirectional_iterator_tag, category))
			while (dist < 0) {
				--iter;
				++dist;
			}
	}
}


template <class It>
constexpr typename std::iterator_traits<It>::difference_type distance(It first, It second) {
	using category = typename std::iterator_traits<It>::iterator_category;

	static_assert(std::is_base_of(std::input_iterator_tag, category));

	if constexpr (std::is_base_of(std::random_access_iterator_tag, category))
		return second - first;

	typename std::iterator_traits<It>::difference_type dist = 0;

	while (first != second) {
		++dist;
		++first;
	}

	return dist;
}


int main()
{
    std::cout << "Hello World!\n";
}
