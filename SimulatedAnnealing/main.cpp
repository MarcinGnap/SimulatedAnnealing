#include "Reader.h"
#include "Graph.h"
#include "SimulatedAnnealing.h"
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;

int main() {
	Reader reader;
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
		//myGraph->display();

		cout << "Calculating...\n";

		double llMinTime = INT_MAX;
		double llMaxTime = 0;
		double llMinErrorRate = INT_MAX;
		double llMaxErrorRate = 0;

		double llTempTime = 0;

		vector<int> path;
		int cost;
		double exeTime;
		float errorRate;

		vector<double> initTemp = { reader.tzero };
		vector<double> minTemp = { reader.tmin };
		vector<double> initEra = { reader.era };
		vector<double> wspA = {reader.dA};

		path.resize(myGraph->getSize() + 1);

		outputFile << "Otrzymany koszt: ;";
		outputFile << "Wspó³czynnik b³êdu[%]: ;";
		outputFile << "Czas wykonywania[s] ;";
		for (int min = 0; min < wspA.size(); min++) {
			cout << "\nDoing for a = " << wspA[min] << " ... \n\n";
			outputFile << "Rozwi¹zania dla " << wspA[min] << " wspó³czynnika a" << endl;
			for (int init = 0; init < initTemp.size(); init++) {
				cout << "\nDoing for " << initTemp[init] << " ini temperature... \n\n";
				outputFile << "Rozwi¹zania dla " << initTemp[init] << " inicjalizacyjnej temperatury" << endl;
				for (int z = 0; z < initEra.size(); z++) {
					cout << "\nDoing for " << initEra[z] << " eras... \n\n";
					outputFile << "Rozwi¹zania dla " << initEra[z] << " d³ugoœci epoki" << endl;
					double avrageErrorRate = 0;
					for (int y = 0; y < reader.iRNumber[pliki]; y++) {
						SimulatedAnnealing* test = new SimulatedAnnealing();

						test->settingsSimulatedAnnealing(initTemp[init], minTemp[min]);
						exeTime = test->algorithmSimulatedAnnealing(myGraph->getMatrix(), path, cost, initEra[z], reader.dA);

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
						if (abs(errorRate - 100) < llMinErrorRate) {
							llMinErrorRate = abs(errorRate - 100);
						}
						else if (abs(errorRate - 100) > llMaxErrorRate) {
							llMaxErrorRate = abs(errorRate - 100);
						}
						llAvgTime = llAvgTime + llTempTime;
					}
					cout << "Optimal solution " << reader.iOCost[pliki] << endl;

					llAvgTimefloat = llAvgTime / reader.iRNumber[pliki];
					outputFile << "Œredni czas wykonywania algorytmu [s]: " << " ; " << llAvgTimefloat << endl;
					outputFile << "Œredni wspó³czynnik b³êdu [%]: " << " ; " << avrageErrorRate/reader.iRNumber[pliki] << endl;
					outputFile << "Optymalny koszt: " << " ; " << reader.iOCost[pliki] << endl << endl;
					llAvgTimefloat = 0;
					llAvgTime = 0;
					llTempTime = 0;

					cout << "Maksymalny wspolczynnik bledu: " << llMaxErrorRate << endl;
					cout << "Minimalny wspolczynnik bledu: " << llMinErrorRate << endl;
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