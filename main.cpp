#include <iostream>
#include <vector>

int	main()
{
	std::vector<std::string>	vTest;

	std::cout << vTest.max_size() << std::endl;
	std::cout << (((size_t) - 1) / sizeof(std::string)) / 2 << std::endl;
	return (0);
}
