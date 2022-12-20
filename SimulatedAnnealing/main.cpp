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

	outputFile.open("results.csv", ios::out);
	cout << "Starting...\n";

	for (int pliki = 0; pliki < sizeof(reader.iRNumber) / sizeof(int); pliki++) {

		double llAvgTime = 0;
		double llAvgTimefloat = 0;

		reader.OpenIni();
		cout << "\nNazwa pliku: " << reader.fileNames[pliki] << endl;
		cout << "Ilosc wykonan: " << reader.iRNumber[pliki] << endl;
		cout << "Optymalny koszt: " << reader.iOCost[pliki] << endl << endl;

		reader.OpenAll(reader.fileNames[pliki]);

		outputFile << "Nazwa pliku: " << reader.sFilename << endl;
		outputFile << "Powtórzenia algorytmu: " << reader.iRNumber[pliki] << endl;
		outputFile << "Optymalny koszt: " << reader.iOCost[pliki] << endl;

		cout << "\n \nChecking input file...\n";

		cout << "\nLoading Graph...\n";
		Graph* myGraph = new Graph(reader.iNOfVertices, reader.iVertices);

		cout << "\n \nGraph output: \n";
		myGraph->display();

		cout << "Calculating...\n";

		double llMinTime = INT_MAX;
		double llMaxTime = 0;

		double llTempTime = 0;

		vector<int> path;
		int cost;
		double exeTime;
		float errorRate;

		// parametry SA
		vector<double> initTemp = { 10.0, 100.0, 1000.0 , 5000.0, 10000.0 };
		vector<double> minTemp = { 0.99, 0.999, 0.9999, 0.99999, 0.999999 };
		vector<double> initEra = { 5, 10, 100, 1000 };

		path.resize(myGraph->getSize() + 1);
		// Tutaj petla ze zmianami + wrzucic wypluwanie wynikow w petle i wypluwanie do outputFile w petle.

		outputFile << "Otrzymany koszt: ;";
		outputFile << "Wspó³czynnik b³êdu[%]: ;";
		outputFile << "Czas wykonywania[s] ;";
		for (int min = 0; min < minTemp.size(); min++) {
			cout << "\nDoing for " << minTemp[min] << " min temperature... \n\n";
			outputFile << "Rozwi¹zania dla " << minTemp[min] << " minimalnej temperatury" << endl;
			for (int init = 0; init < initTemp.size(); init++) {
				cout << "\nDoing for " << initTemp[init] << " ini temperature... \n\n";
				outputFile << "Rozwi¹zania dla " << initTemp[init] << " inicjalizacyjnej temperatury" << endl;
				for (int z = 0; z < initEra.size(); z++) {
					cout << "\nDoing for " << initEra[z] << " eras... \n\n";
					outputFile << "Rozwi¹zania dla " << initEra[z] << " iloœci epok" << endl;
					double avrageErrorRate = 0;
					for (int y = 0; y < reader.iRNumber[pliki]; y++) {
						SimulatedAnnealing* test = new SimulatedAnnealing();

						test->settingsSimulatedAnnealing(initTemp[init], minTemp[min]);
						exeTime = test->algorithmSimulatedAnnealing(myGraph->getMatrix(), path, cost, initEra[z]);

						errorRate = (static_cast<float>(test->getfoundOptimum()) / static_cast<float>(reader.iOCost[pliki])) * 100;
						cout << "Error rate " << abs(errorRate - 100) << "%\n";
						cout << "Time " << exeTime << " s\n";
						cout << "Found solution " << test->getfoundOptimum() << endl << endl;
						outputFile << test->getfoundOptimum() << " ; ";
						outputFile << abs(errorRate - 100) << " ; ";
						outputFile << exeTime << " ; " << endl;

						avrageErrorRate = avrageErrorRate + abs(errorRate - 100);

						llTempTime = exeTime;
						if (llTempTime < llMinTime) {
							llMinTime = llTempTime;
						}
						else if (llTempTime > llMaxTime) {
							llMaxTime = llTempTime;
						}
						llAvgTime = llAvgTime + llTempTime;
					}
					cout << "Optimal solution " << reader.iOCost[pliki] << endl;

					llAvgTimefloat = llAvgTime / reader.iRNumber[pliki];
					outputFile << "Œredni czas wykonywania algorytmu [s]: " << llAvgTimefloat << endl;
					outputFile << "Œredni wspó³czynnik b³êdu [%]: " << avrageErrorRate/reader.iRNumber[pliki] << endl << endl;
					outputFile << "Optymalny koszt: " << reader.iOCost[pliki];

					cout << "Sredni wspolczynnik bledu: " << avrageErrorRate / reader.iRNumber[pliki] << endl;
					cout << "Maksymalny czas wykonywania algorytmu: " << llMaxTime << endl;
					cout << "Minimalny czas wykonywania algorytmu: " << llMinTime << endl;
					cout << "Sredni czas wykonywania algorytmu: " << llAvgTimefloat << endl;

					cout << endl;
					cout << endl;
				}
			}
		}
		initTemp.clear();
		minTemp.clear();


		cout << "Done...\n";
	}

	cout << "All done... \nGood luck... \n";
	getchar();
	return 0;
}