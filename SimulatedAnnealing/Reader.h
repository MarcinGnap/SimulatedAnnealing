#pragma once

#include <string>

class Reader
{
public:
	double tzero;
	double tmin;
	double era;
	double dA;
	int iRNumber[16];
	int iOCost[16];
	std::string fileNames[16];
	std::string sFilename;

	int iNOfVertices;
	int **iVertices;
	int *bestPath;

	Reader();
	~Reader();
	bool OpenIni();
	/*
	int OpenTxt(std::string & sFilename);
	int OpenTsp(std::string & sFilename);
	int OpenAtsp(std::string & sFilename);
	*/
	int OpenAll(std::string & sFilename);
};