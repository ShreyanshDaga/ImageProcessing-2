//////////////////////////////////////////////////
//EE 569 Homework #2
//
//Date:			October 13, 2013
//Name:		Shreyansh Dnyanesh Daga
//ID:				6375-3348-33	
//Email:		sdaga@usc.edu
//
//Compiled and build on Visual Studio 2012
//OS: Windows 8
//File: Histogram.h
//////////////////////////////////////////////////

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include<string.h>
#include<conio.h>
#include<stdlib.h>
#include<iostream>

#include"Pixel.h"

using namespace std;

class Histogram
{
	private:
		string szHistName;			

	public:
		int iHistogramR[256];
		int iHistogramG[256];
		int iHistogramB[256];	
		int iHistogramBW[256];

		int iCumHistogramR[256];
		int iCumHistogramG[256];
		int iCumHistogramB[256];	
		int iCumHistogramBW[256];		
		
		int iRows;
		int iCols;
		int iColorFormat;

	public:
		Pixel **pxaPixArr;
		unsigned char **pcHistImage;

		Histogram();
		~Histogram();
		void InitHist(string szImName, int iRows, int iCols,int iColorFormat);
		void AllocHistData();
		void PlotHistogram();
		void PlotCumHistogram();
		void WriteHistogramFile(int iHistCount[256], char cColor, char cType);
		void GetPixelCount();
		void FreeHistogram();
};

#endif