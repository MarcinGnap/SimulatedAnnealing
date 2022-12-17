#include "SimulatedAnnealing.h"
#include "Timer.h"



SimulatedAnnealing::SimulatedAnnealing() {

}


SimulatedAnnealing::~SimulatedAnnealing() {

}


/*
 * Algorytm symulowanego wy¿arzania
 * Algorytm rozpoczyna dzia³anie od temperatury pocz¹tkowej. W trakcie dzia³ania stopniowo obni¿ana jest temperatura.
 * Algorytm zaczyna od pewnego rozwi¹zania pocz¹tkowego i w kolejnych iteracjach zamienia miejscami losowe elementy trasy.
 * Jeœli po zamianie, trasa jest lepsza – krótsza, zostaje ona zapisana jako dotychczasowo najlepsza, jeœli nie,
 * odrzucamy j¹ i wykonujemy zamianê elementów ponownie. Aby podczas tych zamian, algorytm nie utkn¹³ w minimum lokalnym,
 * dopuszczane jest tak¿e przyjêcie z pewnym prawdopodobieñstwem gorszego od dotychczas najlepszego rozwi¹zania.
 */

void SimulatedAnnealing::settingsSimulatedAnnealing(double maxTemperature, double minTemperature) {

	this->maxTemperature = maxTemperature;
	this->minTemperature = minTemperature;
}



int SimulatedAnnealing::calculateCost(vector<int> &currentPath) {

	int cost = 0;

	// zliczanie drogi dla danego zbioru wierzcho³ków
	for (int i = 0; i < matrixSize - 1; i++) {
		cost += matrix[currentPath[i]][currentPath[i + 1]];
	}

	// dodanie drogi z ostatniego wierzcho³ka do pierwszego
	cost += matrix[currentPath[matrixSize - 1]][currentPath[0]];

	return cost;
}



void SimulatedAnnealing::shufflePath(vector<int> &currentPath) {

	auto rng = std::default_random_engine{};

	// pomocnicza œcie¿ka
	vector<int> num;
	num.resize(matrixSize);

	// wype³nienie wektora
	iota(num.begin(), num.end(), 0);

	// przetasowanie œcie¿ki
	shuffle(begin(num), end(num), rng);

	// przypisanie (bez pomocniczej œcie¿ki by³y b³êdy)
	currentPath = num;

	for (int i = 0; i < matrixSize; i++) {

		// ustawienie pocz¹tkowego miasta jako 0 po przetasownaiu
		if (currentPath[i] == 0) {

			auto temp = currentPath[0];
			currentPath[0] = 0;
			currentPath[i] = temp;
		}
	}

	num.clear();
}



bool SimulatedAnnealing::probability(int length1, int length2, double temperature) {

	// prawdopodobieñstwo
	double p = exp((-1 * ((long long)length2 - (long long)length1)) / temperature);

	// wybranie liczby z przedzia³u <0,1>
	double r = (double)rand() / RAND_MAX;

	// jezeli prawdopodobienstwo ze wzoru jest wieksze od losowo wybranej liczby
	return (r < p);
}



double SimulatedAnnealing::algorithmSimulatedAnnealing(vector<vector<int>> originalMatrix, vector<int> &bestPath, int &bestCost) {

	Timer timer;

	// rozpoczêcie mierzenia czasu
	timer.start();

	// wartoœci pocz¹tkowe
	foundOptimum = INT_MAX;
	matrix = originalMatrix;
	matrixSize = originalMatrix.size();

	int vertex1, vertex2;
	double cost1, cost2;

	vector<int> permutation1(matrixSize);
	vector<int> permutation2(matrixSize);

	// zaczynamy od maksymalnej temperatury
	currentTemperature = maxTemperature;

	// beta
	double beta = (maxTemperature - minTemperature) / (1000000 * maxTemperature * minTemperature);

	// pierwsza permutacja miast i jej koszt
	shufflePath(permutation1);
	cost1 = calculateCost(permutation1);

	// zapasowa permutacja
	permutation2 = permutation1;

	time_t check = 0;

	// wykonywanie przez okreœlony czas lub do osi¹gniêcia minimalnej temperatury
	while (currentTemperature > minTemperature) {

		// epoka: liczba kroków, po której zmniejszana jest temperatura
		for (int era = 0; era < 100; ++era) {

			do {
				// losowanie 2 miast (ró¿nych i nierównych 0)
				vertex1 = rand() % matrixSize;
				vertex2 = rand() % matrixSize;

			} while (vertex1 == vertex2 || vertex1 == 0 || vertex2 == 0);

			// zamiana miast
			permutation2[vertex2] = permutation1[vertex1];
			permutation2[vertex1] = permutation1[vertex2];

			// koszt nowej permutacji miast
			cost2 = calculateCost(permutation2);

			// je¿eli jest lepszy od poprzedniej lub prawdopodobieñstwo == true
			if (cost2 <= cost1 || probability(cost1, cost2, currentTemperature)) {

				// nowe minimum lokalne
				cost1 = cost2;

				// je¿eli jest mniejsze od minimum globalnego
				if (cost1 <= foundOptimum) {

					foundOptimum = cost1;
					path = permutation2;
				}

				// przepisanie lepszej permutacji
				permutation1[vertex1] = permutation2[vertex1];
				permutation1[vertex2] = permutation2[vertex2];
			}
			else {
				// przepisanie aktualnej permutacji
				permutation2[vertex1] = permutation1[vertex1];
				permutation2[vertex2] = permutation1[vertex2];
			}
		}

		// zmiana temperatury
		currentTemperature = (currentTemperature / (1 + beta * currentTemperature));

		// pokazuje aktualny wynik co sekundê (dane do wykresów)

		//        if (timer.stop() > check) {
		//
		//            cout << "po " << timer.stop() << "s: " << foundOptimum << endl;
		//            check += 1;
		//        }
	}

	// dodanie pocz¹tkowego wierzcho³ka na koniec cyklu
	path.push_back(0);

	permutation1.clear();
	permutation2.clear();

	// zwrócenie wyników
	bestPath = path;
	bestCost = foundOptimum;

	return timer.stop();
}