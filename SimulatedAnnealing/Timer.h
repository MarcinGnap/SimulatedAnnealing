#pragma once
#include <chrono>
#include <iostream>
#include <windows.h>

using namespace std;
using namespace chrono;


class Timer {
private:
	long long counter;
	double frequency;
public:
	Timer();
	~Timer();
	void start();
	double stop();
};