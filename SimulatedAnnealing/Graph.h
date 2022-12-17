#pragma once
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <random>
#include <time.h>
#include <fstream>
#include <sstream>
#include <cmath>
#include <queue>
#include <numeric>
#include <windows.h>

using namespace std;


class Graph {

private:

	// macierz
	vector<vector<int>> matrix;

	// wielko�� macierzy
	int size;

public:

	// pusty konstruktor
	Graph();

	// destruktor
	~Graph();

	// konstruktor kt�ry generuje losowy graf
	Graph(int size, int** newMatrix);

	// konstruktor kt�ry przypisuje wska�nik na macierz
	Graph(Graph* graph);

	// konstruktor kt�ry wczytuje dane z pliku
	Graph(string filePath);

	// wypisanie grafu w postaci macierzy s�siedztwa
	void display();

	// wypisanie �cie�ki z podanej tablicy w�z��w
	void displayPath(vector<int> path);

	// ustawienie kom�rki
	void setCell(int x, int y, int value);

	// pobranie kom�rki
	int getCell(int x, int y);

	// pobranie wielko�ci macierzy
	int getSize();

	// skopiowanie macierzy
	void copyMatrix(Graph* old);

	// pobranie macierzy
	vector<vector<int>> getMatrix();
};