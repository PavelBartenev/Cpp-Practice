#include <iostream>

template <class T>
class List {
public:
	List() = default;

	List(size_t count, const T& value) {
		Node* first_node = new Node(value);

		start = end = first_node;
		
		for (size_t i = 0; i < count - 1; ++i)
			push_back(value);
	}

	List(const List& other) {
		Node* cur_node = other.start;
		Node* first_node = new Node(*cur_node);

		start = end = first_node;

		while (cur_node != other.end) {
			cur_node = cur_node->next;

			push_back(cur_node->data);
		}
	}

	void push_back(const T& value) {
		Node* new_node = new Node(value);

		new_node->prev = end;
		end->next = new_node;
		end = new_node;
	}

	void push_back(T&& value) {
		Node* new_node = new Node(std::move(value));

		new_node->prev = end;
		end->next = new_node;
		end = new_node;
	}

	~List() {
		Node* cur_node = start;

		while (cur_node != end) {
			cur_node = cur_node->next;
			delete cur_node->prev;
		}

		delete cur_node;
	}


private:
	struct Node {
		T data;

		Node() = default;

		Node(const T& data_, Node* next_ = nullptr, Node* prev_ = nullptr) :
		     data {data_},
			 next {next_},
			 prev {prev_}{}

		Node(T&& data_, Node* next_ = nullptr, Node* prev_ = nullptr) :
			data {std::move(data_)},
			next {next_},
			prev {prev_}{}

		Node(const Node& other) :
			data{other.data}{}

		Node(Node&& other) :
			data{std::move(other.data)}{}

		Node* next = nullptr;
		Node* prev = nullptr;
	};

	Node* start = nullptr;
	Node* end = nullptr;
};



int main()
{
	List<int> my_list(5, 10);

    std::cout << "Hello World!\n";
}


