#include "SimulatedAnnealing.h"
#include "Timer.h"


SimulatedAnnealing::SimulatedAnnealing() {

}


SimulatedAnnealing::~SimulatedAnnealing() {

}


void SimulatedAnnealing::settingsSimulatedAnnealing(double maxTemperature, double minTemperature) {

	this->maxTemperature = maxTemperature;
	this->minTemperature = minTemperature;
}


int SimulatedAnnealing::calculateCost(vector<int> &currentPath) {
	int cost = 0;

	for (int i = 0; i < matrixSize - 1; i++) {
		cost += matrix[currentPath[i]][currentPath[i + 1]];
	}
	cost += matrix[currentPath[matrixSize - 1]][currentPath[0]];

	return cost;
}


void SimulatedAnnealing::shufflePath(vector<int> &currentPath) {
	auto rng = std::default_random_engine{};

	vector<int> num;
	num.resize(matrixSize);

	iota(num.begin(), num.end(), 0);

	shuffle(begin(num), end(num), rng);

	currentPath = num;

	for (int i = 0; i < matrixSize; i++) {
		if (currentPath[i] == 0) {
			auto temp = currentPath[0];
			currentPath[0] = 0;
			currentPath[i] = temp;
		}
	}
	num.clear();
}


bool SimulatedAnnealing::probability(int length1, int length2, double temperature) {
	double p = exp((-1 * ((long long)length2 - (long long)length1)) / temperature);

	double r = (double)rand() / RAND_MAX;

	return (r < p);
}


double SimulatedAnnealing::algorithmSimulatedAnnealing(vector<vector<int>> originalMatrix, vector<int> &bestPath, int &bestCost, int initEra, double dA) {
	Timer timer;

	timer.start();

	foundOptimum = INT_MAX;
	matrix = originalMatrix;
	matrixSize = originalMatrix.size();

	int vertex1, vertex2;
	double cost1, cost2;

	vector<int> permutation1(matrixSize);
	vector<int> permutation2(matrixSize);

	currentTemperature = maxTemperature;

	
	shufflePath(permutation1);
	cost1 = calculateCost(permutation1);

	permutation2 = permutation1;

	time_t check = 0;

	while (currentTemperature > minTemperature) {
		for (int era = 0; era < initEra; ++era) {

			do {
				vertex1 = rand() % matrixSize;
				vertex2 = rand() % matrixSize;

			} while (vertex1 == vertex2 || vertex1 == 0 || vertex2 == 0);

			permutation2[vertex2] = permutation1[vertex1];
			permutation2[vertex1] = permutation1[vertex2];

			cost2 = calculateCost(permutation2);

			if (cost2 <= cost1 || probability(cost1, cost2, currentTemperature)) {
				cost1 = cost2;

				if (cost1 <= foundOptimum) {

					foundOptimum = cost1;
					path = permutation2;
				}
				permutation1[vertex1] = permutation2[vertex1];
				permutation1[vertex2] = permutation2[vertex2];
			}
			else {
				permutation2[vertex1] = permutation1[vertex1];
				permutation2[vertex2] = permutation1[vertex2];
			}
		}
		currentTemperature = currentTemperature * dA;
	}
	path.push_back(0);

	permutation1.clear();
	permutation2.clear();

	bestPath = path;
	bestCost = foundOptimum;

	return timer.stop();
}


int SimulatedAnnealing::getfoundOptimum() {
	return foundOptimum;
}