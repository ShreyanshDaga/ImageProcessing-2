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

#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>

#include"HW_2_Q_1.h"

using namespace std;

int PrintMenu();
void GenerateTestImage();

void main()
{
	int iCh = 0;

	while(1)
	{
		iCh = PrintMenu();		
		//GenerateTestImage();
		switch(iCh)
		{				
			case 1:
				Problem_1_A();				
			break;
		
			case 2:
			{
				Problem_1_B_a();
				Problem_1_B_b();				
			}
			break;
		
			case 3:
				Problem_1_C();
			break;			
		
			case 4:
			{
				Problem_1_D_a();
				Problem_1_D_b();
			}
			break;			

			case 5:
				Problem_1_E();
			break;

			case 6:
				Problem_2_A();
			break;

			case 7:
				Problem_2_B();
			break;

			case 8:
				Problem_2_C();
			break;

			case 9:
				Problem_2_D();
			break;

			case 10:
				Problem_3_A();
			break;

			case 11:
				Problem_3_B();
			break;

			case 12:
				Problem_3_C();
			break;

			case 13:
				exit(0);
			break;			
		}
	}
}

int PrintMenu()
{
	int iCh = 0;

	while(1)
	{
		system("CLS");		
		cout<<"\t\t\t\t\t\tShreyansh Daga, USCID 6375334833\n";
		cout<<"******* Home work 2 ********\n";		
		cout<<" Problem 1:Special Effect Image Filter\n";
		cout<<"1.\tQ1(a) Color to Gray Scale Conversion\n";
		cout<<"2.\tQ1(b) Pencil Sketch using Edge Detectors\n";
		cout<<"3.\tQ1(c) Background Special Effect\n";
		cout<<"4.\tQ1(d) Transition Special Effect\n";
		cout<<"5.\tQ1(e) Color Pencil Sketch\n\n";
		cout<<"6.\tQ2(a) Shrinking\n";
		cout<<"7.\tQ2(b) Thinning\n";
		cout<<"8.\tQ2(c) Skeletonizing\n";
		cout<<"9.\tQ2(d) PAC-MAN Game\n\n";
		cout<<"10.\tQ3(a) Dithering\n";
		cout<<"11.\tQ3(b) Error Diffusion\n";
		cout<<"12.\tQ3(c) Inverse Halftoning\n";
		cout<<"13.\tExit\n";
		cout<<"\n Enter your choice: ";
		cin>>iCh;

		if(iCh > 13)
		{
			cout<<"\n\n!! Please enter valid choice !!\n\n";
			getch();
			iCh = 0;
		}
		else
		{
			break;
		}
	}
	return iCh;
}

void GenerateTestImage()
{
	Image imRect, imSquare, imCircle, imRing, imTriangle;
	string strNames[5] = {"Rect.raw","Square.raw","Circle.raw","Ring.raw","Triangle.raw"};
	int iRows = 300, iCols = 300;
	int iaImData[300][300] = {255};

	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			iaImData[i][j] = 255;
		}
	}

	imRect.SetImageName(strNames[0]);
	imRect.SetImageRows(iRows);
	imRect.SetImageCols(iCols);
	imRect.SetImageColorFormat(1);
	imRect.AllocImageMem();

	imSquare.SetImageName(strNames[1]);
	imSquare.SetImageRows(iRows);
	imSquare.SetImageCols(iCols);
	imSquare.SetImageColorFormat(1);
	imSquare.AllocImageMem();

	imCircle.SetImageName(strNames[2]);
	imCircle.SetImageRows(iRows);
	imCircle.SetImageCols(iCols);
	imCircle.SetImageColorFormat(1);
	imCircle.AllocImageMem();

	imRing.SetImageName(strNames[3]);
	imRing.SetImageRows(iRows);
	imRing.SetImageCols(iCols);
	imRing.SetImageColorFormat(1);
	imRing.AllocImageMem();

	imTriangle.SetImageName(strNames[4]);
	imTriangle.SetImageRows(iRows);
	imTriangle.SetImageCols(iCols);
	imTriangle.SetImageColorFormat(1);
	imTriangle.AllocImageMem();

	//Rectangle
	for(int i = 100;i<201;i++)
	{
		for(int j = 50;j<251;j++)
		{
			iaImData[i][j] = 0;
		}
	}

	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			imRect.pImageDate[i][j] = iaImData[i][j];
		}
	}
	imRect.WriteImageData();

	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			iaImData[i][j] = 255;
		}
	}

	//Square
	for(int i = 100;i<201;i++)
	{
		for(int j = 100;j<201;j++)
		{
			iaImData[i][j] = 0;
		}
	}

	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			imSquare.pImageDate[i][j] = iaImData[i][j];
		}
	}
	imSquare.WriteImageData();

	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			iaImData[i][j] = 255;
		}
	}

	//Ring
	for(int i = 100;i<201;i++)
	{
		for(int j = 50;j<251;j++)
		{
			iaImData[i][j] = 0;
		}
	}

	for(int i = 130;i<171;i++)
	{
		for(int j = 70;j<231;j++)
		{
			iaImData[i][j] = 255;
		}
	}

	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			imRing.pImageDate[i][j] = iaImData[i][j];
		}
	}
	imRing.WriteImageData();

	getch();
}