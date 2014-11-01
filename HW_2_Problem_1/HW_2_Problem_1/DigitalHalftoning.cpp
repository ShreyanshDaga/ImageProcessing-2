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
#include<iostream>
#include<string.h>

#include"HW_2_Q_1.h"
#include"Pixel.h"
#include"Image.h"
#include"Histogram.h"

using namespace std;

void Problem_3_A()
{
	Image imMan, imOutput;
	int iRows = 1024, iCols = 1024;
	string strMan("man.raw"), strOutput("man_out_fixed.raw"), strOutput1("man_out_random.raw"), strOutput2("man_out_dithering_2x2.raw"), strOutput4("man_out_dithering_4x4.raw");
	int iThresh;

	imMan.SetImageName(strMan);
	imMan.SetImageRows(iRows);
	imMan.SetImageCols(iCols);
	imMan.SetImageColorFormat(1);
	imMan.AllocImageMem();

	imOutput.SetImageName(strOutput);
	imOutput.SetImageRows(iRows);
	imOutput.SetImageCols(iCols);
	imOutput.SetImageColorFormat(1);
	imOutput.AllocImageMem();

	imMan.ReadImageData();

	imMan.PlotHistogram();

	cout<<"\nEnter Threshold: ";
	cin>>iThresh;

	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel *pxP = imMan.GetPixel(i,j);
			Pixel pxPB(0), pxPW(255);

			if(pxP->GetBW() >= iThresh)
			{
				imOutput.SetPixel(i,j,pxPW);
			}
			else
			{
				imOutput.SetPixel(i,j,pxPB);
			}

			delete pxP;
		}
	}

	imOutput.WriteImageData();

	cout<<"\nRandom THresholding:";
	srand(127);
	imOutput.SetImageName(strOutput1);
	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel *pxP = imMan.GetPixel(i,j);
			Pixel pxPB(0), pxPW(255);

			iThresh = abs(rand())%255;

			if(pxP->GetBW() >= iThresh)
			{
				imOutput.SetPixel(i,j,pxPW);
			}
			else
			{
				imOutput.SetPixel(i,j,pxPB);
			}

			delete pxP;
		}
	}

	imOutput.WriteImageData();
	
	//Dithering Matrix
	imOutput.SetImageName(strOutput2);

	int iaDitMat2[2][2], iaDitMat4[4][4];
	float iaT2[2][2],iaT4[4][4];

	//Create DitMat 2x2
	iaDitMat2[0][0] = 1;
	iaDitMat2[0][1] = 2;
	iaDitMat2[1][0] = 3;
	iaDitMat2[1][1] = 0;

	//Create 4x4 DitMat
	for(int i = 0;i<2;i++)
	{
		for(int j = 0;j<2;j++)
		{
			for(int l = 0;l<2;l++)
			{
				for(int m = 0;m<2;m++)
				{
					iaDitMat4[i][j] = 4*iaDitMat2[l][m] + 1;
				}
			}
		}
	}

	for(int i = 2;i<4;i++)
	{
		for(int j = 0;j<2;j++)
		{
			for(int l = 0;l<2;l++)
			{
				for(int m = 0;m<2;m++)
				{
					iaDitMat4[i][j] = 4*iaDitMat2[l][m] + 3;
				}
			}
		}
	}

	for(int i = 0;i<2;i++)
	{
		for(int j = 2;j<4;j++)
		{
			for(int l = 0;l<2;l++)
			{
				for(int m = 0;m<2;m++)
				{
					iaDitMat4[i][j] = 4*iaDitMat2[l][m] + 2;
				}
			}
		}
	}

	for(int i = 2;i<4;i++)
	{
		for(int j = 2;j<4;j++)
		{
			for(int l = 0;l<2;l++)
			{
				for(int m = 0;m<2;m++)
				{
					iaDitMat4[i][j] = 4*iaDitMat2[l][m];
				}
			}
		}
	}
	
	for(int i = 0;i<2;i++)
	{
		for(int j = 0;j<2;j++)
		{
			iaT2[i][j] = (iaDitMat2[i][j] + 0.5)/4;
		}
	}

	for(int i = 0;i<4;i++)
	{
		for(int j = 0;j<4;j++)
		{
			iaT4[i][j] = (iaDitMat4[i][j] + 0.5)/4;
		}
	}

	//Do THresholding
	cout<<"\n*Using 2x2 Matrix";
	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel *pxP = imMan.GetPixel(i,j);
			Pixel pxPB(0),pxPW(255);

			if(pxP->GetBW() > iaT2[i%2][j%2]*255)
			{
				imOutput.SetPixel(i,j,pxPW);
			}
			else
			{
				imOutput.SetPixel(i,j,pxPB);
			}
		}
	}
	imOutput.WriteImageData();

	cout<<"\n* Using 4x4 Matrix";
	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel *pxP = imMan.GetPixel(i,j);
			Pixel pxPB(0),pxPW(255);

			if(pxP->GetBW() > iaT4[i%4][j%4]*255)
			{
				imOutput.SetPixel(i,j,pxPW);
			}
			else
			{
				imOutput.SetPixel(i,j,pxPB);
			}
		}
	}
	imOutput.SetImageName(strOutput4);
	imOutput.WriteImageData();
	cout<<"\nPress 'C' to continue!";
	getch();
}

void Problem_3_B()
{
	Image imGrad,imGradOut;
	int iRows = 256, iCols = 256;
	
	double **iaGMat = new double*[iRows];
	for(int i = 0;i<iRows;i++)
		iaGMat[i] = new double[iCols];

	double **iaFMat = new double*[iRows];
	for(int i = 0;i<iRows;i++)
		iaFMat[i] = new double[iCols];

	imGrad.SetImageName(string::basic_string("gradient.raw"));
	imGrad.SetImageRows(iRows);
	imGrad.SetImageCols(iCols);
	imGrad.SetImageColorFormat(1);
	imGrad.AllocImageMem();

	imGradOut.SetImageName(string::basic_string("grad_FS.raw"));
	imGradOut.SetImageRows(iRows);
	imGradOut.SetImageCols(iCols);
	imGradOut.SetImageColorFormat(1);
	imGradOut.AllocImageMem();

	imGrad.ReadImageData();
	imGrad.PlotHistogram();

	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel *pxP = imGrad.GetPixel(i,j);					
			iaFMat[i][j] = (double) pxP->GetBW();
			delete pxP;
		}
	}


	//Floyed Steinberg Error Diffusion Method
	double iThresh = 127.00;
	double dDiff;

	//Serpentine SCannign
	for(int i = 0;i<iRows;i++)
	{
		if(i%2 == 0)
		{
			for(int j = 0;j<iCols;j++)
			{
				int iTempBW = iaFMat[i][j];
				
				if( iTempBW > iThresh)
				{
					iaFMat[i][j] = 255;
				}
				else
				{
					iaFMat[i][j] = 0;
				}

				dDiff = iTempBW - iaFMat[i][j];

				if(i == (iRows - 1))
				{
					if(j != (iCols - 1))
						iaFMat[i][j + 1] += (dDiff/(double)4);
				}
				else
				{
					if(j == 0)
					{
						iaFMat[i][j + 1] += ((double)8 / (double)16 * dDiff);
						iaFMat[i + 1][j] += ((double)6 / (double)16 * dDiff);
						iaFMat[i + 1][j + 1] += ((double)2 / (double)16 * dDiff);
					}
					else if(j == (iCols - 1))
					{
						iaFMat[i + 1][j - 1] += ((double)12 / (double)16 * dDiff);
						iaFMat[i + 1][j] += ((double)4 / (double)16 * dDiff);
					}
					else
					{
						iaFMat[i][j + 1] += ((double)7 / (double)16 * dDiff);
						iaFMat[i + 1][j - 1] += ((double)3 / (double)16 * dDiff);
						iaFMat[i + 1][j] += ((double)5 / (double)16 * dDiff);
						iaFMat[i + 1][j + 1] += ((double)1 / (double)16 * dDiff);
					}				
				}
			}
		}
		else
		{
			for(int j = iCols - 1;j>=0;j--)
			{
				double iTempBW = iaFMat[i][j];
				
				if( iTempBW > iThresh)
				{
					iaFMat[i][j] = 255;
				}
				else
				{
					iaFMat[i][j] = 0;
				}

				dDiff = iTempBW - iaFMat[i][j];

				if(i == (iRows - 1))
				{
					if(j != 0)
						iaFMat[i][j - 1] += (dDiff/(double)4);
				}
				else
				{
					if(j == (iCols - 1))
					{
						iaFMat[i][j - 1] += ((double)10 / (double)16 * dDiff);
						iaFMat[i + 1][j] += ((double)5 / (double)16 * dDiff);
						iaFMat[i + 1][j - 1] += ((double)1 / (double)16 * dDiff);
					}
					else if(j == 0)
					{
						iaFMat[i + 1][j - 1] += ((double)10 / (double)16 * dDiff);
						iaFMat[i + 1][j] += ((double)6 / (double)16 * dDiff);
					}
					else
					{
						iaFMat[i][j - 1] += ((double)7 / (double)16 * dDiff);
						iaFMat[i + 1][j + 1] += ((double)3 / (double)16 * dDiff);
						iaFMat[i + 1][j] += ((double)5 / (double)16 * dDiff);
						iaFMat[i + 1][j - 1] += ((double)1 / (double)16 * dDiff);
					}				
				}
			}
		}
	}

	for(int i = 0; i < iRows; i++)
	{
		for(int j = 0; j < iCols; j++)
		{
			Pixel pxP;

			pxP.iBW = iaFMat[i][j];

			imGradOut.SetPixel(i,j,pxP);
		}
	}

	imGradOut.WriteImageData();


	//JJN Error Diffusion Method
	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel *pxP = imGrad.GetPixel(i,j);					
			iaFMat[i][j] = (double) pxP->GetBW();
			delete pxP;
		}
	}

	iThresh = 127.00;
	dDiff = 0;

	//Serpentine SCannign
	for(int i = 0;i<iRows;i++)
	{
		if(i%2 == 0)
		{
			//Forward Row Tracing
			for(int j = 0;j<iCols;j++)
			{
				int iTempBW = iaFMat[i][j];
				
				if( iTempBW > iThresh)
				{
					iaFMat[i][j] = 255;
				}
				else
				{
					iaFMat[i][j] = 0;
				}

				dDiff = iTempBW - iaFMat[i][j];

				if(i == (iRows - 1))
				{					
					if(j != (iCols - 1))
					{						
						if(j == (iCols - 2))
						{
							iaFMat[i][j + 1] += (dDiff * 7.00/48.00);				
						}
						else
						{
							iaFMat[i][j+1] += dDiff * 7.00/12.00;
							iaFMat[i][j+2] += dDiff * 5.00/12.00;
						}
					}
				}
				else if(i == (iRows - 2))
				{
					if(j != (iCols - 1))
					{			
						if(j == 0)
						{
							iaFMat[i][j + 1] += (dDiff * 7.00/27.00);
							iaFMat[i][j+2] += (dDiff * 5.00/27.00);
							iaFMat[i+1][j] += (dDiff * 7.00/27.00);
							iaFMat[i+1][j+1] += (dDiff * 5.00/27.00);
							iaFMat[i+1][j+2] += (dDiff * 3.00/27.00);
						}
						else if(j == (iCols - 2))
						{
							iaFMat[i][j + 1] += (dDiff * 7.00/25.00);
							iaFMat[i+1][j-2] += (dDiff * 3.00/25.00);
							iaFMat[i+1][j-1] += (dDiff * 5.00/25.00);
							iaFMat[i+1][j] += (dDiff * 7.00/25.00);
							iaFMat[i+1][j+1] += (dDiff * 3.00/25.00);							
						}
						else
						{
							iaFMat[i][j+1] += dDiff * 7.00/35.00;
							iaFMat[i][j+2] += dDiff * 5.00/35.00;
							iaFMat[i+1][j-2] += (dDiff * 3.00/35.00);
							iaFMat[i+1][j-1] += (dDiff * 5.00/35.00);
							iaFMat[i+1][j] += (dDiff * 7.00/35.00);
							iaFMat[i+1][j+1] += (dDiff * 5.00/35.00);
							iaFMat[i+1][j+2] += (dDiff * 3.00/35.00);
						}
					}
				}
				else
				{
					if(j == 0)
					{
						iaFMat[i][j+1] += (dDiff * 7.00/36.00);
						iaFMat[i][j+2] += (dDiff * 5.00/36.00);
						iaFMat[i+1][j] += (dDiff * 7.00/36.00);
						iaFMat[i+1][j+1] += (dDiff * 5.00/36.00);
						iaFMat[i+1][j+2] += (dDiff * 3.00/36.00);
						iaFMat[i+2][j] += (dDiff * 5.00/36.00);
						iaFMat[i+2][j+1] += (dDiff * 3.00/36.00);
						iaFMat[i+2][j+2] += (dDiff * 1.00/36.00);						
					}
					else if(j == (iCols - 2))
					{
						iaFMat[i][j+1] += (dDiff * 7.00/39.00);
						iaFMat[i+1][j-2] += (dDiff * 3.00/39.00);
						iaFMat[i+1][j-1] += (dDiff * 5.00/39.00);
						iaFMat[i+1][j] += (dDiff * 7.00/39.00);
						iaFMat[i+1][j+1] += (dDiff * 5.00/39.00);
						iaFMat[i+2][j-2] += (dDiff * 1.00/39.00);
						iaFMat[i+2][j-1] += (dDiff * 3.00/39.00);
						iaFMat[i+2][j] += (dDiff * 5.00/39.00);
						iaFMat[i+2][j+1] += (dDiff * 3.00/39.00);
					}
					else if(j == (iCols - 1))
					{												
						iaFMat[i+1][j-2] += (dDiff * 3.00/24.00);
						iaFMat[i+1][j-1] += (dDiff * 5.00/24.00);
						iaFMat[i+1][j] += (dDiff * 7.00/24.00);												
						iaFMat[i+2][j-2] += (dDiff * 1.00/24.00);
						iaFMat[i+2][j-1] += (dDiff * 3.00/24.00);
						iaFMat[i+2][j] += (dDiff * 5.00/24.00);
					}
					else
					{
						iaFMat[i][j+1] += (dDiff * 7.00/48.00);
						iaFMat[i][j+2] += (dDiff * 5.00/48.00);
						iaFMat[i+1][j-2] += (dDiff * 3.00/48.00);
						iaFMat[i+1][j-1] += (dDiff * 5.00/48.00);
						iaFMat[i+1][j] += (dDiff * 7.00/48.00);
						iaFMat[i+1][j+1] += (dDiff * 5.00/48.00);
						iaFMat[i+1][j+2] += (dDiff * 3.00/48.00);
						iaFMat[i+2][j-2] += (dDiff * 1.00/48.00);
						iaFMat[i+2][j-1] += (dDiff * 3.00/48.00);
						iaFMat[i+2][j] += (dDiff * 5.00/48.00);
						iaFMat[i+2][j+1] += (dDiff * 3.00/48.00);
						iaFMat[i+2][j+2] += (dDiff * 1.00/48.00);
					}				
				}
			}
		}
		else
		{
			//Reverse Row Tracing
			for(int j = iCols - 1;j>=0;j--)
			{
				double iTempBW = iaFMat[i][j];
				
				if( iTempBW > iThresh)
				{
					iaFMat[i][j] = 255;
				}
				else
				{
					iaFMat[i][j] = 0;
				}

				dDiff = iTempBW - iaFMat[i][j];
				
				if(i == (iRows - 1))
				{					
					if(j != 0)
					{						
						if(j == 1)
						{
							iaFMat[i][j-1] += (dDiff * 7.00/48.00);				
						}
						else
						{
							iaFMat[i][j-1] += dDiff * 7.00/12.00;
							iaFMat[i][j-2] += dDiff * 5.00/12.00;
						}
					}
				}
				else if(i == (iRows - 2))
				{
					if(j != 0)
					{			
						if(j == (iCols-1))
						{
							iaFMat[i][j-1] += (dDiff * 7.00/27.00);
							iaFMat[i][j-2] += (dDiff * 5.00/27.00);
							iaFMat[i+1][j] += (dDiff * 7.00/27.00);
							iaFMat[i+1][j-1] += (dDiff * 5.00/27.00);
							iaFMat[i+1][j-2] += (dDiff * 3.00/27.00);
						}
						else if(j == 1)
						{
							iaFMat[i][j-1] += (dDiff * 7.00/25.00);
							iaFMat[i+1][j+2] += (dDiff * 3.00/25.00);
							iaFMat[i+1][j+1] += (dDiff * 5.00/25.00);
							iaFMat[i+1][j] += (dDiff * 7.00/25.00);
							iaFMat[i+1][j-1] += (dDiff * 3.00/25.00);							
						}
						else
						{
							iaFMat[i][j-1] += dDiff * 7.00/35.00;
							iaFMat[i][j-2] += dDiff * 5.00/35.00;
							iaFMat[i+1][j+2] += (dDiff * 3.00/35.00);
							iaFMat[i+1][j+1] += (dDiff * 5.00/35.00);
							iaFMat[i+1][j] += (dDiff * 7.00/35.00);
							iaFMat[i+1][j-1] += (dDiff * 5.00/35.00);
							iaFMat[i+1][j-2] += (dDiff * 3.00/35.00);
						}
					}
				}
				else
				{
					if(j == (iCols-1))
					{
						iaFMat[i][j-1] += (dDiff * 7.00/36.00);
						iaFMat[i][j-2] += (dDiff * 5.00/36.00);
						iaFMat[i+1][j] += (dDiff * 7.00/36.00);
						iaFMat[i+1][j-1] += (dDiff * 5.00/36.00);
						iaFMat[i+1][j-2] += (dDiff * 3.00/36.00);
						iaFMat[i+2][j] += (dDiff * 5.00/36.00);
						iaFMat[i+2][j-1] += (dDiff * 3.00/36.00);
						iaFMat[i+2][j-2] += (dDiff * 1.00/36.00);						
					}
					else if(j == 1)
					{
						iaFMat[i][j-1] += (dDiff * 7.00/39.00);
						iaFMat[i+1][j+2] += (dDiff * 3.00/39.00);
						iaFMat[i+1][j+1] += (dDiff * 5.00/39.00);
						iaFMat[i+1][j] += (dDiff * 7.00/39.00);
						iaFMat[i+1][j-1] += (dDiff * 5.00/39.00);
						iaFMat[i+2][j+2] += (dDiff * 1.00/39.00);
						iaFMat[i+2][j+1] += (dDiff * 3.00/39.00);
						iaFMat[i+2][j] += (dDiff * 5.00/39.00);
						iaFMat[i+2][j-1] += (dDiff * 3.00/39.00);
					}
					else if(j == 0)
					{												
						iaFMat[i+1][j+2] += (dDiff * 3.00/24.00);
						iaFMat[i+1][j+1] += (dDiff * 5.00/24.00);
						iaFMat[i+1][j] += (dDiff * 7.00/24.00);												
						iaFMat[i+2][j+2] += (dDiff * 1.00/24.00);
						iaFMat[i+2][j+1] += (dDiff * 3.00/24.00);
						iaFMat[i+2][j] += (dDiff * 5.00/24.00);
					}
					else
					{
						iaFMat[i][j-1] += (dDiff * 7.00/48.00);
						iaFMat[i][j-2] += (dDiff * 5.00/48.00);
						iaFMat[i+1][j+2] += (dDiff * 3.00/48.00);
						iaFMat[i+1][j+1] += (dDiff * 5.00/48.00);
						iaFMat[i+1][j] += (dDiff * 7.00/48.00);
						iaFMat[i+1][j-1] += (dDiff * 5.00/48.00);
						iaFMat[i+1][j-2] += (dDiff * 3.00/48.00);
						iaFMat[i+2][j+2] += (dDiff * 1.00/48.00);
						iaFMat[i+2][j+1] += (dDiff * 3.00/48.00);
						iaFMat[i+2][j] += (dDiff * 5.00/48.00);
						iaFMat[i+2][j-1] += (dDiff * 3.00/48.00);
						iaFMat[i+2][j-2] += (dDiff * 1.00/48.00);
					}				
				}
			}
		}
	}

	//Write THe IMgae file
	for(int i = 0; i < iRows; i++)
	{
		for(int j = 0; j < iCols; j++)
		{
			Pixel pxP;

			pxP.iBW = iaFMat[i][j];

			imGradOut.SetPixel(i,j,pxP);
		}
	}

	imGradOut.SetImageName(string::basic_string("grad_JJN_out.raw"));
	imGradOut.WriteImageData();


	//Stucki Error Diffusion Method
	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel *pxP = imGrad.GetPixel(i,j);					
			iaFMat[i][j] = (double) pxP->GetBW();
			delete pxP;
		}
	}

	iThresh = 127.00;
	dDiff = 0;

	//Serpentine SCannign
	for(int i = 0;i<iRows;i++)
	{
		if(i%2 == 0)
		{
			//Forward Row Tracing
			for(int j = 0;j<iCols;j++)
			{
				int iTempBW = iaFMat[i][j];
				
				if( iTempBW > iThresh)
				{
					iaFMat[i][j] = 255;
				}
				else
				{
					iaFMat[i][j] = 0;
				}

				dDiff = iTempBW - iaFMat[i][j];

				if(i == (iRows - 1))
				{					
					if(j != (iCols - 1))
					{						
						if(j == (iCols - 2))
						{
							iaFMat[i][j + 1] += (dDiff * 8.00/42.00);				
						}
						else
						{
							iaFMat[i][j+1] += dDiff * 8.00/12.00;
							iaFMat[i][j+2] += dDiff * 4.00/12.00;
						}
					}
				}
				else if(i == (iRows - 2))
				{
					if(j != (iCols - 1))
					{			
						if(j == 0)
						{
							iaFMat[i][j + 1] += (dDiff * 8.00/26.00);
							iaFMat[i][j+2] += (dDiff * 4.00/26.00);
							iaFMat[i+1][j] += (dDiff * 8.00/26.00);
							iaFMat[i+1][j+1] += (dDiff * 4.00/26.00);
							iaFMat[i+1][j+2] += (dDiff * 2.00/26.00);
						}
						else if(j == (iCols - 2))
						{
							iaFMat[i][j + 1] += (dDiff * 8.00/24.00);
							iaFMat[i+1][j-2] += (dDiff * 2.00/24.00);
							iaFMat[i+1][j-1] += (dDiff * 4.00/24.00);
							iaFMat[i+1][j] += (dDiff * 8.00/24.00);
							iaFMat[i+1][j+1] += (dDiff * 4.00/24.00);							
						}
						else
						{
							iaFMat[i][j+1] += dDiff * 8.00/32.00;
							iaFMat[i][j+2] += dDiff * 4.00/32.00;
							iaFMat[i+1][j-2] += (dDiff * 2.00/32.00);
							iaFMat[i+1][j-1] += (dDiff * 4.00/32.00);
							iaFMat[i+1][j] += (dDiff * 8.00/32.00);
							iaFMat[i+1][j+1] += (dDiff * 4.00/32.00);
							iaFMat[i+1][j+2] += (dDiff * 2.00/32.00);
						}
					}
				}
				else
				{
					if(j == 0)
					{
						iaFMat[i][j+1] += (dDiff * 8.00/33.00);
						iaFMat[i][j+2] += (dDiff * 4.00/33.00);
						iaFMat[i+1][j] += (dDiff * 8.00/33.00);
						iaFMat[i+1][j+1] += (dDiff * 4.00/33.00);
						iaFMat[i+1][j+2] += (dDiff * 2.00/33.00);
						iaFMat[i+2][j] += (dDiff * 4.00/33.00);
						iaFMat[i+2][j+1] += (dDiff * 2.00/33.00);
						iaFMat[i+2][j+2] += (dDiff * 1.00/33.00);						
					}
					else if(j == (iCols - 2))
					{
						iaFMat[i][j+1] += (dDiff * 8.00/35.00);
						iaFMat[i+1][j-2] += (dDiff * 2.00/35.00);
						iaFMat[i+1][j-1] += (dDiff * 4.00/35.00);
						iaFMat[i+1][j] += (dDiff * 8.00/35.00);
						iaFMat[i+1][j+1] += (dDiff * 4.00/35.00);
						iaFMat[i+2][j-2] += (dDiff * 1.00/35.00);
						iaFMat[i+2][j-1] += (dDiff * 2.00/35.00);
						iaFMat[i+2][j] += (dDiff * 4.00/35.00);
						iaFMat[i+2][j+1] += (dDiff * 2.00/35.00);
					}
					else if(j == (iCols - 1))
					{												
						iaFMat[i+1][j-2] += (dDiff * 2.00/21.00);
						iaFMat[i+1][j-1] += (dDiff * 4.00/21.00);
						iaFMat[i+1][j] += (dDiff * 8.00/21.00);												
						iaFMat[i+2][j-2] += (dDiff * 1.00/21.00);
						iaFMat[i+2][j-1] += (dDiff * 2.00/21.00);
						iaFMat[i+2][j] += (dDiff * 4.00/21.00);
					}
					else
					{
						iaFMat[i][j+1] += (dDiff * 8.00/42.00);
						iaFMat[i][j+2] += (dDiff * 4.00/42.00);
						iaFMat[i+1][j-2] += (dDiff * 2.00/42.00);
						iaFMat[i+1][j-1] += (dDiff * 4.00/42.00);
						iaFMat[i+1][j] += (dDiff * 8.00/42.00);
						iaFMat[i+1][j+1] += (dDiff * 4.00/42.00);
						iaFMat[i+1][j+2] += (dDiff * 2.00/42.00);
						iaFMat[i+2][j-2] += (dDiff * 1.00/42.00);
						iaFMat[i+2][j-1] += (dDiff * 2.00/42.00);
						iaFMat[i+2][j] += (dDiff * 4.00/42.00);
						iaFMat[i+2][j+1] += (dDiff * 2.00/42.00);
						iaFMat[i+2][j+2] += (dDiff * 1.00/42.00);
					}				
				}
			}
		}
		else
		{
			//Reverse Row Tracing
			for(int j = iCols - 1;j>=0;j--)
			{
				double iTempBW = iaFMat[i][j];
				
				if( iTempBW > iThresh)
				{
					iaFMat[i][j] = 255;
				}
				else
				{
					iaFMat[i][j] = 0;
				}

				dDiff = iTempBW - iaFMat[i][j];
				
				if(i == (iRows - 1))
				{					
					if(j != 0)
					{						
						if(j == 1)
						{
							iaFMat[i][j-1] += (dDiff * 8.00/42.00);				
						}
						else
						{
							iaFMat[i][j-1] += dDiff * 8.00/12.00;
							iaFMat[i][j-2] += dDiff * 4.00/12.00;
						}
					}
				}
				else if(i == (iRows - 2))
				{
					if(j != 0)
					{			
						if(j == (iCols-1))
						{
							iaFMat[i][j-1] += (dDiff * 8.00/26.00);
							iaFMat[i][j-2] += (dDiff * 4.00/26.00);
							iaFMat[i+1][j] += (dDiff * 8.00/26.00);
							iaFMat[i+1][j-1] += (dDiff * 4.00/26.00);
							iaFMat[i+1][j-2] += (dDiff * 2.00/26.00);
						}
						else if(j == 1)
						{
							iaFMat[i][j-1] += (dDiff * 8.00/24.00);
							iaFMat[i+1][j+2] += (dDiff * 2.00/24.00);
							iaFMat[i+1][j+1] += (dDiff * 4.00/24.00);
							iaFMat[i+1][j] += (dDiff * 8.00/24.00);
							iaFMat[i+1][j-1] += (dDiff * 2.00/24.00);							
						}
						else
						{
							iaFMat[i][j-1] += dDiff * 8.00/32.00;
							iaFMat[i][j-2] += dDiff * 4.00/32.00;
							iaFMat[i+1][j+2] += (dDiff * 2.00/32.00);
							iaFMat[i+1][j+1] += (dDiff * 4.00/32.00);
							iaFMat[i+1][j] += (dDiff * 8.00/32.00);
							iaFMat[i+1][j-1] += (dDiff * 4.00/32.00);
							iaFMat[i+1][j-2] += (dDiff * 2.00/32.00);
						}
					}
				}
				else
				{
					if(j == (iCols-1))
					{
						iaFMat[i][j-1] += (dDiff * 8.00/33.00);
						iaFMat[i][j-2] += (dDiff * 4.00/33.00);
						iaFMat[i+1][j] += (dDiff * 8.00/33.00);
						iaFMat[i+1][j-1] += (dDiff * 4.00/33.00);
						iaFMat[i+1][j-2] += (dDiff * 2.00/33.00);
						iaFMat[i+2][j] += (dDiff * 4.00/33.00);
						iaFMat[i+2][j-1] += (dDiff * 2.00/33.00);
						iaFMat[i+2][j-2] += (dDiff * 1.00/33.00);						
					}
					else if(j == 1)
					{
						iaFMat[i][j-1] += (dDiff * 8.00/35.00);
						iaFMat[i+1][j+2] += (dDiff * 2.00/35.00);
						iaFMat[i+1][j+1] += (dDiff * 4.00/35.00);
						iaFMat[i+1][j] += (dDiff * 8.00/35.00);
						iaFMat[i+1][j-1] += (dDiff * 4.00/35.00);
						iaFMat[i+2][j+2] += (dDiff * 1.00/35.00);
						iaFMat[i+2][j+1] += (dDiff * 2.00/35.00);
						iaFMat[i+2][j] += (dDiff * 4.00/35.00);
						iaFMat[i+2][j-1] += (dDiff * 2.00/35.00);
					}
					else if(j == 0)
					{												
						iaFMat[i+1][j+2] += (dDiff * 2.00/21.00);
						iaFMat[i+1][j+1] += (dDiff * 4.00/21.00);
						iaFMat[i+1][j] += (dDiff * 8.00/21.00);												
						iaFMat[i+2][j+2] += (dDiff * 1.00/21.00);
						iaFMat[i+2][j+1] += (dDiff * 2.00/21.00);
						iaFMat[i+2][j] += (dDiff * 4.00/21.00);
					}
					else
					{
						iaFMat[i][j-1] += (dDiff * 8.00/42.00);
						iaFMat[i][j-2] += (dDiff * 4.00/42.00);
						iaFMat[i+1][j+2] += (dDiff * 2.00/42.00);
						iaFMat[i+1][j+1] += (dDiff * 4.00/42.00);
						iaFMat[i+1][j] += (dDiff * 8.00/42.00);
						iaFMat[i+1][j-1] += (dDiff * 4.00/42.00);
						iaFMat[i+1][j-2] += (dDiff * 2.00/42.00);
						iaFMat[i+2][j+2] += (dDiff * 1.00/42.00);
						iaFMat[i+2][j+1] += (dDiff * 2.00/42.00);
						iaFMat[i+2][j] += (dDiff * 4.00/42.00);
						iaFMat[i+2][j-1] += (dDiff * 2.00/42.00);
						iaFMat[i+2][j-2] += (dDiff * 1.00/42.00);
					}				
				}
			}
		}
	}

	//Write THe IMgae file
	for(int i = 0; i < iRows; i++)
	{
		for(int j = 0; j < iCols; j++)
		{
			Pixel pxP;

			pxP.iBW = iaFMat[i][j];

			imGradOut.SetPixel(i,j,pxP);
		}
	}

	imGradOut.SetImageName(string::basic_string("grad_Stucki_out.raw"));
	imGradOut.WriteImageData();

	cout<<"\nPress 'C' to continue!";
	getch();
}

void Problem_3_C()
{
	Image imHalftone,imHalftoneOut;
	string strHalftone("grad_FS.raw"), strOut("invHT_1st_stage.raw");
	int iRows = 256, iCols = 256;
	int **iaHT = new int*[iRows];
	for(int i = 0;i<iRows;i++)
		iaHT[i] = new int[iCols];

	imHalftone.SetImageName(strHalftone);
	imHalftone.SetImageRows(iRows);
	imHalftone.SetImageCols(iCols);
	imHalftone.SetImageColorFormat(1);
	imHalftone.AllocImageMem();

	imHalftone.ReadImageData();

	imHalftoneOut.SetImageName(strOut);
	imHalftoneOut.SetImageRows(iRows);
	imHalftoneOut.SetImageCols(iCols);
	imHalftoneOut.SetImageColorFormat(1);
	imHalftoneOut.AllocImageMem();

	//convert into int ARRAY
	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel *pxP = imHalftone.GetPixel(i,j);
			if(pxP->GetBW() == 0)
				iaHT[i][j] = 1;
			else
				iaHT[i][j] = 0;

			delete pxP;
		}
	}
	
	//Run length across rows
	double **iaRLRows = new double*[iRows],**iaRLCols = new double*[iRows], **iaRLG = new double*[iRows], **iaRLG2 = new double*[iRows], **iaRLG3 = new double*[iRows];
	for(int i = 0;i<iRows;i++)
	{
		iaRLRows[i] = new double[iCols];
		iaRLCols[i] = new double[iCols];
		iaRLG[i] = new double[iCols];
		iaRLG2[i] = new double[iCols];
		iaRLG3[i] = new double[iCols];
	}
	
	int j = 0;
	for(int i = 0;i<iRows;i++)
	{
		j = 0;
		while(j<iCols)
		{
			int iTemp = iaHT[i][j];
			int iCurrent = j;
			int iCount = 0;

			while(iaHT[i][j] == iTemp)
			{
				iCount++;
				j++;
			}

			while(iCount > 0)
			{
				if(iaHT[i][iCurrent] == 1)
					iaRLRows[i][iCurrent] = (double)((double)iCount/((double)iCount + 1.00));
				else
					iaRLRows[i][iCurrent] = (double)(1.00/((double)iCount + 1.00));

				iCount--;
				iCurrent++;
			}						
		}
	}

	int i = 0;
	for(int j = 0;j<iCols;j++)
	{
		i = 0;
		while(i<iRows)
		{
			int iTemp = iaHT[i][j];
			int iCurrent = i;
			int iCount = 0;

			while(iaHT[i][j] == iTemp)
			{
				iCount++;
				i++;
				if(i == iRows)
				{
					if(iCount != 0)
						iCount--;
					break;
				}				
			}

			while(iCount > 0)
			{
				if(iaHT[iCurrent][j] == 1)
					iaRLCols[iCurrent][j] = (double)((double)iCount/((double)iCount + 1.00));
				else
					iaRLCols[iCurrent][j] = (double)(1.00/((double)iCount + 1.00));

				iCount--;
				iCurrent++;
			}

			if(i == iRows)
				break;
		}
	}

	
	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			double dAvg = 0;
			dAvg = (iaRLRows[i][j] + iaRLCols[i][j])/2.00;
			dAvg *= 255;
			iaRLG[i][j] = dAvg;
			Pixel pxP(dAvg);
			imHalftoneOut.SetPixel(i,j,pxP);
		}
	}	

	imHalftoneOut.WriteImageData();

	//Second Stage
	double daWin[5][5];


	for(int i = 2;i<iRows-2;i++)
	{
		for(int j = 2;j<iCols-2;j++)
		{
			double dMu = 0.00;
			double dMAXVAR, dBeta;

			for(int l = 0;l<5;l++)
			{
				for(int m = 0;m<5;m++)
				{
					daWin[l][m] = iaRLG[i +(l-2)][j+(m-2)];
					dMu += iaRLG[i +(l-2)][j+(m-2)];
				}
			}

			dMu /= 25.00;
			double dSigmaSq = 0.00;
			double dSigma = 0.00;

			for(int l = 0;l<5;l++)
			{
				for(int m = 0;m<5;m++)
				{
					dSigmaSq += pow((daWin[l][m] - dMu),2);
				}
			}

			dSigmaSq /= 25.00;
			dSigma = sqrt(dSigmaSq);

			iaRLG2[i][j] = (dMu + ((dSigmaSq)/(dSigmaSq + 800.00))*(iaRLG[i][j] - dMu));
			
			dMAXVAR = 3.5;
			dBeta = 30;

			if((dSigmaSq <= dMAXVAR ) && ((iaRLG2[i][j] + dBeta) < dMu))
				iaRLG3[i][j] = (iaRLG2[i][j] + dBeta);
			else if((dSigmaSq <= dMAXVAR) && ((iaRLG2[i][j] - dBeta) > dMu))
				iaRLG3[i][j] = (iaRLG2[i][j] - dBeta);			
			else
				iaRLG3[i][j] = iaRLG2[i][j];
		}
	}

	imHalftoneOut.SetImageName(string::basic_string("invHT_Final.raw"));
	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			double dAvg = 0;				 
			dAvg =iaRLG3[i][j];
			Pixel pxP(dAvg);
			imHalftoneOut.SetPixel(i,j,pxP);
		}
	}	

	imHalftoneOut.WriteImageData();
	DeNoiseMedian(&imHalftoneOut,5);
	imHalftoneOut.SetImageName(string::basic_string("invHT_median_op.raw"));
	imHalftoneOut.WriteImageData();
	cout<<"\nPress 'C' to continue!";
	getch();
}