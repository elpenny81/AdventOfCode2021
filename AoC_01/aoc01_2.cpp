#include <iostream>
#include <fstream>
#include <list>
#include <numeric>

int main()
{
	std::ifstream input("input.txt");
	int n, last_sum = -1, num_increases = 0;
	std::list<int> sliding_window;
	while (input >> n) {
		sliding_window.push_back(n);
		if (sliding_window.size() == 3) {
			auto sum = std::accumulate(sliding_window.begin(), sliding_window.end(), 0);
			if (last_sum != -1 && sum > last_sum)
				num_increases++;
			last_sum = sum;
			sliding_window.pop_front();
		}
	}

	std::cout << "Result = " << num_increases << std::endl;
}