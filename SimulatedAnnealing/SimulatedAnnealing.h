#pragma once
#include "Graph.h"

using namespace std;


class SimulatedAnnealing {

private:

	// pocz�tkowa temperatura
	double maxTemperature;

	// minimalna temperatura
	double minTemperature;

	// aktualna temperatura
	double currentTemperature;

	// czas wykonania

	// macierz
	vector<vector<int>> matrix;

	// liczba miast
	int matrixSize;

	// aktualnie optymalna �cie�ka
	vector<int> path;

	// najmniejszy znaleziony koszt
	int foundOptimum;

	// obliczanie kosztu �cie�ki
	int calculateCost(vector<int> &currentPath);

	// losowanie kolejno�ci miast
	void shufflePath(vector<int> &currentPath);

	// prawdopodobienstwo przyj�cia gorszego rozwi�zania
	bool probability(int length1, int length2, double temperature);


public:

	// konstruktor
	SimulatedAnnealing();

	// destruktor
	~SimulatedAnnealing();

	// algorytm Simulated Annealing
	double algorithmSimulatedAnnealing(vector<vector<int>> originalMatrix, vector<int> &bestPath, int &bestCost);

	int getfoundOptimum();

	// ustawienia Simulated Annealing
	void settingsSimulatedAnnealing(double initialTemperature, double minTemperature);
};