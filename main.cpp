#include <iostream>
#include <vector>

int	main()
{
	std::vector<int>	vTest(5, 42);

	std::cout << vTest.capacity() << std::endl;
	return (0);
}
