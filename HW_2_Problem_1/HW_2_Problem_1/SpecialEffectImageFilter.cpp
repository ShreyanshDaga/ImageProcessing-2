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

#include"HW_2_Q_1.h"
#include"Pixel.h"
#include"Image.h"
#include"Histogram.h"

using namespace std;

//Conversio to Gray Scale
void Problem_1_A()
{
	int iRows = 256,iCols = 256;
	int iColorFormat = 3;
	Image imInput[3],imOutput[3];
	string szNames[3] = {"naruto.raw","harrypotter.raw","lena_noisy.raw"};


	for(int i = 0;i<3;i++)
	{
		imInput[i].SetImageName(szNames[i]);
		imInput[i].SetImageRows(iRows);
		imInput[i].SetImageCols(iCols);
		imInput[i].SetImageColorFormat(iColorFormat);
		imInput[i].AllocImageMem();

		imInput[i].ReadImageData();
	}
	
	for(int i = 0;i<3;i++)
	{
		imOutput[i] = *imInput[i].Convert2Gray();
		imOutput[i].WriteImageData();
	}

	cout<<"\nPress 'C' to continue!";
	getch();
}

//First Order Derivative Method
void Problem_1_B_a()
{
	//Variables
	int iRows = 256,iCols = 256;
	int iColorFormat = 3;
	Image imInput[3],imGrayScale[3],imExtended[3],imGradientMap[3],imOutput[3];
	string szNames[3] = {"naruto.raw","harrypotter.raw","lena_noisy.raw"};

	//Read Input Imagess
	for(int i = 0;i<3;i++)
	{
		imInput[i].SetImageName(szNames[i]);
		imInput[i].SetImageRows(iRows);
		imInput[i].SetImageCols(iCols);
		imInput[i].SetImageColorFormat(iColorFormat);
		imInput[i].AllocImageMem();
		imInput[i].ReadImageData();
	}

	//Get Gray Level Images
	for(int i = 0;i<3;i++)
	{
		imGrayScale[i] = *imInput[i].Convert2Gray();
		imGrayScale[i].WriteImageData();
		imExtended[i] = *imGrayScale[i].ImageExtend(1);
		imExtended[i].WriteImageData();
	}

	//Denoise Lena
	DeNoiseGaussian(&imExtended[2],1);

	//1st Order Diff Image
	cout<<"\n1st Order Differential method";
	Image imGrad[3];
	string szGradient[3] = {"Naruto_Pencil.raw","Harry_Pencil.raw","Lena_Pencil.raw"};

	for(int k = 0;k<3;k++)
	{
		imGrad[k].SetImageName(szGradient[k]);
		imGrad[k].SetImageRows(iRows);
		imGrad[k].SetImageCols(iCols);
		imGrad[k].SetImageColorFormat(1);
		imGrad[k].AllocImageMem();
		
		for(int i = 0;i<iRows;i++)
		{
			for(int j = 0;j<iCols;j++)
			{
				Pixel *pxCurr,*pxNext;
				Pixel pxP;
				int cGr,cGc;
				
				pxCurr = imExtended[k].GetPixel(i+1,j);
				pxNext = imExtended[k].GetPixel(i+1,j+2);
				cGr =  (int)pxNext->iBW - (int)pxCurr->iBW;
				delete pxNext;

				pxNext = imExtended[k].GetPixel(i+2,j+1);
				cGc = (int)pxNext->iBW - (int)pxCurr->iBW;

				pxP.iBW = (unsigned char)sqrt(cGr*cGr + cGc*cGc);				
				imGrad[k].SetPixel(i,j,pxP);
				delete pxCurr;
				delete pxNext;
			}
		}
		int iThresh;

		imGrad[k].PlotHistogram();
		cout<<"\nEnter Threshold for ("<<(k+1)<<")th Image: ";
		cin>>iThresh;
		
		for(int i = 0;i<iRows;i++)
		{
			for(int j = 0;j<iCols;j++)
			{
				Pixel *pxP;
				pxP = imGrad[k].GetPixel(i,j);

				if(pxP->iBW >= iThresh)
					pxP->iBW = pxP->iBW;
				else
					pxP->iBW = 255;
				imGrad[k].SetPixel(i,j,*pxP);
				delete pxP;
			}
		}
		imGrad[k].WriteImageData();
	}	
	cout<<"\nPress 'C' to continue!";
	getch();
}

//Second Order Derivative Method
void Problem_1_B_b()
{
	int iRows = 256,iCols = 256;
	int iColorFormat = 3;
	Image imInput[3],imGrayScale[3],imExtended[3],imGradientMap[3],imOutput[3];
	string szNames[3] = {"naruto.raw","harrypotter.raw","lena_noisy.raw"};	

	Image im2Grad[3],imLapMap[3],imLoG[3];
	string sz2GradNames[3] = {"naruto_2nd.raw","harry_2nd.raw","lena_2nd.raw"};
	string szLapNames[3] = {"naruto_LoG.raw","harry_LoG.raw","lena_LoG.raw"};
	string szLogNames[3] = {"naruto_LoG.raw","harry_LoG.raw","lena_LoG.raw"};
	float **faLapMap;
	float iT;
	
	cout<<"\n2nd Order Differential Method:";

	//Read Input Imagess
	for(int i = 0;i<3;i++)
	{
		imInput[i].SetImageName(szNames[i]);
		imInput[i].SetImageRows(iRows);
		imInput[i].SetImageCols(iCols);
		imInput[i].SetImageColorFormat(iColorFormat);
		imInput[i].AllocImageMem();
		imInput[i].ReadImageData();

		imGrayScale[i] = *imInput[i].Convert2Gray();
		//imGrayScale[i].WriteImageData();
	}	

	for(int k = 0;k<3;k++)
	{
		im2Grad[k].SetImageName(sz2GradNames[k]);
		im2Grad[k].SetImageRows(iRows);
		im2Grad[k].SetImageCols(iCols);
		im2Grad[k].SetImageColorFormat(1);
		im2Grad[k].AllocImageMem();		

		imLapMap[k].SetImageName(szLapNames[k]);
		imLapMap[k].SetImageRows(iRows);
		imLapMap[k].SetImageCols(iCols);
		imLapMap[k].SetImageColorFormat(1);
		imLapMap[k].AllocImageMem();		

		imLoG[k].SetImageName(szLogNames[k]);
		imLoG[k].SetImageRows(iRows);
		imLoG[k].SetImageCols(iCols);
		imLoG[k].SetImageColorFormat(1);
		imLoG[k].AllocImageMem();

		DeNoiseGaussian(&imGrayScale[k],1);
		cout<<"\nAfter Gaussian Denoising: ";
		imGrayScale[k].WriteImageData();
		
		faLapMap = LoG(&imGrayScale[k],3,&imLapMap[k]);

		imLapMap[k].PlotHistogram();

		cout<<"\nEnter Threshold |T| from Histogram: ";
		cin>>iT;
		Pixel pxP;
		iT = iT/4;
		//Convert to 3 level map
		for(int i = 0;i<iRows;i++)
		{
			for(int j = 0;j<iCols;j++)
			{		
				if(faLapMap[i][j] <= 0)	//-ve
				{
					if(faLapMap[i][j] < -iT)
					{
						faLapMap[i][j] = -1;
						pxP.iBW = (unsigned char)128;
						imLapMap[k].SetPixel(i,j,pxP);
					}
					else
					{
						faLapMap[i][j] = 0;
						pxP.iBW = (unsigned char)0;
						imLapMap[k].SetPixel(i,j,pxP);
					}
				}
				else if(faLapMap[i][j] >= 0)	//+ve
				{
					if(faLapMap[i][j] > iT)
					{
						faLapMap[i][j] = 1;
						pxP.iBW = (unsigned char)255;
						imLapMap[k].SetPixel(i,j,pxP);
					}
					else
					{
						faLapMap[i][j] = 0;
						pxP.iBW = (unsigned char)0;
						imLapMap[k].SetPixel(i,j,pxP);
					}
				}
			}
		}

		imLapMap[k].WriteImageData();
		//Check for hits
		bool bHit = false;
		for(int i = 1;i<255;i++)
		{
			for(int j = 1;j<255;j++)
			{
				Pixel pxP;
				float fP = faLapMap[i][j];
				bHit = false;

				if(fP == 0)
				{
					if(faLapMap[i][j+1] == 1 &&faLapMap[i][j-1] == -1)
					{
						bHit = true;
					}
					else if(faLapMap[i][j-1] == 1 &&faLapMap[i][j+1] == -1)
					{
						bHit = true;
					}
					else if(faLapMap[i+1][j] == 1 &&faLapMap[i-1][j] == -1)
					{
						bHit = true;
					}
					else if(faLapMap[i-1][j] == 1 &&faLapMap[i+1][j] == -1)
					{
						bHit = true;
					}
					else if(faLapMap[i+1][j+1] == 1 &&faLapMap[i-1][j-1] == -1)
					{
						bHit = true;
					}
					else if(faLapMap[i-1][j-1] == 1 &&faLapMap[i+1][+-1] == -1)
					{
						bHit = true;
					}
					else if(faLapMap[i-1][j+1] == 1 &&faLapMap[i+1][j-1] == -1)
					{
						bHit = true;
					}
					else if(faLapMap[i+1][j-1] == 1 &&faLapMap[i-1][j+1] == -1)
					{
						bHit = true;
					}

					if(bHit == true)
					{
						pxP.iBW = 0;
						im2Grad[k].SetPixel(i,j,pxP);
					}
					else
					{
						pxP.iBW = 255;
						im2Grad[k].SetPixel(i,j,pxP);
					}
				}
				else
				{	
					pxP.iBW = 255;
					im2Grad[k].SetPixel(i,j,pxP);
				}				
			}
		}

		delete [] faLapMap;

		im2Grad[k].PlotHistogram();
		im2Grad[k].WriteImageData();
	}

	cout<<"\nPress 'C' to continue!";
	getch();
}

//Background Special Effects
void Problem_1_C()
{
	int iRows = 256,iCols = 256;
	int iColorFormat = 3;
	float fAlpha = -0.80, fBeta = 20;
	string szNames[3] = {"Naruto_Pencil.raw","Harry_Pencil.raw","Lena_Pencil.raw"};
	string szGrainy[3] = {"Naruto_Grainy.raw","Harry_Grainy.raw","Lena_Grainy.raw"};
	Image imOutput[3], *pimRGB, imInput[3];
	Image imGrainy;

	cout<<"\n Grainy Image: ";
	imGrainy.ReadImageInfo();
	imGrainy.ReadImageData();

	for(int k = 0;k<3;k++)
	{
		imInput[k].SetImageName(szNames[k]);
		imInput[k].SetImageRows(iRows);
		imInput[k].SetImageCols(iCols);
		imInput[k].SetImageColorFormat(1);
		imInput[k].AllocImageMem();
		imInput[k].ReadImageData();

		imOutput[k].SetImageName(szGrainy[k]);
		imOutput[k].SetImageRows(iRows);
		imOutput[k].SetImageCols(iCols);
		imOutput[k].SetImageColorFormat(iColorFormat);
		imOutput[k].AllocImageMem();

		for(int i = 0;i<iRows;i++)
		{
			for(int j = 0;j<iCols;j++)
			{
				Pixel pxP;
				Pixel *pxPI = imInput[k].GetPixel(i,j), *pxPG = imGrainy.GetPixel(i,j);
							
				if(pxPI->GetBW() == 255)
				{
					pxP.iR = pxPI->GetBW() + fAlpha*pxPG->iR + fBeta;
					pxP.iG = pxPI->GetBW() + fAlpha*pxPG->iG + fBeta;
					pxP.iB = pxPI->GetBW() + fAlpha*pxPG->iB + fBeta;
				}
				else
				{
					pxP.iR = pxPI->GetBW();
					pxP.iG = pxPI->GetBW();
					pxP.iB = pxPI->GetBW();
				}
				imOutput[k].SetPixel(i,j,pxP);

				delete pxPI;
				delete pxPG;
			}
		}

		//delete pimRGB;
		imOutput[k].WriteImageData();
	}
	cout<<"\nPress 'C' to continue!";
	getch();
}

//Transition Effects
void Problem_1_D_a()
{
	Image imPencil[3],imColor[3], imFrame[3], *pimPencilRGB[3];
	string szPencil[3] = {"Naruto_Pencil.raw","Harry_Pencil.raw","Lena_Pencil.raw"};
	string szColor[3] = {"naruto.raw","harrypotter.raw","lena_noisy.raw"};
	string szFrame[3] = {"Naruto_Slide_TD.raw","Harry_Slide_TD.raw","Lena_Slide_TD.raw"};
	string szFrame_LR[3] = {"Naruto_Slide_LR.raw","Harry_Slide_LR.raw","Lena_Slide_LR.raw"};

	int iRows = 256,iCols = 256,iColorFormat = 3;

	for(int k = 0;k<3;k++)
	{
		imColor[k].SetImageName(szColor[k]);
		imColor[k].SetImageRows(iRows);
		imColor[k].SetImageCols(iCols);
		imColor[k].SetImageColorFormat(iColorFormat);
		imColor[k].AllocImageMem();
		imColor[k].ReadImageData();
	}

	for(int k = 0;k<3;k++)
	{
		imPencil[k].SetImageName(szPencil[k]);
		imPencil[k].SetImageRows(iRows);
		imPencil[k].SetImageCols(iCols);
		imPencil[k].SetImageColorFormat(1);
		imPencil[k].AllocImageMem();
		imPencil[k].ReadImageData();

		pimPencilRGB[k] = imPencil[k].Convert2RGB();
	}

	for(int k = 0;k<3;k++)
	{
		imFrame[k].SetImageName(szFrame[k]);
		imFrame[k].SetImageRows(iRows);
		imFrame[k].SetImageCols(iCols);
		imFrame[k].SetImageColorFormat(iColorFormat);
		imFrame[k].AllocImageMem();

		//Copy The Color image
		for(int i = 0;i<iRows;i++)
		{
			for(int j = 0;j<iCols;j++)
			{
				Pixel *pxP = imColor[k].GetPixel(i,j);
				imFrame[k].SetPixel(i,j,*pxP);
				delete pxP;
			}
		}		
	}

	for(int k = 0;k<3;k++)
	{
		//TopDown Slide
		for(int f = 0;f<64;f++)
		{
			//Get New Frame Name
			char szName[45];
			int iPos = szFrame[k].find('.');
			sprintf(szName,"%s_F_%d.raw",szFrame[k].substr(0,iPos).c_str(),f);			
			string strName(szName);
			imFrame[k].SetImageName(strName);

			Pixel *pxP;
			for(int i= 0;i<(f+1)*4;i++)
			{
				for(int j = 0;j<iCols;j++)
				{
					Pixel *pxP = pimPencilRGB[k]->GetPixel(i,j);
					imFrame[k].SetPixel(i, j,*pxP);
				}
			}

			imFrame[k].WriteImageData();
		}		
	}

	for(int k =0;k<3;k++)
	{
		for(int i = 0;i<iRows;i++)
		{
			for(int j = 0;j<iCols;j++)
			{
				Pixel *pxP = imColor[k].GetPixel(i,j);
				imFrame[k].SetPixel(i,j,*pxP);
				delete pxP;
			}		
		}
	}

	//Left to right slide
	for(int k = 0;k<3;k++)
	{
		for(int f = 0;f<64;f++)
		{
			//Get New Frame Name
			char szName[45];
			int iPos = szFrame[k].find('.');
			sprintf(szName,"%s_F_%d.raw",szFrame_LR[k].substr(0,iPos).c_str(),f);			
			string strName(szName);
			imFrame[k].SetImageName(strName);

			Pixel *pxP;
			for(int j = 0;j<iCols;j++)
			{
				for(int i = 0;i<(f+1)*4;i++)
				{
						Pixel *pxP = pimPencilRGB[k]->GetPixel(j,i);
						imFrame[k].SetPixel(j,i,*pxP);
				}
			}	

			imFrame[k].WriteImageData();
		}		
	}

	cout<<"\nPress 'C' to continue!";
	getch();
}

void Problem_1_D_b()
{
	Image imPencil[3],imColor[3], imFrame[3], *pimPencilRGB[3];
	string szPencil[3] = {"Naruto_Pencil.raw","Harry_Pencil.raw","Lena_Pencil.raw"};
	string szColor[3] = {"naruto.raw","harrypotter.raw","lena_noisy.raw"};
	string szFrame[3] = {"Naruto_Tran.raw","Harry_Tran.raw","Lena_Tran.raw"};
	int iRows = 256,iCols = 256,iColorFormat = 3;

	for(int k = 0;k<3;k++)
	{
		imColor[k].SetImageName(szColor[k]);
		imColor[k].SetImageRows(iRows);
		imColor[k].SetImageCols(iCols);
		imColor[k].SetImageColorFormat(iColorFormat);
		imColor[k].AllocImageMem();
		imColor[k].ReadImageData();
	}

	for(int k = 0;k<3;k++)
	{
		imPencil[k].SetImageName(szPencil[k]);
		imPencil[k].SetImageRows(iRows);
		imPencil[k].SetImageCols(iCols);
		imPencil[k].SetImageColorFormat(1);
		imPencil[k].AllocImageMem();
		imPencil[k].ReadImageData();

		pimPencilRGB[k] = imPencil[k].Convert2RGB();
	}

	for(int k = 0;k<3;k++)
	{
		imFrame[k].SetImageName(szFrame[k]);
		imFrame[k].SetImageRows(iRows);
		imFrame[k].SetImageCols(iCols);
		imFrame[k].SetImageColorFormat(iColorFormat);
		imFrame[k].AllocImageMem();

		//Copy THe Color image
		for(int i = 0;i<iRows;i++)
		{
			for(int j = 0;j<iCols;j++)
			{
				Pixel *pxP = imColor[k].GetPixel(i,j);
				imFrame[k].SetPixel(i,j,*pxP);
				delete pxP;
			}
		}		
	}

	for(int k = 0;k<3;k++)
	{
		int iF = 0;
		//Transition Effect
		for(float f = -0.5;f<+0.5;f += 0.05)
		{
			//Get New Frame Name
			char szName[45];
			int iPos = szFrame[k].find('.');
			sprintf(szName,"%s_F_%d.raw",szFrame[k].substr(0,iPos).c_str(),iF);	
			iF++;
			string strName(szName);
			imFrame[k].SetImageName(strName);

			Pixel *pxP;
			for(int i= 0;i<iRows;i++)
			{
				for(int j = 0;j<iCols;j++)
				{
					Pixel *pxP1 = pimPencilRGB[k]->GetPixel(i,j);
					Pixel *pxP2 = imColor[k].GetPixel(i,j);

					char cR,cG,cB;

					cR = (0.5 - f)*pxP1->iR + (0.5 + f)*pxP2->iR;
					cG = (0.5 - f)*pxP1->iG + (0.5 + f)*pxP2->iG;
					cB = (0.5 - f)*pxP1->iB + (0.5 + f)*pxP2->iB;

					Pixel pxP(cR,cG,cB);
					imFrame[k].SetPixel(i, j,pxP);
				}
			}

			imFrame[k].WriteImageData();
		}		
	}
	cout<<"\nPress 'C' to continue!";
	getch();
}

//Color Sketch
void Problem_1_E()
{
	int iRows = 256,iCols = 256;

	Image imInput, *imGray, im1Order, im2ndOrder, *imExtended, imGrad;
	string strColImage("naruto.raw"), strColPencil("naruto_color_pencil.raw"), strColGrad("naruto_C_grad.raw");

	imInput.SetImageName(strColImage);
	imInput.SetImageRows(iRows);
	imInput.SetImageCols(iCols);
	imInput.SetImageColorFormat(3);
	imInput.AllocImageMem();	

	imInput.ReadImageData();

	im1Order.SetImageName(strColPencil);
	im1Order.SetImageRows(iRows);
	im1Order.SetImageCols(iCols);
	im1Order.SetImageColorFormat(3);
	im1Order.AllocImageMem();	

	imGrad.SetImageName(strColGrad);
	imGrad.SetImageRows(iRows);
	imGrad.SetImageCols(iCols);
	imGrad.SetImageColorFormat(1);
	imGrad.AllocImageMem();	

	imGray = imInput.Convert2Gray();
	imExtended = imGray->ImageExtend(1);

	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel *pxCurr,*pxNext;
			Pixel pxP;
			int cGr,cGc;
			
			pxCurr = imExtended->GetPixel(i+1,j);
			pxNext = imExtended->GetPixel(i+1,j+2);
			cGr =  (int)pxNext->iBW - (int)pxCurr->iBW;
			delete pxNext;

			pxNext = imExtended->GetPixel(i+2,j+1);
			cGc = (int)pxNext->iBW - (int)pxCurr->iBW;

			pxP.iBW = (unsigned char)sqrt(cGr*cGr + cGc*cGc);				
			imGrad.SetPixel(i,j,pxP);
			delete pxCurr;
			delete pxNext;
		}
	}
	int iThresh;

	imGrad.PlotHistogram();
	cout<<"\nEnter Threshold for Image: ";
	cin>>iThresh;
	
	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel *pxP;
			pxP = imGrad.GetPixel(i,j);

			if(pxP->iBW >= iThresh)
				pxP->iBW = 0;
			else
				pxP->iBW = 255;
			imGrad.SetPixel(i,j,*pxP);
			delete pxP;
		}
	}

	imGrad.WriteImageData();

	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel *pxP;
			pxP = imGrad.GetPixel(i,j);

			if(pxP->GetBW() == 0)
			{
				im1Order.SetPixel(i,j,*imInput.GetPixel(i,j));
			}
			else
			{
				Pixel pxP(255,255,255);
				im1Order.SetPixel(i,j,pxP);
			}
		}
	}

	im1Order.WriteImageData();
	cout<<"\nPress 'C' to continue!";
	getch();
}

void DeNoiseMedian(Image *pimImage,int iWinSize)
{
	Image *pimEx;

	Pixel **pxWin;
	Pixel *pxSeq;

	int iIndex = (iWinSize - 1)/2;

	cout<<"\n*Denoising "<<pimImage->GetImageName().c_str()<<" using Median Filter . . .";

	pxWin = new Pixel*[iWinSize];
	pxSeq = new Pixel[iWinSize*iWinSize];
	
	for(int i = 0;i<iWinSize ;i++)
	{
		pxWin[i] = new Pixel[iWinSize];
	}

	pimEx = pimImage->ImageExtend(iIndex);

	int iRows = pimEx->GetImageRows();
	int iCols = pimEx->GetImageCols();

	cout<<"\n*Applying Median Filter of window "<<(iWinSize-1)/2;
	int R,G,B,BW;	
	for(int i = iIndex;i<iRows-iIndex;i++)
	{
		for(int j = iIndex;j<iCols-iIndex;j++)
		{
			//Slide Window
			for(int l = 0;l<iWinSize;l++)
			{
				for(int m = 0;m<iWinSize;m++)
				{
					Pixel *pxP;
					pxP = pimEx->GetPixel(i + (iIndex - l),j + (iIndex - m));
					pxWin[l][m] = *pxP;
					delete pxP;
				}
			}
			
			//Get Window
			for(int l = 0;l<iWinSize;l++)
			{
				for(int m = 0;m<iWinSize;m++)
				{
					pxSeq[l*iWinSize + m] = pxWin[l][m];
				}
			}

			//Ascending order windows based on color
			unsigned char cR,cG,cB;
			unsigned char cBW;

			for(int l = 0;l<iWinSize*iWinSize;l++)
			{
				for(int m = 0;m<iWinSize*iWinSize - 1;m++)
				{					
					if(pimEx->GetImageColorFormat() == 1)
					{
						if(pxSeq[m].iBW > pxSeq[m+1].iBW)
						{
							cBW = pxSeq[m].iBW;
							pxSeq[m].iBW = pxSeq[m+1].iBW;
							pxSeq[m+1].iBW = cBW;
						}
					}
					else if(pimEx->GetImageColorFormat() == 3)
					{
						//Red
						if(pxSeq[m].iR > pxSeq[m+1].iR)
						{
							cR = pxSeq[m].iR;
							pxSeq[m].iR = pxSeq[m+1].iR;
							pxSeq[m+1].iR = cR;
						}

						//Green
						if(pxSeq[m].iG > pxSeq[m+1].iG)
						{
							cG = pxSeq[m].iG;
							pxSeq[m].iG = pxSeq[m+1].iG;
							pxSeq[m+1].iG = cG;
						}

						//Blue
						if(pxSeq[m].iB > pxSeq[m+1].iB)
						{
							cB = pxSeq[m].iB;
							pxSeq[m].iB = pxSeq[m+1].iB;
							pxSeq[m+1].iB = cB;
						}
					}
				}
			}			
			if(pimImage->GetImageColorFormat() == 3)
			{
				Pixel pxP(cR,cG,cB);
				pimImage->SetPixel(i-iIndex,j-iIndex,pxP);
			}
			else if(pimImage->GetImageColorFormat() == 1)
			{
				pimImage->SetPixel(i-iIndex,j-iIndex,pxSeq[(iWinSize*iWinSize - 1)/2]);
			}				
		}
	}
}

float** LoG(Image *pimImage,int iWinSize, Image *pimLapMap)
{
	Image *pimEx;

	int iaLoGWin[3][3] = {{0,-1,0},{-1,4,-1},{0,-1,0}};
	float **faImLoG;

	int iIndex = (iWinSize - 1)/2;	

	cout<<"\n*Calculating Laplacian of "<<pimImage->GetImageName().c_str()<<" . . .";

	pimEx = pimImage->ImageExtend(iIndex);

	int iRows = pimEx->GetImageRows();
	int iCols = pimEx->GetImageCols();

	faImLoG = new float*[iRows];
	for(int i = 0;i<iRows;i++)
		faImLoG[i] = new float[iCols];

	float fBW = 0.00;

	for(int i = iIndex;i<iRows-iIndex;i++)
	{
		for(int j = iIndex;j<iCols-iIndex;j++)
		{
			//Slide Window
			fBW = 0.00;
			for(int l = 0;l<iWinSize;l++)
			{
				for(int m = 0;m<iWinSize;m++)
				{
					Pixel *pxP;
					pxP = pimEx->GetPixel(i + (iIndex - l),j + (iIndex - m));
					
					fBW +=  iaLoGWin[l][m]* (pxP->iBW);
					delete pxP;
				}
			}

			faImLoG[i - iIndex][j - iIndex] = fBW/4;
			if(fBW < 0)
				fBW = -1*fBW;
			
			Pixel pxP((int)fBW);
			pimLapMap->SetPixel(i-iIndex,j-iIndex,pxP);			
		}
	}

	delete pimEx;

	return faImLoG;
}

void DeNoiseNLM(Image *pimImage)
{

}

void DeNoiseGaussian(Image *pimImage, int sigma)
{
	float iaGWin[3][3] = {{1,2,1},{2,4,2},{1,2,1}};
	int iWinSize = 3;
	Image *pimEx;

	/*sigma = 1;
	for(int i = 0;i<5;i++)
	{
		for(int j = 0;j<5;j++)
		{
			iaGWin[i][j] =100*(1/(2*3.14*0.25)) *(exp(-1*((i-2)*(i-2) + (j-2)*(j-2))/(2*0.25)));
		}
	}*/

	unsigned char **pxWin;

	int iIndex = (iWinSize - 1)/2;

	cout<<"\n*Denoising "<<pimImage->GetImageName().c_str()<<" using Gaussian Filter . . .";

	pxWin = new unsigned char*[iWinSize];
	
	for(int i = 0;i<iWinSize ;i++)
	{
		pxWin[i] = new unsigned char[iWinSize];
	}

	pimEx = pimImage->ImageExtend(iIndex);

	int iRows = pimEx->GetImageRows();
	int iCols = pimEx->GetImageCols();

	int R,G,B,BW;	
	for(int i = iIndex;i<iRows-iIndex;i++)
	{
		for(int j = iIndex;j<iCols-iIndex;j++)
		{
			//Slide Window
			int cAvg = 0;
			for(int l = 0;l<iWinSize;l++)
			{
				for(int m = 0;m<iWinSize;m++)
				{
					Pixel *pxP;
					pxP = pimEx->GetPixel(i + (iIndex - l),j + (iIndex - m));
					cAvg += iaGWin[l][m]*pxP->iBW;					
					delete pxP;
				}
			}							

			cAvg /= 16;
			Pixel pxP((unsigned char)cAvg);
								
			if(pimImage->GetImageColorFormat() == 1)
			{
				pimImage->SetPixel(i-iIndex,j-iIndex,pxP);
			}				
		}
	}
}

/*pimImage->Convert2Gray();
	/*

	int cBW = 0;
				if(bHit != true)
				{
					for(int l = 0;l<3;l++)
					{
						for(int m = 0;m<3;m++)
						{
							pxP = imLapMap[k].GetPixel(i -1 + l,j - 1 + m);
							cBW += pxP->iBW * iaHit1[l][m];
						}
					}
					if(cBW == 81409)
						bHit = true;
				}

				if(bHit != true)
				{
					cBW = 0;
					for(int l = 0;l<3;l++)
					{
						for(int m = 0;m<3;m++)
						{
							pxP = imLapMap[k].GetPixel(i -1 + l,j - 1 + m);
							cBW += pxP->iBW * iaHit2[l][m];
						}
					}
					if(cBW == 81409)
						bHit = true;
				}
				if(bHit != true)
				{
					cBW = 0;
					for(int l = 0;l<3;l++)
					{
						for(int m = 0;m<3;m++)
						{
							pxP = imLapMap[k].GetPixel(i -1 + l,j - 1 + m);
							cBW += pxP->iBW * iaHit3[l][m];
						}
					}
					if(cBW == 81409)
						bHit = true;
				}
				if(bHit != true)
				{
					cBW = 0;
					for(int l = 0;l<3;l++)
					{
						for(int m = 0;m<3;m++)
						{
							pxP = imLapMap[k].GetPixel(i -1 + l,j - 1 + m);
							cBW += pxP->iBW * iaHit4[l][m];
						}
					}
					if(cBW == 81409)
						bHit = true;
				}
				if(bHit != true)
				{
					cBW = 0;
					for(int l = 0;l<3;l++)
					{
						for(int m = 0;m<3;m++)
						{
							pxP = imLapMap[k].GetPixel(i -1 + l,j - 1 + m);
							cBW += pxP->iBW * iaHit5[l][m];
						}
					}
					if(cBW == 81409)
						bHit = true;
				}
				if(bHit != true)
				{
					cBW = 0;
					for(int l = 0;l<3;l++)
					{
						for(int m = 0;m<3;m++)
						{
							pxP = imLapMap[k].GetPixel(i -1 + l,j - 1 + m);
							cBW += pxP->iBW * iaHit6[l][m];
						}
					}
					if(cBW == 81409)
						bHit = true;
				}
				if(bHit != true)
				{
					cBW = 0;
					for(int l = 0;l<3;l++)
					{
						for(int m = 0;m<3;m++)
						{
							pxP = imLapMap[k].GetPixel(i -1 + l,j - 1 + m);
							cBW += pxP->iBW * iaHit7[l][m];
						}
					}
					if(cBW == 81409)
						bHit = true;
				}
				if(bHit != true)
				{
					cBW = 0;
					for(int l = 0;l<3;l++)
					{
						for(int m = 0;m<3;m++)
						{
							pxP = imLapMap[k].GetPixel(i -1 + l,j - 1 + m);
							cBW += pxP->iBW * iaHit8[l][m];
						}
					}
					if(cBW == 81409)
						bHit = true;
				}
				
				//if not hit then background
				if(bHit == true)
				{
					pxP->iBW = 0;
					im2Grad[k].SetPixel(i-1,j-1,*pxP);
				}
				else
				{
					pxP->iBW = 255;
					im2Grad[k].SetPixel(i-1,j-1,*pxP);
				}
			}

	*/
