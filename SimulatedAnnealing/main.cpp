#include "Reader.h"
#include "timeMeasurement.h"
#include "Graph.h"
#include "SimulatedAnnealing.h"
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;

int main() {
	Reader reader;
	timeMeasurement tM;
	fstream outputFile;
	long long llAvgTime = 0;
	long long llAvgTimefloat = 0;

	outputFile.open("results.csv", ios::out);

	cout << "Starting...\n";

	reader.OpenIni();
	cout << "\nNazwa pliku: " << reader.sFilename << endl;
	cout << "Ilosc wykonan: " << reader.iRNumber << endl;
	cout << "Optymalny koszt: " << reader.iOCost << endl << endl;

	reader.OpenTxt(reader.sFilename);
	
	outputFile << "Nazwa pliku: " << reader.sFilename << endl;
	outputFile << "Powtórzenia algorytmu: " << reader.iRNumber << endl;
	outputFile << "Optymalny koszt: " << reader.iOCost << endl;
	outputFile << "Otrzymany koszt: " << endl;

	cout << "\n \nChecking input file...\n";

	cout << "\nLoading Graph...\n";
	Graph* myGraph = new Graph(reader.iNOfVertices, reader.iVertices);

	cout << "\n \nGraph output: \n";
	myGraph->display();

	cout << "Calculating...\n";	

	long long llMinTime = INT_MAX;
	long long llMaxTime = 0;
	for (int n = 0; n < reader.iRNumber; ++n) {
		long long llTempTime = 0;
		

		vector<int> path;
		int cost;
		double exeTime;
		float errorRate;

		// parametry SA
		vector<double> initTemp = { 2500.0 };
		vector<double> minTemp = { 1.0008 };

		path.resize(myGraph->getSize() + 1);

		SimulatedAnnealing* test = new SimulatedAnnealing();

		cout << "Doing " << n + 1 << " time... \n";
		test->settingsSimulatedAnnealing(initTemp[0], minTemp[0]);
		exeTime = test->algorithmSimulatedAnnealing(myGraph->getMatrix(), path, cost);

		cout << "Time " << exeTime << " of sth\n";
		cout << "Found solution " << test->getfoundOptimum() << endl;
		cout << "Optimal solution " << reader.iOCost << endl;
		errorRate = (static_cast<float>(test->getfoundOptimum()) / static_cast<float>(reader.iOCost)) * 100;
		cout << "Error rate " << abs(errorRate - 100) << "%\n";

		llTempTime = exeTime;
		if (llTempTime < llMinTime) {
			llMinTime = llTempTime;
		}
		else if (llTempTime > llMaxTime) {
			llMaxTime = llTempTime;
		}
		outputFile << llTempTime << endl;
		llAvgTime = llAvgTime + llTempTime;

		cout << endl;
		cout << endl;


		initTemp.clear();
		minTemp.clear();
	}
	llAvgTimefloat = llAvgTime / reader.iRNumber;
	outputFile << "Œredni czas wykonywania algorytmu [ns]: " << llAvgTimefloat << endl;
	outputFile << "Optymalna œcie¿ka: ";
	//Graph* myGraph = new Graph(reader.iNOfVertices);
	//myGraph->loadGraphFromFile(reader.iNOfVertices, reader.iVertices);
	/*BXB* test = new BXB();
	test->startAlgorithm(myGraph);
	for (int i = 0; i < reader.iNOfVertices + 1; i++) {
		outputFile << test->resultPermut[i] << "-->";
	}
	outputFile << test->startVert << std::endl;
	*/ /*
	cout << "Maksymalny czas wykonywania algorytmu: " << llMaxTime << endl;
	cout << "Minimalny czas wykonywania algorytmu: " << llMinTime << endl;
	cout << "Sredni czas wykonywania algorytmu: " << llAvgTimefloat << endl; */

	

	cout << "Done...\n";

	getchar();
	return 0;
}