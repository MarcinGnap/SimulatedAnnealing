#pragma once
#include <chrono>
#include <iostream>
#include <windows.h>

using namespace std;
using namespace chrono;


class Timer {

private:

	// licznik
	long long counter;

	// cz�stotliwo��
	double frequency;


public:

	// konstruktor
	Timer();

	// destruktor
	~Timer();

	// pocz�tek pomiaru
	void start();

	// koniec pomiaru
	double stop();
};

