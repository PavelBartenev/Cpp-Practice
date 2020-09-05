#include <iostream>


class string {
	char* str = nullptr;
	size_t len = 0;

public:

	string(const char* data, size_t size) {
		str = new char[size];
		memcpy(str, data, size);
		len = size;
	}

	string(const string& old_str) {
		str = new char[old_str.len];
		memcpy(str, old_str.str, old_str.len);
		len = old_str.len;
	}

	string& operator =(const string& old_str) {
		this->~string();
		str = new char[old_str.len];
		memcpy(str, old_str.str, old_str.len);
		len = old_str.len;
	}

	string(string&& other_str) noexcept : str{ other_str.str }, len{ other_str.len } {
		other_str.str = nullptr;
		other_str.len = 0;
	}

	string& operator = (string&& other_str) noexcept {
		str = other_str.str;
		len = other_str.len;
		other_str.str = nullptr;
		other_str.len = 0;
	}

	string& operator +(const string& other_str) const {
		char* new_arr = new char[len + other_str.len];
		
		memcpy(new_arr, str, len);
		memcpy(new_arr + len, other_str.str, other_str.len);

		string* new_string = new string (new_arr, len + other_str.len);

		delete[] new_arr;

		return *new_string;
	}

	~string() {
		delete[] str;
		len = 0;
	}

	friend std::ostream& operator <<(std::ostream& output, const string& str);

	friend std::ostream& operator >>(std::istream& input, string& str);
};

std::ostream& operator <<(std::ostream& output, const string& str) {
	for (int i = 0; i < str.len; ++i)
		output << str.str[i];

	return output;
}




int main()
{
	const char* h = "Hello";
	const char* w = ", world!";

	string first_str(h, 5);
	string second_str(w, 8);

	string hello = first_str;
	string word = second_str;

	std::cout << (first_str + second_str) << " " << (hello + word);

	return 0;
}


