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

	// wielkoœæ macierzy
	int size;

public:

	// pusty konstruktor
	Graph();

	// destruktor
	~Graph();

	// konstruktor który generuje losowy graf
	Graph(int size, int** newMatrix);

	// konstruktor który przypisuje wskaŸnik na macierz
	Graph(Graph* graph);

	// konstruktor który wczytuje dane z pliku
	Graph(string filePath);

	// wypisanie grafu w postaci macierzy s¹siedztwa
	void display();

	// wypisanie œcie¿ki z podanej tablicy wêz³ów
	void displayPath(vector<int> path);

	// ustawienie komórki
	void setCell(int x, int y, int value);

	// pobranie komórki
	int getCell(int x, int y);

	// pobranie wielkoœci macierzy
	int getSize();

	// skopiowanie macierzy
	void copyMatrix(Graph* old);

	// pobranie macierzy
	vector<vector<int>> getMatrix();
};