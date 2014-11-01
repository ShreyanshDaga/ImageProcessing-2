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

int SecondStageCount(int iaHitWin[3][3]);
int OutLierFilter(int iaHitWin[3][3]);
int Shrink(int **piaF, int iRows, int iCols);
void Thin(int **piaF, int iRows, int iCols);
void Skeletonize(int **piaF, int iRows, int iCols);
void Dilate(int **piaF, int iRows, int iCols);
int CheckTurn(int iaHitWin[5][5]);

void Problem_2_A()
{
	Image imStars, *pimStarsG;
	string strImName("stars.raw"), strOutput("stars_shrink.raw");

	int iRows = 768, iCols = 1024, iColorFormat = 3;
	int iT;

	int **iaStarsBin;

	imStars.SetImageName(strImName);
	imStars.SetImageRows(iRows);
	imStars.SetImageCols(iCols);
	imStars.SetImageColorFormat(iColorFormat);
	imStars.AllocImageMem();

	imStars.ReadImageData();

	Image imOutput;

	imOutput.SetImageName(strOutput);
	imOutput.SetImageRows(iRows);
	imOutput.SetImageCols(iCols);
	imOutput.SetImageColorFormat(1);
	imOutput.AllocImageMem();

	pimStarsG = imStars.Convert2Gray();

	pimStarsG->PlotHistogram();

	cout<<"\n Enter Min Threshold for Background: (Enter anythgin between 27 to 35)";
	cin>>iT;

	//Allocate Data for binary image
	iaStarsBin = new int*[iRows];
	for(int i = 0;i<iRows;i++)
		iaStarsBin[i] = new int[iCols];

	cout<<"\n*Convering Image into Binary...";
	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel *pxP = pimStarsG->GetPixel(i,j);

			if(pxP->GetBW() >= iT)
			{
				iaStarsBin[i][j] = 1;
				pxP->iBW = 0;
				pimStarsG->SetPixel(i,j,*pxP);
			}
			else
			{
				iaStarsBin[i][j] = 0;
				pxP->iBW = 255;
				pimStarsG->SetPixel(i,j,*pxP);
			}

			delete pxP;
		}
	}
	
	//Binary image for stars.raw
	pimStarsG->WriteImageData();

	int **iaStarsExBin;
	int iaHitWin[3][3];
	int **iaM = new int*[iRows];
	int **iaG = new int*[iRows];
	int **iaF = new int*[iRows];
	int **iaFEx = new int*[iRows +2];
	int **iaMEx = new int*[iRows + 2];
	int iHitCount = 1;

	//M Extended Matrix
	for(int i = 0;i<iRows + 2;i++)
	{
		iaFEx[i] = new int[iCols + 2];
		iaMEx[i] = new int[iCols + 2];
	}

	//M & G Matrix
	for(int i = 0;i<iRows;i++)
	{
		iaM[i] = new int[iCols];
		iaG[i] = new int[iCols];
		iaF[i] = new int[iCols];
	}

	//Fill M Matrix With Zeros
	for(int i = 0;i<iRows;i++)
		for(int j = 0;j<iCols;j++)
		{
			iaM[i][j] = 0;
			iaG[i][j] = iaStarsBin[i][j];
			iaF[i][j] = iaStarsBin[i][j];
		}	

	for(int jj = 0;jj<iRows + 2;jj++)
	{
		for(int kk = 0;kk<iCols + 2;kk++)
		{
			iaFEx[jj][kk] = 0;
			iaMEx[jj][kk] = 0;
		}
	}		
	//Extend F to FEx		
	for(int jj = 1;jj<iRows +1;jj++)
	{
		for(int kk = 1;kk<iCols + 1;kk++)
		{
			iaFEx[jj][kk] = iaF[jj-1][kk-1];				
		}
	}

	cout<<"\n*Counting Stars...";
	int iSize = 0;
	while(1)
	{
		//Reset MEx to zero
		for(int jj = 0;jj<iRows + 2;jj++)
		{
			for(int kk = 0;kk<iCols + 2;kk++)
			{				
				iaMEx[jj][kk] = 0;
			}
		}

		//First Stage
		for(int j = 1;j<iRows+1;j++)
		{
			for(int k = 1;k<iCols+1;k++)
			{				
				if(iaFEx[j][k] == 0)
					continue;
				//Calculate Bond
				int iBond = 0;
				iBond = iaFEx[j-1][k-1] + iaFEx[j+1][k-1] + iaFEx[j+1][k+1] + iaFEx[j-1][k+1];
				iBond += 2*(iaFEx[j][k-1] + iaFEx[j][k+1] + iaFEx[j-1][k] + iaFEx[j+1][k]);
				
				//Get The Surrounding Pixels
				for(int l = 0;l<3;l++)
				{
					for(int m = 0;m<3;m++)
					{
						iaHitWin[l][m] = iaFEx[j+(l-1)][k + (m-1)];
					}
				}

				int iHit = 0;
				//Select Mask Pattern based on bond
				switch(iBond)
				{
					case 1:			
						iHit = HitMiss_S_Bond_1(iaHitWin);				
					break;

					case 2:				
						iHit = HitMiss_S_Bond_2(iaHitWin);				
					break;

					case 3:				
						iHit = HitMiss_S_Bond_3(iaHitWin);				
					break;

					case 4:
						iHit = HitMiss_STK_Bond_4(iaHitWin);
					break;

					case 5:
						iHit = HitMiss_ST_Bond_5(iaHitWin);
					break;

					case 6:
						if(HitMiss_ST_Bond_6(iaHitWin) || HitMiss_STK_Bond_6(iaHitWin))
							iHit = 1;
						else 
							iHit = 0;					
					break;

					case 7:
						iHit = HitMiss_STK_Bond_7(iaHitWin);
					break;

					case 8:
						iHit = HitMiss_STK_Bond_8(iaHitWin);
					break;

					case 9:
						iHit = HitMiss_STK_Bond_9(iaHitWin);
					break;

					case 10:
						iHit = HitMiss_STK_Bond_10(iaHitWin);
					break;
				}
				/////////////////

				//If Hit Selecte Candidate in MEx(matrix);
				if(iHit)
				{					
					iaMEx[j][k] = 1;		
				}
				else
				{
					iaMEx[j][k] = 0;
				}
			}
		}
		//First Stage Complete						
		
		//MEx count
		int iCount = 0;
		//Find Hit Count
		for(int j = 1;j<iRows+1;j++)
		{
			for(int k = 1;k<iCols+1;k++)
			{
				if(iaMEx[j][k] == 1)
					iCount++;
			}
		}
		iHitCount = iCount;
		if(iHitCount == 0)
			break;

		//Second Stage	
		for(int j = 1;j<iRows+1;j++)
		{
			for(int k = 1;k<iCols+1;k++)
			{				
				if(iaMEx[j][k] == 0)
					continue;
				//Get The Surrounding Pixels
				for(int l = 0;l<3;l++)
				{
					for(int m = 0;m<3;m++)
					{
						iaHitWin[l][m] = iaMEx[j + (l-1)][k + (m-1)];
					}
				}

				int iHit = 0;//SecondStageCount(iaHitWin);

				iHit += UnMask_ST_1(iaHitWin);				
				iHit += UnMask_ST_2(iaHitWin);
				iHit += UnMask_ST_3(iaHitWin);
				iHit += UnMask_ST_4(iaHitWin);
				iHit += UnMask_ST_5(iaHitWin);
				iHit += UnMask_ST_6(iaHitWin);
				iHit += UnMask_ST_7(iaHitWin);
				iHit += UnMask_ST_8(iaHitWin);
				iHit += UnMask_ST_9(iaHitWin);
				
				if(iHit != 0)
				{					
					iaMEx[j][k] = 0;					
				}						
			}
		}		
		//Find Count of candidates
		int iMCount = 0;
		for(int j = 1;j<iRows+1;j++)
		{
			for(int k = 1;k<iCols+1;k++)
			{
				if(iaMEx[j][k] == 1)
					iMCount++;
			}
		}
		cout<<"\nStar Count: "<<iCount - iMCount<<", Star Size: "<<iSize + 2;
		iSize++;

		//Delete 1s in the binary
		for(int j = 1;j<iRows+1;j++)
		{
			for(int k = 1;k<iCols+1;k++)
			{
				if(iaMEx[j][k] == 1)
					iaFEx[j][k] = 0;
			}
		}		
		//Second Stage COmplete
	}

	//Display Image
	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel pxPB(0),pxPW(255);
			if(iaFEx[i+1][j+1] == 1)
				imOutput.SetPixel(i,j,pxPW);
			else
				imOutput.SetPixel(i,j,pxPB);
		}
	}

	imOutput.WriteImageData();

	int iCount = 0;
	for(int j = 0;j<iRows;j++)
	{
		for(int k = 0;k<iCols;k++)
		{
			if(iaFEx[j+1][k+1] == 1)
			{
				iCount++;
			}
		}
	}
	cout<<"\Number Of Stars: "<<iCount;
	cout<<"\nPress 'C' to continue!";
	getch();
}

void Problem_2_B()
{
	Image imDigits, *pimDigitsG, imDigitsThinned, *pimDigitsBin;
	string strImName("shapes.raw"), strOutput("shapes_thinned.raw");

	int iRows = 500, iCols = 500, iColorFormat = 3;
	int iT;

	int **iaDigitsBin;

	imDigits.SetImageName(strImName);
	imDigits.SetImageRows(iRows);
	imDigits.SetImageCols(iCols);
	imDigits.SetImageColorFormat(iColorFormat);
	imDigits.AllocImageMem();

	imDigits.ReadImageData();	

	imDigitsThinned.SetImageName(strOutput);
	imDigitsThinned.SetImageRows(iRows);
	imDigitsThinned.SetImageCols(iCols);
	imDigitsThinned.SetImageColorFormat(1);
	imDigitsThinned.AllocImageMem();

	pimDigitsG = imDigits.Convert2Gray();
	pimDigitsG->WriteImageData();
	pimDigitsG->PlotHistogram();

	cout<<"\n Enter Min Threshold of Background for Binary conversion:(ENter 151) ";
	cin>>iT;

	//Allocate Data for binary image
	iaDigitsBin = new int*[iRows];
	for(int i = 0;i<iRows;i++)
		iaDigitsBin[i] = new int[iCols];

	cout<<"\n*Convering Image into Binary...";
	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel *pxP = pimDigitsG->GetPixel(i,j);

			if(pxP->GetBW() >= iT)
			{
				iaDigitsBin[i][j] = 0;
				pxP->iBW = 255;
				pimDigitsG->SetPixel(i,j,*pxP);
			}
			else
			{
				iaDigitsBin[i][j] = 1;
				pxP->iBW = 0;
				pimDigitsG->SetPixel(i,j,*pxP);
			}
			delete pxP;
		}
	}
	
	//Delete Outlier Pixels
	int iaHitWin[3][3];
	for(int i = 1;i<iRows-1;i++)
	{
		for(int j = 1;j<iCols-1;j++)
		{
			if(iaDigitsBin[i][j] == 0)
				continue;
	
			for(int l = 0;l<3;l++)
			{
				for(int m = 0;m<3;m++)
				{
					iaHitWin[l][m] = iaDigitsBin[i+(l-1)][j + (m-1)];
				}
			}
	
			if(OutLierFilter(iaHitWin))
				iaDigitsBin[i][j] = 0;
		}
	}

	//Binary image for digits.raw
	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel pxP;

			if(iaDigitsBin[i][j] == 0)
			{				
				pxP.iBW = 255;
				pimDigitsG->SetPixel(i,j,pxP);
			}
			else
			{				
				pxP.iBW = 0;
				pimDigitsG->SetPixel(i,j,pxP);
			}
		}
	}

	//Write File
	pimDigitsG->SetImageName(string::basic_string("digits_bin.raw"));
	pimDigitsG->WriteImageData();

	//Thin the Image
	int **iaM = new int*[iRows];
	int **iaF = new int*[iRows];
	int **iaFEx = new int*[iRows +2];
	int **iaMEx = new int*[iRows + 2];
	int iHitCount = 1;

	//M Extended Matrix
	for(int i = 0;i<iRows + 2;i++)
	{
		iaFEx[i] = new int[iCols + 2];
		iaMEx[i] = new int[iCols + 2];
	}

	//M & F Matrix
	for(int i = 0;i<iRows;i++)
	{
		iaM[i] = new int[iCols];
		iaF[i] = new int[iCols];
	}

	//Fill M Matrix With Zeros
	for(int i = 0;i<iRows;i++)
		for(int j = 0;j<iCols;j++)
		{
			iaM[i][j] = 0;
			iaF[i][j] = iaDigitsBin[i][j];
		}	

	for(int jj = 0;jj<iRows + 2;jj++)
	{
		for(int kk = 0;kk<iCols + 2;kk++)
		{
			iaFEx[jj][kk] = 0;
			iaMEx[jj][kk] = 0;
		}
	}		
	//Extend F to FEx		
	for(int jj = 1;jj<iRows +1;jj++)
	{
		for(int kk = 1;kk<iCols + 1;kk++)
		{
			iaFEx[jj][kk] = iaF[jj-1][kk-1];				
		}
	}

	int iSize = 0;

	while(1)
	{
		//Reset MEx to zero
		for(int jj = 0;jj<iRows + 2;jj++)
		{
			for(int kk = 0;kk<iCols + 2;kk++)
			{				
				iaMEx[jj][kk] = 0;
			}
		}

		//First Stage
		for(int j = 1;j<iRows+1;j++)
		{
			for(int k = 1;k<iCols+1;k++)
			{				
				if(iaFEx[j][k] == 0)
					continue;
				//Calculate Bond
				int iBond = 0;
				iBond = iaFEx[j-1][k-1] + iaFEx[j+1][k-1] + iaFEx[j+1][k+1] + iaFEx[j-1][k+1];
				iBond += 2*(iaFEx[j][k-1] + iaFEx[j][k+1] + iaFEx[j-1][k] + iaFEx[j+1][k]);
				
				//Get The Surrounding Pixels
				for(int l = 0;l<3;l++)
				{
					for(int m = 0;m<3;m++)
					{
						iaHitWin[l][m] = iaFEx[j+(l-1)][k + (m-1)];
					}
				}

				int iHit = 0;
				//Select Mask Pattern based on bond
				switch(iBond)
				{
					//case 1:			
					//	iHit = HitMiss_S_Bond_1(iaHitWin);				
					//break;
					//
					//case 2:				
					//	iHit = HitMiss_S_Bond_2(iaHitWin);				
					//break;
					//
					//case 3:				
					//	iHit = HitMiss_S_Bond_3(iaHitWin);				
					//break;

					case 4:
						//
						if(HitMiss_STK_Bond_4(iaHitWin) || HitMiss_TK_Bond_4(iaHitWin))
							iHit = 1;
						else
							iHit = 0;
					break;

					case 5:
						iHit = HitMiss_ST_Bond_5(iaHitWin);
					break;

					case 6:
						if(HitMiss_ST_Bond_6(iaHitWin) || HitMiss_STK_Bond_6(iaHitWin))
							iHit = 1;
						else 
							iHit = 0;					
					break;

					case 7:
						iHit = HitMiss_STK_Bond_7(iaHitWin);
					break;

					case 8:
						iHit = HitMiss_STK_Bond_8(iaHitWin);
					break;

					case 9:
						iHit = HitMiss_STK_Bond_9(iaHitWin);
					break;

					case 10:
						iHit = HitMiss_STK_Bond_10(iaHitWin);
					break;
				}
				/////////////////

				//If Hit Selecte Candidate in MEx(matrix);
				if(iHit)
				{					
					iaMEx[j][k] = 1;		
				}
				else
				{
					iaMEx[j][k] = 0;
				}
			}
		}
		//First Stage Complete						
		
		//MEx count
		int iCount = 0;
		//Find Hit Count
		for(int j = 1;j<iRows+1;j++)
		{
			for(int k = 1;k<iCols+1;k++)
			{
				if(iaMEx[j][k] == 1)
					iCount++;
			}
		}
		iHitCount = iCount;
		if(iHitCount == 0)
			break;

		//Second Stage	
		for(int j = 1;j<iRows+1;j++)
		{
			for(int k = 1;k<iCols+1;k++)
			{				
				if(iaMEx[j][k] == 0)
					continue;
				//Get The Surrounding Pixels
				for(int l = 0;l<3;l++)
				{
					for(int m = 0;m<3;m++)
					{
						iaHitWin[l][m] = iaMEx[j + (l-1)][k + (m-1)];
					}
				}

				int iHit = 0;//SecondStageCount(iaHitWin);

				iHit += UnMask_ST_1(iaHitWin);				
				iHit += UnMask_ST_2(iaHitWin);
				iHit += UnMask_ST_3(iaHitWin);
				iHit += UnMask_ST_4(iaHitWin);
				iHit += UnMask_ST_5(iaHitWin);
				iHit += UnMask_ST_6(iaHitWin);
				iHit += UnMask_ST_7(iaHitWin);
				iHit += UnMask_ST_8(iaHitWin);
				iHit += UnMask_ST_9(iaHitWin);
				
				if(iHit != 0)
				{					
					iaMEx[j][k] = 0;					
				}						
			}
		}		
		//Find Count of candidates
		int iMCount = 0;
		for(int j = 1;j<iRows+1;j++)
		{
			for(int k = 1;k<iCols+1;k++)
			{
				if(iaMEx[j][k] == 1)
					iMCount++;
			}
		}

		if(iMCount == 0)
			break;

		//Delete 1s in the binary
		for(int j = 1;j<iRows+1;j++)
		{
			for(int k = 1;k<iCols+1;k++)
			{
				if(iaMEx[j][k] == 1)
					iaFEx[j][k] = 0;
			}
		}		
		//Second Stage COmplete
	}

	//Outlier Filter
	//for(int i = 1;i<iRows+1;i++)
	//{
	//	for(int j = 1;j<iCols+1;j++)
	//	{
	//		if(iaFEx[i][j] == 0)
	//			continue;
	//
	//		for(int l = 0;l<3;l++)
	//		{
	//			for(int m = 0;m<3;m++)
	//			{
	//				iaHitWin[l][m] = iaFEx[i+(l-1)][j + (m-1)];
	//			}
	//		}
	//
	//		if(OutLierFilter(iaHitWin))
	//			iaFEx[i][j] = 0;
	//	}
	//}

	//Display Image
	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel pxPB(0),pxPW(255);
			if(iaFEx[i+1][j+1] == 1)
				imDigitsThinned.SetPixel(i,j,pxPB);
			else
				imDigitsThinned.SetPixel(i,j,pxPW);
		}
	}

	imDigitsThinned.WriteImageData();
	cout<<"\nPress 'C' to continue!";
	getch();
}

void Problem_2_C()
{
	Image imDino, *pimDinoG, imOutput;
	string strImName("dinosaur.raw"), strOutput("dino_K.raw"), strDinoBin("dino_bin.raw");

	int iRows = 444, iCols = 960, iColorFormat = 3;
	int iT;

	int **iaDinoBin;

	imDino.SetImageName(strImName);
	imDino.SetImageRows(iRows);
	imDino.SetImageCols(iCols);
	imDino.SetImageColorFormat(iColorFormat);
	imDino.AllocImageMem();

	imDino.ReadImageData();	

	imOutput.SetImageName(strOutput);
	imOutput.SetImageRows(iRows);
	imOutput.SetImageCols(iCols);
	imOutput.SetImageColorFormat(1);
	imOutput.AllocImageMem();

	pimDinoG = imDino.Convert2Gray();

	pimDinoG->WriteImageData();
	pimDinoG->PlotHistogram();

	cout<<"\n Enter Min Threshold of Background for Binary conversion:(Enter 65) ";
	cin>>iT;

	//Allocate Data for binary image
	iaDinoBin = new int*[iRows];
	for(int i = 0;i<iRows;i++)
		iaDinoBin[i] = new int[iCols];

	cout<<"\n*Convering Image into Binary...";
	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel *pxP = pimDinoG->GetPixel(i,j);

			if(pxP->GetBW() <= iT)
			{
				iaDinoBin[i][j] = 0;
			}
			else
			{
				iaDinoBin[i][j] = 1;
			}

			delete pxP;
		}
	}
		
	//Delete Outlier Pixels
	int iaHitWin[3][3];
	for(int i = 1;i<iRows-1;i++)
	{
		for(int j = 1;j<iCols-1;j++)
		{
			if(iaDinoBin[i][j] == 0)
				continue;

			for(int l = 0;l<3;l++)
			{
				for(int m = 0;m<3;m++)
				{
					iaHitWin[l][m] = iaDinoBin[i+(l-1)][j + (m-1)];
				}
			}

			if(OutLierFilter(iaHitWin))
				iaDinoBin[i][j] = 0;
		}
	}

	//Dilate
	Dilate(iaDinoBin,iRows,iCols);
	Dilate(iaDinoBin,iRows,iCols);
	Dilate(iaDinoBin,iRows,iCols);
	Dilate(iaDinoBin,iRows,iCols);

	//Binary image for dino.raw
	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel pxP;

			if(iaDinoBin[i][j] == 0)
			{				
				pxP.iBW = 255;
				pimDinoG->SetPixel(i,j,pxP);
			}
			else
			{				
				pxP.iBW = 0;
				pimDinoG->SetPixel(i,j,pxP);
			}
		}
	}

	//Apply Median filter
	cout<<"\n*Please Wait for Computation....";
	DeNoiseMedian(pimDinoG,7);

	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel *pxP = pimDinoG->GetPixel(i,j);

			if(pxP->GetBW() == 0)
			{								
				iaDinoBin[i][j] = 1;
			}
			else
			{				
				iaDinoBin[i][j] = 0;
			}
		}
	}

	//Binary image for dino.raw
	pimDinoG->SetImageName(strDinoBin);
	pimDinoG->WriteImageData();

	//Skeletonize the Image
	cout<<"\n*Skeltonizing...";
	int **iaM = new int*[iRows];
	int **iaF = new int*[iRows];
	int **iaFEx = new int*[iRows +2];
	int **iaMEx = new int*[iRows + 2];
	int iHitCount = 1;

	//M Extended Matrix
	for(int i = 0;i<iRows + 2;i++)
	{
		iaFEx[i] = new int[iCols + 2];
		iaMEx[i] = new int[iCols + 2];
	}

	//M & G Matrix
	for(int i = 0;i<iRows;i++)
	{
		iaM[i] = new int[iCols];
		iaF[i] = new int[iCols];
	}

	//Fill M Matrix With Zeros
	for(int i = 0;i<iRows;i++)
		for(int j = 0;j<iCols;j++)
		{
			iaM[i][j] = 0;
			iaF[i][j] = iaDinoBin[i][j];
		}	

	for(int jj = 0;jj<iRows + 2;jj++)
	{
		for(int kk = 0;kk<iCols + 2;kk++)
		{
			iaFEx[jj][kk] = 0;
			iaMEx[jj][kk] = 0;
		}
	}		
	//Extend F to FEx		
	for(int jj = 1;jj<iRows +1;jj++)
	{
		for(int kk = 1;kk<iCols + 1;kk++)
		{
			iaFEx[jj][kk] = iaF[jj-1][kk-1];				
		}
	}

	int iSize = 0;
	while(1)
	{
		//Reset MEx to zero
		for(int jj = 0;jj<iRows + 2;jj++)
		{
			for(int kk = 0;kk<iCols + 2;kk++)
			{				
				iaMEx[jj][kk] = 0;
			}
		}

		//First Stage
		for(int j = 1;j<iRows+1;j++)
		{
			for(int k = 1;k<iCols+1;k++)
			{				
				if(iaFEx[j][k] == 0)
					continue;
				//Calculate Bond
				int iBond = 0;
				iBond = iaFEx[j-1][k-1] + iaFEx[j+1][k-1] + iaFEx[j+1][k+1] + iaFEx[j-1][k+1];
				iBond += 2*(iaFEx[j][k-1] + iaFEx[j][k+1] + iaFEx[j-1][k] + iaFEx[j+1][k]);
				
				//Get The Surrounding Pixels
				for(int l = 0;l<3;l++)
				{
					for(int m = 0;m<3;m++)
					{
						iaHitWin[l][m] = iaFEx[j+(l-1)][k + (m-1)];
					}
				}

				int iHit = 0;
				//Select Mask Pattern based on bond
				switch(iBond)
				{					
					case 4:
						//
						if(HitMiss_STK_Bond_4(iaHitWin) || HitMiss_TK_Bond_4(iaHitWin))
							iHit = 1;
						else
							iHit = 0;
					break;

					case 6:
						if(HitMiss_STK_Bond_6(iaHitWin))
							iHit = 1;
						else 
							iHit = 0;					
					break;

					case 7:
						iHit = HitMiss_STK_Bond_7(iaHitWin);
					break;

					case 8:
						iHit = HitMiss_STK_Bond_8(iaHitWin);
					break;

					case 9:
						iHit = HitMiss_STK_Bond_9(iaHitWin);
					break;

					case 10:
						iHit = HitMiss_STK_Bond_10(iaHitWin);
					break;

					case 11:
						iHit = HitMiss_K_Bond_11(iaHitWin);
					break;
				}
				/////////////////

				//If Hit Selecte Candidate in MEx(matrix);
				if(iHit)
				{					
					iaMEx[j][k] = 1;		
				}
				else
				{
					iaMEx[j][k] = 0;
				}
			}
		}
		//First Stage Complete						
		
		//MEx count
		int iCount = 0;
		//Find Hit Count
		for(int j = 1;j<iRows+1;j++)
		{
			for(int k = 1;k<iCols+1;k++)
			{
				if(iaMEx[j][k] == 1)
					iCount++;
			}
		}
		iHitCount = iCount;
		if(iHitCount == 0)
			break;

		//Second Stage	
		for(int j = 1;j<iRows+1;j++)
		{
			for(int k = 1;k<iCols+1;k++)
			{				
				if(iaMEx[j][k] == 0)
					continue;
				//Get The Surrounding Pixels
				for(int l = 0;l<3;l++)
				{
					for(int m = 0;m<3;m++)
					{
						iaHitWin[l][m] = iaMEx[j + (l-1)][k + (m-1)];
					}
				}

				int iHit = 0;//SecondStageCount(iaHitWin);

				iHit += UnMask_K_1(iaHitWin);				
				iHit += UnMask_K_2(iaHitWin);
				iHit += UnMask_K_3(iaHitWin);
				iHit += UnMask_K_4(iaHitWin);
				iHit += UnMask_K_5(iaHitWin);
				iHit += UnMask_K_6(iaHitWin);
				iHit += UnMask_K_7(iaHitWin);				
				
				if(iHit != 0)
				{					
					iaMEx[j][k] = 0;					
				}						
			}
		}		
		//Find Count of candidates
		int iMCount = 0;
		for(int j = 1;j<iRows+1;j++)
		{
			for(int k = 1;k<iCols+1;k++)
			{
				if(iaMEx[j][k] == 1)
					iMCount++;
			}
		}

		if(iMCount == 0)
			break;

		//Delete 1s in the binary
		for(int j = 1;j<iRows+1;j++)
		{
			for(int k = 1;k<iCols+1;k++)
			{
				if(iaMEx[j][k] == 1)
					iaFEx[j][k] = 0;
			}
		}		
		//Second Stage COmplete
	}
	//Display Image
	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel pxPB(0),pxPW(255);
			if(iaFEx[i+1][j+1] == 1)
				imOutput.SetPixel(i,j,pxPB);
			else
				imOutput.SetPixel(i,j,pxPW);
		}
	}

	imOutput.WriteImageData();

	cout<<"\nPress 'C' to continue!";
	getch();
}

void Problem_2_D()
{
	Image imPacMan, imPacManG_Walls, imPacManG_Whites, *pimPacManG;
	int iRows = 441, iCols = 441;
	int **iaPacManBinWalls = new int*[iRows];
	int **iaPacManBinWhites = new int*[iRows];
	int iTwalls, iTwhites;
	string strNames[4] = {"pacman.raw","pacman_walls_bin.raw","pacman_gray.raw","pacman_whites_bin.raw"};

	for(int i = 0;i<iRows;i++)
	{
		iaPacManBinWalls[i] = new int[iCols];
		iaPacManBinWhites[i] = new int[iCols];
	}

	imPacMan.SetImageName(strNames[0]);
	imPacMan.SetImageRows(iRows);
	imPacMan.SetImageCols(iCols);
	imPacMan.SetImageColorFormat(3);
	imPacMan.AllocImageMem();

	imPacManG_Walls.SetImageName(strNames[1]);
	imPacManG_Walls.SetImageRows(iRows);
	imPacManG_Walls.SetImageCols(iCols);
	imPacManG_Walls.SetImageColorFormat(1);
	imPacManG_Walls.AllocImageMem();

	imPacManG_Whites.SetImageName(strNames[3]);
	imPacManG_Whites.SetImageRows(iRows);
	imPacManG_Whites.SetImageCols(iCols);
	imPacManG_Whites.SetImageColorFormat(1);
	imPacManG_Whites.AllocImageMem();

	imPacMan.ReadImageData();
	imPacMan.PlotHistogram();

	pimPacManG = imPacMan.Convert2Gray();
	pimPacManG->WriteImageData();
	pimPacManG->PlotHistogram();

	cout<<"\nEnter Threshold for walls: (Enter 89)";
	cin>>iTwalls;

	cout<<"\nEnter Threshold for white balls: (Enter 255) ";
	cin>>iTwhites;

	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel *pxP = pimPacManG->GetPixel(i,j);
			Pixel pxPB(0), pxPW(255);

			if(pxP->GetBW() == iTwalls)
			{
				iaPacManBinWalls[i][j] = 1;
				imPacManG_Walls.SetPixel(i,j,pxPB);
			}
			else
			{
				iaPacManBinWalls[i][j] = 0;
				imPacManG_Walls.SetPixel(i,j,pxPW);
			}

			if(pxP->GetBW() == iTwhites)
			{
				iaPacManBinWhites[i][j] = 1;
				imPacManG_Whites.SetPixel(i,j,pxPB);
			}
			else
			{
				iaPacManBinWhites[i][j] = 0;
				imPacManG_Whites.SetPixel(i,j,pxPW);
			}
			delete pxP;
		}
	}	
	
	imPacManG_Walls.WriteImageData();
	//imPacManG_Whites.WriteImageData();
	//Median FIlter over whites
	DeNoiseMedian(&imPacManG_Whites,5);
	imPacManG_Whites.WriteImageData();

	//Re caliberate Int array for whites
	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel *pxP = imPacManG_Whites.GetPixel(i,j);
			if(pxP->GetBW() == 0)
			{
				iaPacManBinWhites[i][j] = 1;
			}
			else
			{
				iaPacManBinWhites[i][j] = 0;
			}			
		}
	}

	//Shrink walls
	int iWallCount = Shrink(iaPacManBinWalls,iRows,iCols);	
	//Shrink balls
	int iWhiteCount = Shrink(iaPacManBinWhites,iRows,iCols);

	cout<<"\nWall Count: "<<iWallCount - 2;
	cout<<"\nWhite Ball Count: "<<iWhiteCount - 4;

	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel pxPB(0), pxPW(255);

			if(iaPacManBinWalls[i][j] == 1)
			{
				imPacManG_Walls.SetPixel(i,j,pxPW);
			}
			else
			{
				imPacManG_Walls.SetPixel(i,j,pxPB);
			}

			if(iaPacManBinWhites[i][j] == 1)
			{
				imPacManG_Whites.SetPixel(i,j,pxPW);
			}
			else
			{
				imPacManG_Whites.SetPixel(i,j,pxPB);
			}
		}
	}

	string strWallsShr("pacman_wall_shr.raw");
	string strWhiteShr("pacman_white_shr.raw");

	imPacManG_Walls.SetImageName(strWallsShr);
	imPacManG_Whites.SetImageName(strWhiteShr);

	imPacManG_Walls.WriteImageData();
	imPacManG_Whites.WriteImageData();
	
	//Get Walls On New threshold
	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel *pxP = pimPacManG->GetPixel(i,j);

			if( pxP->GetBW() == 89 || pxP->GetBW() == 141)
			{
				iaPacManBinWalls[i][j] = 1;
			}
			else
			{
				iaPacManBinWalls[i][j] = 0;
			}
		}
	}

	//Show Walls
	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel pxPB(0), pxPW(255);

			if(iaPacManBinWalls[i][j] == 1)
			{
				imPacManG_Walls.SetPixel(i,j,pxPB);
			}
			else
			{
				imPacManG_Walls.SetPixel(i,j,pxPW);
			}
		}	
	}	
	imPacManG_Walls.SetImageName(string("pacman_walls_bin_2.raw"));
	imPacManG_Walls.WriteImageData();

	//Skeletonise the walls
	Skeletonize(iaPacManBinWalls,iRows,iCols);
	for(int i = 0;i<iRows;i++)
	{
		for(int j  = 0;j<iCols;j++)
		{
			Pixel pxPB(0), pxPW(255);

			if(iaPacManBinWalls[i][j] == 1)
				imPacManG_Walls.SetPixel(i,j,pxPB);
			else
				imPacManG_Walls.SetPixel(i,j,pxPW);

		}
	}
	string strWallsTurn("pacman_wall_skeleton.raw");
	imPacManG_Walls.SetImageName(strWallsTurn);
	imPacManG_Walls.WriteImageData();

	//Count Turns
	int iTurnCount = 0;
	int iaHitWin[5][5];
	for(int i = 2;i<iRows-2;i++)
	{
		for(int j = 2;j<iCols-2;j++)
		{
			if(iaPacManBinWalls[i][j] == 0)
				continue;
	
			for(int l = 0;l<5;l++)
			{				
				for(int m = 0;m<5;m++)
				{
					iaHitWin[l][m] = iaPacManBinWalls[i + (l-2)][j + (m-2)];
				}
			}
	
			if(CheckTurn(iaHitWin))
				iTurnCount += 2;
		}
	}

	cout<<"\nNumber of turns: "<<iTurnCount - 8;
	cout<<"\nPress 'C' to continue!";
	getch();
}

int HitMiss_S_Bond_1(int iaHitWin[3][3])
{
	int iHit = 0;
	int j = 1,k = 1;

	int X0 = iaHitWin[0][0];
	int X1 = iaHitWin[0][1];
	int X2 = iaHitWin[0][2];
	int X3 = iaHitWin[1][2];
	int X4 = iaHitWin[2][2];
	int X5 = iaHitWin[2][1];
	int X6 = iaHitWin[2][0];
	int X7 = iaHitWin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;

	//Pattern 1 || Pattern 2 || Pattern 3 || Pattern 4 == 1 for a hit
	if((iBval == 4) || (iBval == 1) || (iBval == 16) || (iBval == 64))
	{
		iHit = 1;
	}
	else
		iHit = 0;

	return iHit;
}

int HitMiss_S_Bond_2(int iaHitWin[3][3])
{
	int iHit = 0;
	int j = 1,k = 1;

	int X0 = iaHitWin[0][0];
	int X1 = iaHitWin[0][1];
	int X2 = iaHitWin[0][2];
	int X3 = iaHitWin[1][2];
	int X4 = iaHitWin[2][2];
	int X5 = iaHitWin[2][1];
	int X6 = iaHitWin[2][0];
	int X7 = iaHitWin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;

	//Pattern 1 || Pattern 2 || Pattern 3 || Pattern 4 == 1 for a hit
	if((iBval == 32) || (iBval == 2) || (iBval == 8) || (iBval == 128))
	{
		iHit = 1;
	}
	else
		iHit = 0;

	return iHit;
}

int HitMiss_S_Bond_3(int iaHitWin[3][3])
{
	int iHit = 0;
	int j = 1,k = 1;

	int X0 = iaHitWin[0][0];
	int X1 = iaHitWin[0][1];
	int X2 = iaHitWin[0][2];
	int X3 = iaHitWin[1][2];
	int X4 = iaHitWin[2][2];
	int X5 = iaHitWin[2][1];
	int X6 = iaHitWin[2][0];
	int X7 = iaHitWin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;

	if((iBval == 12) || (iBval == 6) || (iBval == 3) || (iBval == 129) || (iBval == 192) || (iBval == 96) || (iBval == 48) || (iBval == 24))	
		iHit = 1;	
	else
		iHit = 0;

	return iHit;
}

int HitMiss_TK_Bond_4(int iaHitWin[3][3])
{
	int iHit = 0;
	int j = 1,k = 1;

	int X0 = iaHitWin[0][0];
	int X1 = iaHitWin[0][1];
	int X2 = iaHitWin[0][2];
	int X3 = iaHitWin[1][2];
	int X4 = iaHitWin[2][2];
	int X5 = iaHitWin[2][1];
	int X6 = iaHitWin[2][0];
	int X7 = iaHitWin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;

	if((iBval == 10) || (iBval == 130) || (iBval == 160) || (iBval == 40))
		iHit = 1;	
	else
		iHit = 0;

	return iHit;
}

int HitMiss_STK_Bond_4(int iaHitWin[3][3])
{
	int iHit = 0;
	int j = 1,k = 1;

	int X0 = iaHitWin[0][0];
	int X1 = iaHitWin[0][1];
	int X2 = iaHitWin[0][2];
	int X3 = iaHitWin[1][2];
	int X4 = iaHitWin[2][2];
	int X5 = iaHitWin[2][1];
	int X6 = iaHitWin[2][0];
	int X7 = iaHitWin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;

	if((iBval == 28) || (iBval == 7) || (iBval == 193) || (iBval == 112))	
		iHit = 1;	
	else
		iHit = 0;

	return iHit;
}

int HitMiss_ST_Bond_5(int iaHitWin[3][3])
{
	int iHit = 0;
	int j = 1,k = 1;
	int X0 = iaHitWin[0][0];
	int X1 = iaHitWin[0][1];
	int X2 = iaHitWin[0][2];
	int X3 = iaHitWin[1][2];
	int X4 = iaHitWin[2][2];
	int X5 = iaHitWin[2][1];
	int X6 = iaHitWin[2][0];
	int X7 = iaHitWin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;
	
	if((iBval == 11) || (iBval == 26) || (iBval == 134) || (iBval == 44) || (iBval == 14) || (iBval == 131) || (iBval == 224) || (iBval == 56))	
		iHit = 1;	
	else
		iHit = 0;

	return iHit;
}

int HitMiss_ST_Bond_6(int iaHitWin[3][3])
{
	int iHit = 0;
	int j = 1,k = 1;
	int X0 = iaHitWin[0][0];
	int X1 = iaHitWin[0][1];
	int X2 = iaHitWin[0][2];
	int X3 = iaHitWin[1][2];
	int X4 = iaHitWin[2][2];
	int X5 = iaHitWin[2][1];
	int X6 = iaHitWin[2][0];
	int X7 = iaHitWin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;
	//Pattern 1 || Pattern 2 || Pattern 3 || Pattern 4 == 1 for a hit
	if((iBval == 27) || (iBval == 198))	// || (iBval == 23) || (iBval == 150) || (iBval == 15) || (iBval == 43) || (iBval == 105) || (iBval == 232) || (iBval == 240) || (iBval == 212))	
		iHit = 1;	
	else
		iHit = 0;

	return iHit;
}

int HitMiss_STK_Bond_6(int iaHitWin[3][3])
{
	int iHit = 0;
	int j = 1,k = 1;

	int X0 = iaHitWin[0][0];
	int X1 = iaHitWin[0][1];
	int X2 = iaHitWin[0][2];
	int X3 = iaHitWin[1][2];
	int X4 = iaHitWin[2][2];
	int X5 = iaHitWin[2][1];
	int X6 = iaHitWin[2][0];
	int X7 = iaHitWin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;

	//Pattern 1 || Pattern 2 || Pattern 3 || Pattern 4 == 1 for a hit
	if((iBval == 15) || (iBval == 30) || (iBval == 135) || (iBval == 195) || (iBval == 225) || (iBval == 240) || (iBval == 120) || (iBval == 60))
		iHit = 1;
	else
		iHit = 0;

	return iHit;
}

int HitMiss_STK_Bond_7(int iaHitWin[3][3])
{
	int iHit = 0;
	int j = 1,k = 1;
	int X0 = iaHitWin[0][0];
	int X1 = iaHitWin[0][1];
	int X2 = iaHitWin[0][2];
	int X3 = iaHitWin[1][2];
	int X4 = iaHitWin[2][2];
	int X5 = iaHitWin[2][1];
	int X6 = iaHitWin[2][0];
	int X7 = iaHitWin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;
	if((iBval == 31) || (iBval == 199) || (iBval == 241) || (iBval == 124))	
		iHit = 1;	
	else
		iHit = 0;

	return iHit;
}

int HitMiss_STK_Bond_8(int iaHitWin[3][3])
{
	int iHit = 0;
	int j = 1,k = 1;
	int X0 = iaHitWin[0][0];
	int X1 = iaHitWin[0][1];
	int X2 = iaHitWin[0][2];
	int X3 = iaHitWin[1][2];
	int X4 = iaHitWin[2][2];
	int X5 = iaHitWin[2][1];
	int X6 = iaHitWin[2][0];
	int X7 = iaHitWin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;
	if((iBval == 62) || (iBval == 143) || (iBval == 248) || (iBval == 227))	
		iHit = 1;	
	else
		iHit = 0;

	return iHit;
}

int HitMiss_STK_Bond_9(int iaHitWin[3][3])
{
	int iHit = 0;
	int j = 1,k = 1;
	
	int X0 = iaHitWin[0][0];
	int X1 = iaHitWin[0][1];
	int X2 = iaHitWin[0][2];
	int X3 = iaHitWin[1][2];
	int X4 = iaHitWin[2][2];
	int X5 = iaHitWin[2][1];
	int X6 = iaHitWin[2][0];
	int X7 = iaHitWin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;
	

	if((iBval == 63) || (iBval == 126) || (iBval == 207) || (iBval == 159) || (iBval == 231) || (iBval == 249) || (iBval == 252) || (iBval == 243))	
		iHit = 1;	
	else
		iHit = 0;

	return iHit;
}

int HitMiss_STK_Bond_10(int iaHitWin[3][3])
{
	int iHit = 0;
	int j = 1,k = 1;

	int X0 = iaHitWin[0][0];
	int X1 = iaHitWin[0][1];
	int X2 = iaHitWin[0][2];
	int X3 = iaHitWin[1][2];
	int X4 = iaHitWin[2][2];
	int X5 = iaHitWin[2][1];
	int X6 = iaHitWin[2][0];
	int X7 = iaHitWin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;
	
	if((iBval == 127) || (iBval == 223) || (iBval == 247) || (iBval == 253))	
		iHit = 1;	
	else
		iHit = 0;

	return iHit;
}

int HitMiss_K_Bond_11(int iaHitWin[3][3])
{
	int iHit = 0;
	int j = 1,k = 1;

	int X0 = iaHitWin[0][0];
	int X1 = iaHitWin[0][1];
	int X2 = iaHitWin[0][2];
	int X3 = iaHitWin[1][2];
	int X4 = iaHitWin[2][2];
	int X5 = iaHitWin[2][1];
	int X6 = iaHitWin[2][0];
	int X7 = iaHitWin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;

	if((iBval == 191) || (iBval == 239) || (iBval == 251) || (iBval == 254))
		iHit = 1;	
	else
		iHit = 0;

	return iHit;
}

int UnMask_ST_1(int iaMwin[3][3])
{
	int iHit = 0;
	int j = 1,k =1;

	int X0 = iaMwin[0][0];
	int X1 = iaMwin[0][1];
	int X2 = iaMwin[0][2];
	int X3 = iaMwin[1][2];
	int X4 = iaMwin[2][2];
	int X5 = iaMwin[2][1];
	int X6 = iaMwin[2][0];
	int X7 = iaMwin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;

	if(iBval == 4 || iBval == 1)
		iHit = 1;
	else 
		iHit = 0;

	return iHit;
}

int UnMask_ST_2(int iaMwin[3][3])
{
	int iHit = 0;
	int j = 1,k =1;

	int X0 = iaMwin[0][0];
	int X1 = iaMwin[0][1];
	int X2 = iaMwin[0][2];
	int X3 = iaMwin[1][2];
	int X4 = iaMwin[2][2];
	int X5 = iaMwin[2][1];
	int X6 = iaMwin[2][0];
	int X7 = iaMwin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;

	if(iBval == 32 || iBval == 8)
		iHit = 1;
	else 
		iHit = 0;

	return iHit;
}

int UnMask_ST_3(int iaMwin[3][3])
{
	int iHit = 0;
	int j = 1,k =1;

	int X0 = iaMwin[0][0];
	int X1 = iaMwin[0][1];
	int X2 = iaMwin[0][2];
	int X3 = iaMwin[1][2];
	int X4 = iaMwin[2][2];
	int X5 = iaMwin[2][1];
	int X6 = iaMwin[2][0];
	int X7 = iaMwin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;

	if(iBval == 12 || iBval == 6 || iBval == 3 || iBval == 129 || iBval == 192 || iBval == 96 || iBval == 48 || iBval == 24)
		iHit = 1;
	else 
		iHit = 0;

	return iHit;
}

int UnMask_ST_4(int iaMwin[3][3])
{
	int iHit = 0;
	int j = 1,k =1;

	int X0 = iaMwin[0][0];
	int X1 = iaMwin[0][1];
	int X2 = iaMwin[0][2];
	int X3 = iaMwin[1][2];
	int X4 = iaMwin[2][2];
	int X5 = iaMwin[2][1];
	int X6 = iaMwin[2][0];
	int X7 = iaMwin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;

	if(iBval == 134 || iBval == 11 || iBval == 26 || iBval == 44)
	{
		iHit = 1;		
	}
	else 
		iHit = 0;

	return iHit;
}

int UnMask_ST_5(int iaMwin[3][3])
{
	int iHit = 0;
	int j = 1,k =1;

	int iaHitVal_1[3];
	int iaHitVal_2[3];
	int iaHitVal_3[3];
	int iaHitVal_4[3];

	int X0 = iaMwin[0][0];
	int X1 = iaMwin[0][1];
	int X2 = iaMwin[0][2];
	int X3 = iaMwin[1][2];
	int X4 = iaMwin[2][2];
	int X5 = iaMwin[2][1];
	int X6 = iaMwin[2][0];
	int X7 = iaMwin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;
	int i = 0;
	for(int A = 0;A<2;A++)
		for(int B = 0;B<2;B++)
		{
			if(A == 0 && B == 0)
				continue;
			iaHitVal_1[i] = 0*1 + A*2 + 1*4 + B*8 + 0*16 + 0*32 + 1*64 + 0*128;
			iaHitVal_2[i] = 1*1 + B*2 + 0*4 + 0*8 + 1*16 + 0*32 + 0*64 + A*128;
			iaHitVal_3[i] = 0*1 + 0*2 + 1*4 + 0*8 + 0*16 + B*32 + 1*64 + A*128;
			iaHitVal_4[i] = 1*1 + 0*2 + 0*4 + B*8 + 1*16 + A*32 + 0*64 + 0*128;
			i++;
		}

	for(int i=0;i<3;i++)
	{
		if(iBval == iaHitVal_1[i] || iBval == iaHitVal_2[i] || iBval == iaHitVal_3[i] || iBval == iaHitVal_4[i])
		{
			iHit = 1;
			break;
		}
		else
			iHit = 0;
	}

	return iHit;
}

int UnMask_ST_6(int iaMwin[3][3])
{
	int iHit = 0;
	int j = 1,k =1;
	int iaHitVal[32];

	int X0 = iaMwin[0][0];
	int X1 = iaMwin[0][1];
	int X2 = iaMwin[0][2];
	int X3 = iaMwin[1][2];
	int X4 = iaMwin[2][2];
	int X5 = iaMwin[2][1];
	int X6 = iaMwin[2][0];
	int X7 = iaMwin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;

	for(int i = 0;i<32;i++)
	{
		iaHitVal[i] = 131 + i*4;
	}
	
	for(int i = 0;i<32;i++)
	{
		if(iBval == iaHitVal[i])
		{
			iHit = 1;
			break;
		}
		else
			iHit = 0;
	}

	return iHit;
}

int UnMask_ST_7(int iaMwin[3][3])
{
	int iHit = 0;
	int j = 1,k =1;

	int iaHitVal_1[4];
	int iaHitVal_2[4];
	int iaHitVal_3[4];
	int iaHitVal_4[4];
	int iaHitVal_5[4];
	int iaHitVal_6[4];
	int iaHitVal_7[4];
	int iaHitVal_8[4];

	int X0 = iaMwin[0][0];
	int X1 = iaMwin[0][1];
	int X2 = iaMwin[0][2];
	int X3 = iaMwin[1][2];
	int X4 = iaMwin[2][2];
	int X5 = iaMwin[2][1];
	int X6 = iaMwin[2][0];
	int X7 = iaMwin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;
	int i = 0;
	for(int D1 = 0;D1<2;D1++)
	{
		for(int D2 = 0;D2<2;D2++)
		{
			iaHitVal_1[i] = D2*1 + 1*2 + 0*4 + 1*8 + 0*16 + 0*32 + D1*64 + 1*128;
			iaHitVal_2[i] = 0*1 + 1*2 + D2*4 + 1*8 + D1*16 + 1*32 + 0*64 + 1*128;
			iaHitVal_3[i] = 0*1 + 1*2 + D2*4 + 1*8 + D1*16 + 0*32 + 0*64 + 1*128;
			iaHitVal_4[i] = D2*1 + 0*2 + 0*4 + 1*8 + 0*16 + 1*32 + D1*64 + 1*128;
			iaHitVal_5[i] = D2*1 + 1*2 + D1*4 + 0*8 + 0*16 + 1*32 + 0*64 + 1*128;
			iaHitVal_6[i] = 0*1 + 1*2 + 0*4 + 0*8 + D2*16 + 1*32 + D1*64 + 1*128;
			iaHitVal_7[i] = 0*1 + 1*2 + 0*4 + 1*8 + D2*16 + 1*32 + D1*64 + 0*128;
			iaHitVal_8[i] = D2*1 + 1*2 + D1*4 + 1*8 + 0*16 + 1*32 + 0*64 + 0*128;
		}
	}

	for(int i = 0;i<4;i++)
	{
		if(iBval == iaHitVal_1[i])
		{
			iHit = 1;
			break;
		}
		else if(iBval == iaHitVal_2[i])
		{
			iHit = 1;
			break;
		}
		else if(iBval == iaHitVal_3[i])
		{
			iHit = 1;
			break;
		}
		else if(iBval == iaHitVal_4[i])
		{
			iHit = 1;
			break;
		}
		else if(iBval == iaHitVal_5[i])
		{
			iHit = 1;
			break;
		}
		else if(iBval == iaHitVal_6[i])
		{
			iHit = 1;
			break;
		}
		else if(iBval == iaHitVal_7[i])
		{
			iHit = 1;
			break;
		}
		else if(iBval == iaHitVal_8[i])
		{
			iHit = 1;
			break;
		}
		else
			iHit = 0;
	}

	return iHit;
}

int UnMask_ST_8(int iaMwin[3][3])
{
	int iHit = 0;
	int j = 1,k =1;
	
	int iaHitVal_1[56];
	int iaHitVal_2[56];
	int iaHitVal_3[56];
	int iaHitVal_4[56];

	int X0 = iaMwin[0][0];
	int X1 = iaMwin[0][1];
	int X2 = iaMwin[0][2];
	int X3 = iaMwin[1][2];
	int X4 = iaMwin[2][2];
	int X5 = iaMwin[2][1];
	int X6 = iaMwin[2][0];
	int X7 = iaMwin[1][0];

	int i = 0;
	for(int D1 = 0;D1<2;D1++)
		for(int A = 0;A<2;A++)
			for(int B = 0;B<2;B++)
				for(int C = 0;C<2;C++)
					for(int D2 = 0;D2<2;D2++)
						for(int D3 = 0;D3<2;D3++)
						{
							if(A == 0 && B == 0 && C == 0)
								continue;
							iaHitVal_1[i] = 1*1 + D3*2 + 1*4 + D2*8 + C*16 + B*32 + A*64 + D1*128;
							iaHitVal_2[i] = 1*1 + D3*2 + C*4 + B*8 + A*16 + D2*32 + 1*64 + D1*128;
							iaHitVal_3[i] = C*1 + B*2 + A*4 + D3*8 + 1*16 + D2*32 + 1*64 + D1*128;
							iaHitVal_4[i] = A*1 + D3*2 + 1*4 + D2*8 + 1*16 + D1*32 + C*64 + B*128;
							i++;
						}


	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;

	for(int j = 0;j<64;j++)
	{
		if(iBval == iaHitVal_1[i] || iBval == iaHitVal_2[i] || iBval == iaHitVal_3[i] || iBval == iaHitVal_4[i])
		{
			iHit =1;
			break;
		}
		else
			iHit = 0;
	}

	return iHit;
}

int UnMask_ST_9(int iaMwin[3][3])
{
	int iHit = 0;
	int j = 1,k =1;

	int iaHitVal_1[4];
	int iaHitVal_2[4];
	int iaHitVal_3[4];
	int iaHitVal_4[4];

	int X0 = iaMwin[0][0];
	int X1 = iaMwin[0][1];
	int X2 = iaMwin[0][2];
	int X3 = iaMwin[1][2];
	int X4 = iaMwin[2][2];
	int X5 = iaMwin[2][1];
	int X6 = iaMwin[2][0];
	int X7 = iaMwin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;
	
	int i = 0;
	for(int D1 = 0;D1 <2 ;D1++)
		for(int D2 = 0;D2<2;D2++)
		{
			iaHitVal_1[i] = D2*1 + 1*2 + 0*4 + 1*8 + D1*16 + 0*32 + 1*64 + 0*128;
			iaHitVal_2[i] = 0*1 + 1*2 + D2*4 + 0*8 + 1*16 + 0*32 + D1*64 + 1*128;
			iaHitVal_3[i] = D2*1 + 0*2 + 1*4 + 0*8 + D1*16 + 1*32 + 0*64 + 1*128;
			iaHitVal_4[i] = 1*1 + 0*2 + D2*4 + 1*8 + 0*16 + 1*32 + D1*64 + 0*128;
			i++;
		}

	for(int j = 0;j<4;j++)
	{
		if(iBval == iaHitVal_1[i] || iBval == iaHitVal_2[i] || iBval == iaHitVal_3[i] || iBval == iaHitVal_4[i])
		{
			iHit = 1;
			break;
		}
		else
			iHit = 0;
	}

	return iHit;
}

int UnMask_K_1(int iaMwin[3][3])
{
	int iHit = 0;
	int j = 1,k =1;

	int iaHitVal_1[4];
	int iaHitVal_2[4];
	int iaHitVal_3[4];
	int iaHitVal_4[4];

	int X0 = iaMwin[0][0];
	int X1 = iaMwin[0][1];
	int X2 = iaMwin[0][2];
	int X3 = iaMwin[1][2];
	int X4 = iaMwin[2][2];
	int X5 = iaMwin[2][1];
	int X6 = iaMwin[2][0];
	int X7 = iaMwin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;
	
	if(iBval == 16 || iBval == 64 || iBval == 4 || iBval == 1)
		iHit = 1;
	else
		iHit = 0;

	return iHit;
}

int UnMask_K_2(int iaMwin[3][3])
{
	int iHit = 0;
	int j = 1,k =1;

	int iaHitVal_1[4];
	int iaHitVal_2[4];
	int iaHitVal_3[4];
	int iaHitVal_4[4];

	int X0 = iaMwin[0][0];
	int X1 = iaMwin[0][1];
	int X2 = iaMwin[0][2];
	int X3 = iaMwin[1][2];
	int X4 = iaMwin[2][2];
	int X5 = iaMwin[2][1];
	int X6 = iaMwin[2][0];
	int X7 = iaMwin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;
	
	if(iBval == 32 || iBval == 8 || iBval == 128 || iBval == 2)
		iHit = 1;
	else
		iHit = 0;

	return iHit;
}

int UnMask_K_3(int iaMwin[3][3])
{
	int iHit = 0;
	int j = 1,k =1;

	int iaHitVal_1[4];
	int iaHitVal_2[4];
	int iaHitVal_3[4];
	int iaHitVal_4[4];

	int X0 = iaMwin[0][0];
	int X1 = iaMwin[0][1];
	int X2 = iaMwin[0][2];
	int X3 = iaMwin[1][2];
	int X4 = iaMwin[2][2];
	int X5 = iaMwin[2][1];
	int X6 = iaMwin[2][0];
	int X7 = iaMwin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;
	
	if(iBval == 10 || iBval == 130 || iBval == 40 || iBval == 160)
		iHit = 1;
	else
		iHit = 0;

	return iHit;
}

int UnMask_K_4(int iaMwin[3][3])
{
	int iHit = 0;
	int j = 1,k =1;

	int iaHitVal_1[4];
	int iaHitVal_2[4];
	int iaHitVal_3[4];
	int iaHitVal_4[4];

	int X0 = iaMwin[0][0];
	int X1 = iaMwin[0][1];
	int X2 = iaMwin[0][2];
	int X3 = iaMwin[1][2];
	int X4 = iaMwin[2][2];
	int X5 = iaMwin[2][1];
	int X6 = iaMwin[2][0];
	int X7 = iaMwin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;
	
	for(int D1 = 0;D1<2;D1++)
	{
		for(int D2 = 0;D2<2;D2++)
		{
			for(int D3 = 0;D3<2;D3++)
			{
				for(int D4 = 0;D4<2;D4++)
				{
					for(int D5 = 0;D5<2;D5++)
					{
						if(iBval == 1*1 + 1*2 + D5*4 + D4*8 + D3*16 + D2*32 + D1*64 + 1*128 || iBval == D5*1 + D4*2 + D3*4 + 1*8 + 1*16 + 1*32 + D2*64 + D1*128)
						{
							iHit = 1;
							return iHit;
						}
					}
				}
			}
		}
	}

	return iHit;
}

int UnMask_K_5(int iaMwin[3][3])
{
	int iHit = 0;
	int j = 1,k =1;

	int iaHitVal_1[4];
	int iaHitVal_2[4];
	int iaHitVal_3[4];
	int iaHitVal_4[4];

	int X0 = iaMwin[0][0];
	int X1 = iaMwin[0][1];
	int X2 = iaMwin[0][2];
	int X3 = iaMwin[1][2];
	int X4 = iaMwin[2][2];
	int X5 = iaMwin[2][1];
	int X6 = iaMwin[2][0];
	int X7 = iaMwin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;
	
	for(int D1 = 0;D1<2;D1++)
	{
		for(int D2 = 0;D2<2;D2++)
		{
			for(int D3 = 0;D3<2;D3++)
			{
				for(int D4 = 0;D4<2;D4++)
				{
					for(int D5 = 0;D5<2;D5++)
					{
						if(iBval == D5*1 + 1*2 + D4*4 + 1*8 + D3*16 + D2*32 + D1*64 + 1*128)
						{
							iHit = 1;
							return iHit;
						}
						else if(iBval == D5*1 + 1*2 + D4*4 + D3*8 + D2*16 + 1*32 + D1*64 + 1*128)
						{
							iHit = 1;
							return iHit;
						}
						else if(iBval == D5*1 + D4*2 + D3*4 + 1*8 + D2*16 + 1*32 + D1*64 + 1*128)
						{
							iHit = 1;
							return iHit;
						}
						else if(iBval == D5*1 + 1*2 + D4*4 + 1*8 + D3*16 + 1*32 + D2*64 + D1*128)
						{
							iHit = 1;
							return iHit;
						}
					}
				}
			}
		}
	}

	return iHit;
}

int UnMask_K_6(int iaMwin[3][3])
{
	int iHit = 0;
	int j = 1,k =1;

	int iaHitVal_1[4];
	int iaHitVal_2[4];
	int iaHitVal_3[4];
	int iaHitVal_4[4];

	int X0 = iaMwin[0][0];
	int X1 = iaMwin[0][1];
	int X2 = iaMwin[0][2];
	int X3 = iaMwin[1][2];
	int X4 = iaMwin[2][2];
	int X5 = iaMwin[2][1];
	int X6 = iaMwin[2][0];
	int X7 = iaMwin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;
	
	for(int D1 = 0;D1<2;D1++)
	{
		for(int D2 = 0;D2<2;D2++)
		{
			for(int D3 = 0;D3<2;D3++)
			{
				for(int A = 0;A<2;A++)
				{
					for(int B = 0;B<2;B++)
					{
						for(int C = 0;C<2;C++)
						{
							if(A == 0 && B == 0 && C == 0)
								continue;

							if(iBval == 1*1 + D3*2 + 1*4 + D2*8 + C*16 + B*32 + A*64 + D1*128)
							{
								iHit = 1;
								return iHit;
							}
							else if(iBval == 1*1 + D3*2 + C*4 + B*8 + A*16 + D2*32 + 1*64 + D1*128)
							{
								iHit = 1;
								return iHit;
							}
							else if(iBval == C*1 + B*2 + A*4 + D3*8 + 1*16 + D2*32 + 1*64 + D1*128)
							{
								iHit = 1;
								return iHit;
							}
							else if(iBval == A*1 + D3*2 + 1*4 + D2*8 + 1*16 + D1*32 + C*64 + B*128)
							{
								iHit = 1;
								return iHit;
							}
						}
					}
				}
			}
		}
	}

	return iHit;
}

int UnMask_K_7(int iaMwin[3][3])
{
	int iHit = 0;
	int j = 1,k =1;

	int iaHitVal_1[4];
	int iaHitVal_2[4];
	int iaHitVal_3[4];
	int iaHitVal_4[4];

	int X0 = iaMwin[0][0];
	int X1 = iaMwin[0][1];
	int X2 = iaMwin[0][2];
	int X3 = iaMwin[1][2];
	int X4 = iaMwin[2][2];
	int X5 = iaMwin[2][1];
	int X6 = iaMwin[2][0];
	int X7 = iaMwin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;
	
	for(int D1 = 0;D1<2;D1++)
	{
		for(int D2 = 0;D2<2;D2++)
		{			
			if(iBval == D2*1 + 1*2 + 0*4 + 1*8 + D1*16 + 0*32 + 1*64 + 0*128)
			{
				iHit = 1;
				return iHit;
			}
			else if(iBval == 0*1 + 1*2 + D2*4 + 0*8 + 1*16 + 0*32 + D1*64 + 1*128)
			{
				iHit = 1;
				return iHit;
			}
			else if(iBval == D2*1 + 0*2 + 1*4 +0*8 + D1*16 + 1*32 + 0*64 + 1*128)
			{
				iHit = 1;
				return iHit;
			}
			else if(iBval == 1*1 + 0*2 + D2*4 + 1*8 + 0*16 + 1*32 + D1*64 + 0*128)
			{
				iHit = 1;
				return iHit;
			}		
		}
	}

	return iHit;
}

int SecondStageCount(int iaMwin[3][3])
{
	int iaMask[332];
	int iCount = 0;
	int iHit = 0;
	
	int X0 = iaMwin[0][0];
	int X1 = iaMwin[0][1];
	int X2 = iaMwin[0][2];
	int X3 = iaMwin[1][2];
	int X4 = iaMwin[2][2];
	int X5 = iaMwin[2][1];
	int X6 = iaMwin[2][0];
	int X7 = iaMwin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;

	//Stage 1
	iaMask[iCount++] = 4;
	iaMask[iCount++] = 1;

	//Stage 2
	iaMask[iCount++] = 32;
	iaMask[iCount++] = 8;

	//Stage 3
	iaMask[iCount++] = 12;
	iaMask[iCount++] = 6;
	iaMask[iCount++] = 3;
	iaMask[iCount++] = 129;
	iaMask[iCount++] = 192;
	iaMask[iCount++] = 96;
	iaMask[iCount++] = 48;
	iaMask[iCount++] = 24;

	//Stage 4
	iaMask[iCount++] = 134;
	iaMask[iCount++] = 11;
	iaMask[iCount++] = 26;
	iaMask[iCount++] = 44;

	//stage 5
	int i = 0;
	for(int A = 0;A<2;A++)
		for(int B = 0;B<2;B++)
		{
			if(A == 0 && B == 0)
				continue;			
			iaMask[iCount++] = 0*1 + A*2 + 1*4 + B*8 + 0*16 + 0*32 + 1*64 + 0*128;
			iaMask[iCount++] = 1*1 + B*2 + 0*4 + 0*8 + 1*16 + 0*32 + 0*64 + A*128;
			iaMask[iCount++] = 0*1 + 0*2 + 1*4 + 0*8 + 0*16 + B*32 + 1*64 + A*128;
			iaMask[iCount++] = 1*1 + 0*2 + 0*4 + B*8 + 1*16 + A*32 + 0*64 + 0*128;			
		}
		//iCount += i*4;

	//Stage 6
	for(int i = 0;i<32;i++)
	{
		iaMask[iCount++] = 131 + i*4;
	}
	//iCount += 32;

	//Stage 7
	for(int D1 = 0;D1<2;D1++)
	{
		for(int D2 = 0;D2<2;D2++)
		{
			iaMask[iCount++] = D2*1 + 1*2 + 0*4 + 1*8 + 0*16 + 0*32 + D1*64 + 1*128;
			iaMask[iCount++] = 0*1 + 1*2 + D2*4 + 1*8 + D1*16 + 1*32 + 0*64 + 1*128;
			iaMask[iCount++] = 0*1 + 1*2 + D2*4 + 1*8 + D1*16 + 0*32 + 0*64 + 1*128;
			iaMask[iCount++] = D2*1 + 0*2 + 0*4 + 1*8 + 0*16 + 1*32 + D1*64 + 1*128;
			iaMask[iCount++] = D2*1 + 1*2 + D1*4 + 0*8 + 0*16 + 1*32 + 0*64 + 1*128;
			iaMask[iCount++] = 0*1 + 1*2 + 0*4 + 0*8 + D2*16 + 1*32 + D1*64 + 1*128;
			iaMask[iCount++] = 0*1 + 1*2 + 0*4 + 1*8 + D2*16 + 1*32 + D1*64 + 0*128;
			iaMask[iCount++] = D2*1 + 1*2 + D1*4 + 1*8 + 0*16 + 1*32 + 0*64 + 0*128;
		}
	}
	//iCount += 4*8;

	//Stage 8
	i = 0;
	for(int D1 = 0;D1<2;D1++)
		for(int A = 0;A<2;A++)
			for(int B = 0;B<2;B++)
				for(int C = 0;C<2;C++)
					for(int D2 = 0;D2<2;D2++)
						for(int D3 = 0;D3<2;D3++)
						{
							if(A == 0 && B == 0 && C == 0)
								continue;	
							iaMask[iCount++] = 1*1 + D3*2 + 1*4 + D2*8 + C*16 + B*32 + A*64 + D1*128;
							iaMask[iCount++] = 1*1 + D3*2 + C*4 + B*8 + A*16 + D2*32 + 1*64 + D1*128;
							iaMask[iCount++] = C*1 + B*2 + A*4 + D3*8 + 1*16 + D2*32 + 1*64 + D1*128;
							iaMask[iCount++] = A*1 + D3*2 + 1*4 + D2*8 + 1*16 + D1*32 + C*64 + B*128;
							//i++;
						}
	//iCount += i*4;

	//Stage 9
	i = 0;
	for(int D1 = 0;D1 <2 ;D1++)
		for(int D2 = 0;D2<2;D2++)
		{
			iaMask[iCount++] = D2*1 + 1*2 + 0*4 + 1*8 + D1*16 + 0*32 + 1*64 + 0*128;
			iaMask[iCount++] = 0*1 + 1*2 + D2*4 + 0*8 + 1*16 + 0*32 + D1*64 + 1*128;
			iaMask[iCount++] = D2*1 + 0*2 + 1*4 + 0*8 + D1*16 + 1*32 + 0*64 + 1*128;
			iaMask[iCount++] = 1*1 + 0*2 + D2*4 + 1*8 + 0*16 + 1*32 + D1*64 + 0*128;
			//i++;
		}
	//iCount += i*4;

	//cout<<"i = "<<iCount;
	//getch();
	
	for(int i = 0;i<332;i++)
	{
		if(iBval == iaMask[i])
		{
			iHit = 1;
			break;
		}
		else
			iHit = 0;
	}

	return iHit;
}

int OutLierFilter(int iaHitWin[3][3])
{
	int iHit = 0;
	int j = 1,k = 1;

	int X0 = iaHitWin[0][0];
	int X1 = iaHitWin[0][1];
	int X2 = iaHitWin[0][2];
	int X3 = iaHitWin[1][2];
	int X4 = iaHitWin[2][2];
	int X5 = iaHitWin[2][1];
	int X6 = iaHitWin[2][0];
	int X7 = iaHitWin[1][0];

	int iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;

	//Pattern 1 || Pattern 2 || Pattern 3 || Pattern 4 == 1 for a hit
	if((iBval == 0))
	{
		iHit = 1;
	}
	else
		iHit = 0;

	return iHit;
}

int Shrink(int **piaF, int iRows, int iCols)
{
	int iaHitWin[3][3];
	int **iaM = new int*[iRows];
	int **iaF = piaF;
	int **iaFEx = new int*[iRows +2];
	int **iaMEx = new int*[iRows + 2];
	int iHitCount = 1;

	//M & F Extended Matrix
	for(int i = 0;i<iRows + 2;i++)
	{
		iaFEx[i] = new int[iCols + 2];
		iaMEx[i] = new int[iCols + 2];
	}

	//M & F Matrix
	for(int i = 0;i<iRows;i++)
	{
		iaM[i] = new int[iCols];
	}

	//Fill M Matrix With Zeros
	for(int i = 0;i<iRows;i++)
		for(int j = 0;j<iCols;j++)
		{
			iaM[i][j] = 0;
		}	

	for(int jj = 0;jj<iRows + 2;jj++)
	{
		for(int kk = 0;kk<iCols + 2;kk++)
		{
			iaFEx[jj][kk] = 0;
			iaMEx[jj][kk] = 0;
		}
	}		
	//Extend F to FEx		
	for(int jj = 1;jj<iRows +1;jj++)
	{
		for(int kk = 1;kk<iCols + 1;kk++)
		{
			iaFEx[jj][kk] = iaF[jj-1][kk-1];				
		}
	}

	int iSize = 0;
	while(1)
	{
		//Reset MEx to zero
		for(int jj = 0;jj<iRows + 2;jj++)
		{
			for(int kk = 0;kk<iCols + 2;kk++)
			{				
				iaMEx[jj][kk] = 0;
			}
		}

		//First Stage
		for(int j = 1;j<iRows+1;j++)
		{
			for(int k = 1;k<iCols+1;k++)
			{				
				if(iaFEx[j][k] == 0)
					continue;
				//Calculate Bond
				int iBond = 0;
				iBond = iaFEx[j-1][k-1] + iaFEx[j+1][k-1] + iaFEx[j+1][k+1] + iaFEx[j-1][k+1];
				iBond += 2*(iaFEx[j][k-1] + iaFEx[j][k+1] + iaFEx[j-1][k] + iaFEx[j+1][k]);
				
				//Get The Surrounding Pixels
				for(int l = 0;l<3;l++)
				{
					for(int m = 0;m<3;m++)
					{
						iaHitWin[l][m] = iaFEx[j+(l-1)][k + (m-1)];
					}
				}

				int iHit = 0;
				//Select Mask Pattern based on bond
				switch(iBond)
				{
					case 1:			
						iHit = HitMiss_S_Bond_1(iaHitWin);				
					break;

					case 2:				
						iHit = HitMiss_S_Bond_2(iaHitWin);				
					break;

					case 3:				
						iHit = HitMiss_S_Bond_3(iaHitWin);				
					break;

					case 4:
						iHit = HitMiss_STK_Bond_4(iaHitWin);
					break;

					case 5:
						iHit = HitMiss_ST_Bond_5(iaHitWin);
					break;

					case 6:
						if(HitMiss_ST_Bond_6(iaHitWin) || HitMiss_STK_Bond_6(iaHitWin))
							iHit = 1;
						else 
							iHit = 0;					
					break;

					case 7:
						iHit = HitMiss_STK_Bond_7(iaHitWin);
					break;

					case 8:
						iHit = HitMiss_STK_Bond_8(iaHitWin);
					break;

					case 9:
						iHit = HitMiss_STK_Bond_9(iaHitWin);
					break;

					case 10:
						iHit = HitMiss_STK_Bond_10(iaHitWin);
					break;
				}
				/////////////////

				//If Hit Selecte Candidate in MEx(matrix);
				if(iHit)
				{					
					iaMEx[j][k] = 1;		
				}
				else
				{
					iaMEx[j][k] = 0;
				}
			}
		}
		//First Stage Complete						
		
		//MEx count
		int iCount = 0;
		//Find Hit Count
		for(int j = 1;j<iRows+1;j++)
		{
			for(int k = 1;k<iCols+1;k++)
			{
				if(iaMEx[j][k] == 1)
					iCount++;
			}
		}
		iHitCount = iCount;
		if(iHitCount == 0)
			break;

		//Second Stage	
		for(int j = 1;j<iRows+1;j++)
		{
			for(int k = 1;k<iCols+1;k++)
			{				
				if(iaMEx[j][k] == 0)
					continue;
				//Get The Surrounding Pixels
				for(int l = 0;l<3;l++)
				{
					for(int m = 0;m<3;m++)
					{
						iaHitWin[l][m] = iaMEx[j + (l-1)][k + (m-1)];
					}
				}

				int iHit = 0;//SecondStageCount(iaHitWin);

				iHit += UnMask_ST_1(iaHitWin);				
				iHit += UnMask_ST_2(iaHitWin);
				iHit += UnMask_ST_3(iaHitWin);
				iHit += UnMask_ST_4(iaHitWin);
				iHit += UnMask_ST_5(iaHitWin);
				iHit += UnMask_ST_6(iaHitWin);
				iHit += UnMask_ST_7(iaHitWin);
				iHit += UnMask_ST_8(iaHitWin);
				iHit += UnMask_ST_9(iaHitWin);
				
				if(iHit != 0)
				{					
					iaMEx[j][k] = 0;					
				}						
			}
		}		
		//Find Count of candidates
		int iMCount = 0;
		for(int j = 1;j<iRows+1;j++)
		{
			for(int k = 1;k<iCols+1;k++)
			{
				if(iaMEx[j][k] == 1)
					iMCount++;
			}
		}		
		iSize++;

		//Delete 1s in the binary
		for(int j = 1;j<iRows+1;j++)
		{
			for(int k = 1;k<iCols+1;k++)
			{
				if(iaMEx[j][k] == 1)
					iaFEx[j][k] = 0;
			}
		}		
		//Second Stage Complete
	}

	//Count Objects
	int iCount = 0;
	for(int i = 1;i<iRows+1;i++)
	{
		for(int j = 1;j<iCols+1;j++)
		{
			iaF[i-1][j-1] = iaFEx[i][j];

			if(iaFEx[i][j] == 1)
				iCount++;
		}
	}

	return iCount;
}

void Thin(int **piaF, int iRows, int iCols)
{
}

void Skeletonize(int **piaF, int iRows, int iCols)
{
	//Skeletonize the Image
	cout<<"\n*Skeltonizing...";
	int **iaM = new int*[iRows];
	int **iaFEx = new int*[iRows +2];
	int **iaMEx = new int*[iRows + 2];
	int iHitCount = 1;
	int iaHitWin[3][3];

	//M Extended Matrix
	for(int i = 0;i<iRows + 2;i++)
	{
		iaFEx[i] = new int[iCols + 2];
		iaMEx[i] = new int[iCols + 2];
	}

	//M & G Matrix
	for(int i = 0;i<iRows;i++)
	{
		iaM[i] = new int[iCols];
	}

	//Fill M Matrix With Zeros
	for(int i = 0;i<iRows;i++)
		for(int j = 0;j<iCols;j++)
		{
			iaM[i][j] = 0;
		}	

	for(int jj = 0;jj<iRows + 2;jj++)
	{
		for(int kk = 0;kk<iCols + 2;kk++)
		{
			iaFEx[jj][kk] = 0;
			iaMEx[jj][kk] = 0;
		}
	}		
	//Extend F to FEx		
	for(int jj = 1;jj<iRows +1;jj++)
	{
		for(int kk = 1;kk<iCols + 1;kk++)
		{
			iaFEx[jj][kk] = piaF[jj-1][kk-1];				
		}
	}

	int iSize = 0;
	while(1)
	{
		//Reset MEx to zero
		for(int jj = 0;jj<iRows + 2;jj++)
		{
			for(int kk = 0;kk<iCols + 2;kk++)
			{				
				iaMEx[jj][kk] = 0;
			}
		}

		//First Stage
		for(int j = 1;j<iRows+1;j++)
		{
			for(int k = 1;k<iCols+1;k++)
			{				
				if(iaFEx[j][k] == 0)
					continue;
				//Calculate Bond
				int iBond = 0;
				iBond = iaFEx[j-1][k-1] + iaFEx[j+1][k-1] + iaFEx[j+1][k+1] + iaFEx[j-1][k+1];
				iBond += 2*(iaFEx[j][k-1] + iaFEx[j][k+1] + iaFEx[j-1][k] + iaFEx[j+1][k]);
				
				//Get The Surrounding Pixels
				for(int l = 0;l<3;l++)
				{
					for(int m = 0;m<3;m++)
					{
						iaHitWin[l][m] = iaFEx[j+(l-1)][k + (m-1)];
					}
				}

				int iHit = 0;
				//Select Mask Pattern based on bond
				switch(iBond)
				{					
					case 4:
						//
						if(HitMiss_STK_Bond_4(iaHitWin) || HitMiss_TK_Bond_4(iaHitWin))
							iHit = 1;
						else
							iHit = 0;
					break;

					case 6:
						if(HitMiss_STK_Bond_6(iaHitWin))
							iHit = 1;
						else 
							iHit = 0;					
					break;

					case 7:
						iHit = HitMiss_STK_Bond_7(iaHitWin);
					break;

					case 8:
						iHit = HitMiss_STK_Bond_8(iaHitWin);
					break;

					case 9:
						iHit = HitMiss_STK_Bond_9(iaHitWin);
					break;

					case 10:
						iHit = HitMiss_STK_Bond_10(iaHitWin);
					break;

					case 11:
						iHit = HitMiss_K_Bond_11(iaHitWin);
					break;
				}
				/////////////////

				//If Hit Selecte Candidate in MEx(matrix);
				if(iHit)
				{					
					iaMEx[j][k] = 1;		
				}
				else
				{
					iaMEx[j][k] = 0;
				}
			}
		}
		//First Stage Complete						
		
		//MEx count
		int iCount = 0;
		//Find Hit Count
		for(int j = 1;j<iRows+1;j++)
		{
			for(int k = 1;k<iCols+1;k++)
			{
				if(iaMEx[j][k] == 1)
					iCount++;
			}
		}
		iHitCount = iCount;
		if(iHitCount == 0)
			break;

		//Second Stage	
		for(int j = 1;j<iRows+1;j++)
		{
			for(int k = 1;k<iCols+1;k++)
			{				
				if(iaMEx[j][k] == 0)
					continue;
				//Get The Surrounding Pixels
				for(int l = 0;l<3;l++)
				{
					for(int m = 0;m<3;m++)
					{
						iaHitWin[l][m] = iaMEx[j + (l-1)][k + (m-1)];
					}
				}

				int iHit = 0;//SecondStageCount(iaHitWin);

				iHit += UnMask_K_1(iaHitWin);				
				iHit += UnMask_K_2(iaHitWin);
				iHit += UnMask_K_3(iaHitWin);
				iHit += UnMask_K_4(iaHitWin);
				iHit += UnMask_K_5(iaHitWin);
				iHit += UnMask_K_6(iaHitWin);
				iHit += UnMask_K_7(iaHitWin);				
				
				if(iHit != 0)
				{					
					iaMEx[j][k] = 0;					
				}						
			}
		}		
		//Find Count of candidates
		int iMCount = 0;
		for(int j = 1;j<iRows+1;j++)
		{
			for(int k = 1;k<iCols+1;k++)
			{
				if(iaMEx[j][k] == 1)
					iMCount++;
			}
		}

		if(iMCount == 0)
			break;

		//Delete 1s in the binary
		for(int j = 1;j<iRows+1;j++)
		{
			for(int k = 1;k<iCols+1;k++)
			{
				if(iaMEx[j][k] == 1)
					iaFEx[j][k] = 0;
			}
		}		
		//Second Stage COmplete
	}

	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			piaF[i][j] = iaFEx[i+1][j+1];
		}
	}
}

void Dilate(int **piaF, int iRows, int iCols)
{
	int iaWin[3][3];
	int iBval = 0;

	for(int i = 1;i<iRows-1;i++)
	{
		for(int j = 1;j<iCols-1;j++)
		{
			if(piaF[i][j] == 1)
				continue;

			for(int l = 0;l<3;l++)
			{
				for(int m = 0;m<3;m++)
				{
					iaWin[l][m] = piaF[i + (l-1)][j + (m-1)];
				}
			}

			int X0 = iaWin[0][0];
			int X1 = iaWin[0][1];
			int X2 = iaWin[0][2];
			int X3 = iaWin[1][2];
			int X4 = iaWin[2][2];
			int X5 = iaWin[2][1];
			int X6 = iaWin[2][0];
			int X7 = iaWin[1][0];
		
			iBval = X0*1 + X1*2 + X2*4 + X3*8 + X4*16 + X5*32 + X6*64 + X7*128;

			if(iBval == 255 || iBval == 223 || iBval == 247 || iBval == 127 || iBval == 253)
				piaF[i][j] = 1;			
			else if(iBval == 251 || iBval == 254 || iBval == 191 || iBval == 239)
				piaF[i][j] = 1;
		}
	}

}

int CheckTurn(int iaWin[5][5])
{
	long unsigned iHit = 0;
	long unsigned iBval = 0;
	long unsigned iVal_1 = 0;
	long unsigned iVal_2 = 0;
	long unsigned iVal_3 = 0;
	long unsigned iVal_4 = 0;

	int iaTurnLeft[5][5] = { {0,0,0,0,0},
										{0,0,0,0,0},
										{0,0,1,0,0},
										{0,1,0,0,0},
										{1,0,0,0,0} };

	int iaTurnRight[5][5] = { {0,0,0,0,0},
										  {0,0,0,0,0},
										  {0,0,1,0,0},
										  {0,0,0,1,0},
										  {0,0,0,0,1} };

	int iaTurnUpLeft[5][5] = { {1,0,0,0,0},
											{0,1,0,0,0},
											{0,0,1,0,0},
											{0,0,0,0,0},
											{0,0,0,0,0} };

	int iaTurnUpRight[5][5] = { {0,0,0,0,1},
											  {0,0,0,1,0},
										      {0,0,1,0,0},
										      {0,0,0,0,0},
										      {0,0,0,0,0} };

	long unsigned iWeight = 1;
	
	for(int i = 0;i<5;i++)
	{
		for(int j = 0;j<5;j++)
		{
			//if(i == 2 && j == 2)
			//	continue;

			iBval += iaWin[i][j]*iWeight;

			iVal_1 += iaTurnLeft[i][j]*iWeight;
			iVal_2 += iaTurnRight[i][j]*iWeight;
			iVal_3 += iaTurnUpLeft[i][j]*iWeight;
			iVal_4 += iaTurnUpRight[i][j]*iWeight;

			iWeight *= 2;
		}
	}

	if(iBval == iVal_1 || iBval == iVal_4 || iBval == iVal_3 || iBval == iVal_2)
	{
		iHit = 1;
		return iHit; 
	}
	else
		iHit = 0;

	return iHit;
}