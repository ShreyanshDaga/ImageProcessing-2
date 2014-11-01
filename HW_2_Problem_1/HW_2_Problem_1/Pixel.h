//////////////////////////////////////////////////
//EE 569 Homework #2
//
//Date:			September 22, 2013
//Name:		Shreyansh Dnyanesh Daga
//ID:				6375-3348-33	
//Email:		sdaga@usc.edu
//
//Compiled and build on Visual Studio 2012
//OS: Windows 8
//File: HW_2_Q_1.cpp
//////////////////////////////////////////////////

#ifndef PIXEL_H
#define PIXEL_H

class Pixel
{
	public:
		int iR;
		int iG;
		int iB;
		int iBW;			//GreyLevel
		int iX;
		int iY;

	public:
		Pixel();
		Pixel(int R, int G, int B);
		Pixel(int R, int G, int B, int X, int Y);
		Pixel(int iBW);
		Pixel(int iBW, int X, int Y, bool bDum);
		~Pixel();
		int GetRed();
		int GetBlue();
		int GetGreen();
		int GetBW();
		int GetPixelX();
		int GetPixelY();
		float DistanceTo(Pixel pxP);
		float DistanceFrom(Pixel pxP);	

		Pixel operator - (Pixel &pxArg);
		Pixel operator + (Pixel &pxArg);
		void operator = (Pixel &pxArg);
		bool operator == (Pixel &pxArg);
};

#endif