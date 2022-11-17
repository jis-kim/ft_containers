#include <memory>
#include <iostream>

int main(void) {
	std::allocator<int> a, b;
	
	int *asdf;
	try {
		a.construct(asdf, 1);
	}catch (std::exception& e) {
		std::cout << e.what() << '\n';
	}

	if (a == b) std::cout << "equal!\n";
}

