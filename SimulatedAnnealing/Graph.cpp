#include "Graph.h"



Graph::Graph() {

}



Graph::~Graph() {

}



Graph::Graph(int size, int** newMatrix) {
	// rezerwujemy pamiêæ
	matrix.reserve(size);

	this->size = size;

	for (int i = 0; i < size; i++) {
		vector<int> temp;
		temp.reserve(size);

		for (int j = 0; j < size; j++) {
			temp.push_back(newMatrix[i][j]);
		}

		temp[i] = 0;
		this->matrix.push_back(temp);
	}

	// uzupe³nianie przek¹tnej zerami
	for (int i = 0; i < size; i++) {

		matrix[i][i] = 0;
	}
}



Graph::Graph(Graph* graph) {

	// przypisujemy wskaŸnik na aktualny graf
	this->matrix = graph->matrix;
}



Graph::Graph(string filePath) {

	ifstream file;
	file.open(filePath);

	int item;

	// sprawdzamy czy uda³o siê otworzyæ plik
	if (file.is_open()) {

		// wczytujemy liczbê miast
		file >> size;

		// sprawdzamy dan¹
		if (size < 0) {

			cout << "Rozmiar jest liczba ujemna";
			exit(EXIT_FAILURE);
		}

		// rezerwujemy miejsce
		matrix.reserve(size);

		// czytamy tylko tyle wierszy jaka jest wielkoœæ macierzy
		for (int i = 0; i < size; ++i) {

			// rezerwowanie miejsca
			vector<int> row;
			row.reserve(size);

			// czytamy tylko tyle kolumn jaka jest wielkoœæ macierzy
			for (int j = 0; j < size; ++j) {

				// pobieramy dan¹ ze wczeœniej wczytanej linii
				file >> item;

				// sprawdzamy dan¹
				if (item < 0) {

					cout << "Napotkano liczbe ujemna";
					exit(EXIT_FAILURE);
				}

				// je¿eli jest to przek¹tna
				if (i == j) {

					item = 0;
				}

				// umieszczamy dan¹
				row.push_back(item);
			}

			// umieszczamy wiersz w macierzy
			matrix.push_back(row);
		}

		file.close();
	}
	else {

		cout << "Nie mozna otworzyc pliku!";
		exit(EXIT_FAILURE);
	}
}



void Graph::display() {

	cout << setw(5) << size << endl << endl;
	for (size_t i = 0; i < matrix.size(); ++i) {

		for (size_t j = 0; j < matrix.size(); ++j) {

			cout << setw(5) << matrix[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}



void Graph::displayPath(vector<int> path) {

	for (size_t i = 0; i < path.size(); ++i) {

		if (i == path.size() - 1)
			cout << path[i];
		else
			cout << path[i] << " -> ";
	}
}



void Graph::setCell(int x, int y, int value) {

	matrix[x][y] = value;
}



int Graph::getCell(int x, int y) {

	return matrix[x][y];
}



int Graph::getSize() {

	return matrix.size();
}



void Graph::copyMatrix(Graph* old) {

	this->matrix = old->matrix;
}



vector<vector<int>> Graph::getMatrix() {

	return matrix;
}