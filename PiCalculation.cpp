#include <random>
#include <chrono>
#include <iostream>
#include <omp.h>

void serialPiCalculation(long number_of_points);
void parallelPiCalculation(long number_of_points, int number_of_threads);

int main(int argc, char** argv)
{
	std::cout << "Program started..." << std::endl;
	int number_of_threds = std::atoi(argv[1]);
	long number_of_points = std::atol(argv[2]);
	std::cout << "number of threads: " << number_of_threds << std::endl;
	std::cout << "number of points: " << number_of_points << std::endl;
	serialPiCalculation(number_of_points);
	parallelPiCalculation(number_of_points, number_of_threds);
	std::cout << "\n\n\n\n";
	return 0;
}

void serialPiCalculation(long number_of_points) {
	std::cout << "serial pi calculation started..." << std::endl;
	srand(time(NULL));
	int inner = 0;
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < number_of_points; i++) {
		float x = ((float)rand()) / (float)RAND_MAX;
		float y = ((float)rand()) / (float)RAND_MAX;
		if ((x * x + y * y) <= 1) {
			inner++;
		}
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds " << std::endl;
	std::cout.precision(10);
	std::cout << 4.0f * ((float)inner / (float)number_of_points) << std::endl;
}

void parallelPiCalculation(long number_of_points, int number_of_threads) {
	std::cout << "parallel pi calculation started..." << std::endl;
	omp_set_num_threads(number_of_threads);
	std::chrono::steady_clock::time_point start, end;
	float x, y;
	int inner = 0;
	start = std::chrono::high_resolution_clock::now();
#pragma omp parallel
	{
		unsigned int seed = ((unsigned)time(NULL) & 0xFFFFFFF0) | (omp_get_thread_num() + 1);
		srand(seed);

#pragma omp for private(x,y) reduction(+:inner)
		for (int i = 0; i < number_of_points; i++) {
			x = ((float)rand()) / (float)RAND_MAX;
			y = ((float)rand()) / (float)RAND_MAX;
			if ((x * x + y * y) <= 1) {
				inner++;
			}
		}

	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds " << std::endl;
	std::cout.precision(10);
	std::cout << 4.0f * ((float)inner / (float)number_of_points) << std::endl;

}