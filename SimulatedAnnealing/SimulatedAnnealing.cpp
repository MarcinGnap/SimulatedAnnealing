#include "SimulatedAnnealing.h"
#include "Timer.h"



SimulatedAnnealing::SimulatedAnnealing() {

}


SimulatedAnnealing::~SimulatedAnnealing() {

}


/*
 * Algorytm symulowanego wy�arzania
 * Algorytm rozpoczyna dzia�anie od temperatury pocz�tkowej. W trakcie dzia�ania stopniowo obni�ana jest temperatura.
 * Algorytm zaczyna od pewnego rozwi�zania pocz�tkowego i w kolejnych iteracjach zamienia miejscami losowe elementy trasy.
 * Je�li po zamianie, trasa jest lepsza � kr�tsza, zostaje ona zapisana jako dotychczasowo najlepsza, je�li nie,
 * odrzucamy j� i wykonujemy zamian� element�w ponownie. Aby podczas tych zamian, algorytm nie utkn�� w minimum lokalnym,
 * dopuszczane jest tak�e przyj�cie z pewnym prawdopodobie�stwem gorszego od dotychczas najlepszego rozwi�zania.
 */

void SimulatedAnnealing::settingsSimulatedAnnealing(double maxTemperature, double minTemperature) {

	this->maxTemperature = maxTemperature;
	this->minTemperature = minTemperature;
}



int SimulatedAnnealing::calculateCost(vector<int> &currentPath) {

	int cost = 0;

	// zliczanie drogi dla danego zbioru wierzcho�k�w
	for (int i = 0; i < matrixSize - 1; i++) {
		cost += matrix[currentPath[i]][currentPath[i + 1]];
	}

	// dodanie drogi z ostatniego wierzcho�ka do pierwszego
	cost += matrix[currentPath[matrixSize - 1]][currentPath[0]];

	return cost;
}



void SimulatedAnnealing::shufflePath(vector<int> &currentPath) {

	auto rng = std::default_random_engine{};

	// pomocnicza �cie�ka
	vector<int> num;
	num.resize(matrixSize);

	// wype�nienie wektora
	iota(num.begin(), num.end(), 0);

	// przetasowanie �cie�ki
	shuffle(begin(num), end(num), rng);

	// przypisanie (bez pomocniczej �cie�ki by�y b��dy)
	currentPath = num;

	for (int i = 0; i < matrixSize; i++) {

		// ustawienie pocz�tkowego miasta jako 0 po przetasownaiu
		if (currentPath[i] == 0) {

			auto temp = currentPath[0];
			currentPath[0] = 0;
			currentPath[i] = temp;
		}
	}

	num.clear();
}



bool SimulatedAnnealing::probability(int length1, int length2, double temperature) {

	// prawdopodobie�stwo
	double p = exp((-1 * ((long long)length2 - (long long)length1)) / temperature);

	// wybranie liczby z przedzia�u <0,1>
	double r = (double)rand() / RAND_MAX;

	// jezeli prawdopodobienstwo ze wzoru jest wieksze od losowo wybranej liczby
	return (r < p);
}



double SimulatedAnnealing::algorithmSimulatedAnnealing(vector<vector<int>> originalMatrix, vector<int> &bestPath, int &bestCost) {

	Timer timer;

	// rozpocz�cie mierzenia czasu
	timer.start();

	// warto�ci pocz�tkowe
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

	// wykonywanie przez okre�lony czas lub do osi�gni�cia minimalnej temperatury
	while (currentTemperature > minTemperature) {

		// epoka: liczba krok�w, po kt�rej zmniejszana jest temperatura
		for (int era = 0; era < 100; ++era) {

			do {
				// losowanie 2 miast (r�nych i nier�wnych 0)
				vertex1 = rand() % matrixSize;
				vertex2 = rand() % matrixSize;

			} while (vertex1 == vertex2 || vertex1 == 0 || vertex2 == 0);

			// zamiana miast
			permutation2[vertex2] = permutation1[vertex1];
			permutation2[vertex1] = permutation1[vertex2];

			// koszt nowej permutacji miast
			cost2 = calculateCost(permutation2);

			// je�eli jest lepszy od poprzedniej lub prawdopodobie�stwo == true
			if (cost2 <= cost1 || probability(cost1, cost2, currentTemperature)) {

				// nowe minimum lokalne
				cost1 = cost2;

				// je�eli jest mniejsze od minimum globalnego
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

		// pokazuje aktualny wynik co sekund� (dane do wykres�w)

		//        if (timer.stop() > check) {
		//
		//            cout << "po " << timer.stop() << "s: " << foundOptimum << endl;
		//            check += 1;
		//        }
	}

	// dodanie pocz�tkowego wierzcho�ka na koniec cyklu
	path.push_back(0);

	permutation1.clear();
	permutation2.clear();

	// zwr�cenie wynik�w
	bestPath = path;
	bestCost = foundOptimum;

	return timer.stop();
}