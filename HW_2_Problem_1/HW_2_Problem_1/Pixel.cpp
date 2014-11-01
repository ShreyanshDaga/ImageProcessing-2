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

#include"Pixel.h"
#include<math.h>

Pixel::Pixel()
{
	this->iB = -1;
	this->iG = -1;
	this->iR = -1;
	this->iBW = -1;

	this->iX = -1;
	this->iY = -1;
}

Pixel::Pixel(int R, int G, int B)
{
	this->iR = R;
	this->iG = G;
	this->iB = B;

	this->iBW = (0.299*R + 0.587*G + B*0.114);

	this->iX = -1;
	this->iY = -1;
}

Pixel::Pixel(int R, int G , int B , int X, int Y)
{
	this->iR = R;
	this->iG = G;
	this->iB = B;	

	this->iBW =  ((0.299*iR + 0.587*iG + 0.114*iB) + 0.5);

	this->iX = X;
	this->iY = Y;
}

Pixel::Pixel(int BW)
{
	this->iBW = BW;
}

Pixel::Pixel(int BW,int X, int Y, bool bDum)
{
	this->iBW = BW;

	this->iX = X;
	this->iY = Y;
}

Pixel::~Pixel()
{
}

int Pixel::GetRed()
{
	return this->iR;
}

int Pixel::GetGreen()
{
	return this->iG;
}

int Pixel::GetBlue()
{
	return this->iB;
}

int Pixel::GetBW()
{
	return this->iBW;
}

int Pixel::GetPixelX()
{
	return this->iX;
}

int Pixel::GetPixelY()
{
	return this->iY;
}

float Pixel::DistanceTo(Pixel pxP)
{
	float fDist;
	int iX1,iX2,iY1,iY2;

	iX1 = this->GetPixelX();
	iX2 = pxP.GetPixelX();
	iY1 = this->GetPixelY();
	iY2 = pxP.GetPixelY();

	fDist = sqrtf(pow((float)(iX2 - iX1),2) + pow((float)(iY2 - iY1),2));

	return fDist;
}

float Pixel::DistanceFrom(Pixel pxP)
{
	float fDist;
	int iX1,iX2,iY1,iY2;

	iX1 = this->GetPixelX();
	iX2 = pxP.GetPixelX();
	iY1 = this->GetPixelY();
	iY2 = pxP.GetPixelY();

	fDist = sqrtf(pow((float)(iX1 - iX2),2) + pow((float)(iY1 - iY2),2));

	return fDist;
}

Pixel Pixel::operator- (Pixel & pxArg)
{
	Pixel pxTemp(this->GetRed() - pxArg.GetRed(),this->GetGreen() - pxArg.GetGreen(),this->GetBlue() - pxArg.GetBlue());

	return pxTemp;
}

bool Pixel::operator==(Pixel &pxArg)
{
	if(this->GetRed() == pxArg.GetRed() && this->GetRed() == pxArg.GetRed() && this->GetRed() == pxArg.GetRed())
		return true;
	else
		return false;
}

Pixel Pixel::operator + (Pixel &pxArg)
{
	Pixel pxTemp(this->GetRed() + pxArg.GetRed(),this->GetGreen() + pxArg.GetGreen(),this->GetBlue() + pxArg.GetBlue());

	return pxTemp;
}

void Pixel::operator = (Pixel &pxArg)
{
	this->iR = pxArg.GetRed();
	this->iG = pxArg.GetGreen();
	this->iB = pxArg.GetBlue();

	this->iBW = pxArg.GetBW();
	this->iX = pxArg.GetPixelX();
	this->iY = pxArg.GetPixelY();
}