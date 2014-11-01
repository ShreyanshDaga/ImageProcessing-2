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

#include"Image.h"
#include"HW_2_Q_1.h"
#include"Histogram.h"

using namespace std;

Histogram::Histogram()
{
	return;
}

Histogram::~Histogram()
{	
}

void Histogram::FreeHistogram()
{
	if(pcHistImage == NULL)
		return;
	for (int i = 0; i < 1024; i++)
  	{
		delete [] this->pcHistImage[i];
  	}
	delete[] this->pcHistImage;

	if(pxaPixArr ==NULL)
		return;
	for (int i = 0; i < this->iRows; i++)
  	{
		delete [] this->pxaPixArr[i];
  	}
	delete[] this->pxaPixArr;

	pxaPixArr = NULL;
	pcHistImage = NULL;
}

void Histogram::AllocHistData()
{
	this->pxaPixArr = new Pixel*[iRows];

	for(int i = 0;i<iRows;i++)
	{
		this->pxaPixArr[i] = new Pixel[iCols];
	}

	//Init Hist image data
	/*this->pcHistImage = new unsigned char* [this->iRows*this->iCols];	//iPixelCount

	for(int i = 0;i<iRows*iCols;i++)
	{
		this->pcHistImage[i] = new unsigned char[1024];
	}*/

	//Init histogram arrays
	for (int i = 0; i <256; ++i)
	{
		this->iHistogramR[i] = 0;
		this->iHistogramG[i] = 0;
		this->iHistogramB[i] = 0;
		this->iHistogramBW[i] = 0;

		this->iCumHistogramR[i] = 0;
		this->iCumHistogramG[i] = 0;
		this->iCumHistogramB[i] = 0;
		this->iCumHistogramBW[i] = 0;
	}
}

void Histogram::InitHist(string szImName, int iRows,int iCols, int iColorFormat)
{
	this->szHistName = szImName;

	this->iRows = iRows;
	this->iCols = iCols;
	this->iColorFormat = iColorFormat;

	this->AllocHistData();
}

void Histogram::PlotHistogram()
{
	for (int i = 0; i < this->iRows; i++)
	{
		for(int j = 0;j<this->iCols;j++)
		{
			if(this->iColorFormat == 3)
			{
				this->iHistogramR[this->pxaPixArr[i][j].GetRed()]++;
				this->iHistogramG[this->pxaPixArr[i][j].GetGreen()]++;
				this->iHistogramB[this->pxaPixArr[i][j].GetBlue()]++;
			}
			else if(this->iColorFormat == 1)
			{				
				iHistogramBW[this->pxaPixArr[i][j].GetBW()]++;
			}
		}
	}

	if(this->iColorFormat == 1)
		WriteHistogramFile(this->iHistogramBW,'g','n');
	else if(this->iColorFormat == 3)
	{
		WriteHistogramFile(this->iHistogramR,'R','n');
		WriteHistogramFile(this->iHistogramG,'G','n');
		WriteHistogramFile(this->iHistogramB,'B','n');			
	}
}

void Histogram::PlotCumHistogram()
{
	if(this->iColorFormat == 3)
	{
		this->iCumHistogramR[0] = this->iHistogramR[0];
		for(int j = 1;j<255;j++)
		{
			this->iCumHistogramR[j] = this->iCumHistogramR[j-1] + this->iHistogramR[j];
		}

		this->iCumHistogramG[0] = this->iHistogramG[0];
		for(int j = 1;j<255;j++)
		{
			this->iCumHistogramG[j] = this->iCumHistogramG[j-1] + this->iHistogramG[j];
		}

		this->iCumHistogramB[0] = this->iHistogramB[0];
		for(int j = 1;j<255;j++)
		{
			this->iCumHistogramB[j] = this->iCumHistogramB[j-1] + this->iHistogramB[j];
		}
	}
	else if(this->iColorFormat == 1)
	{
		this->iCumHistogramBW[0] = this->iHistogramBW[0];
		for(int j = 1;j<255;j++)
		{
			this->iCumHistogramBW[j] = this->iCumHistogramBW[j-1] + this->iHistogramBW[j];
		}
	}

	if(this->iColorFormat == 1)
		WriteHistogramFile(this->iCumHistogramBW,'g','c');
	else if(this->iColorFormat == 3)
	{
		WriteHistogramFile(this->iCumHistogramR,'R','c');
		WriteHistogramFile(this->iCumHistogramG,'G','c');
		WriteHistogramFile(this->iCumHistogramB,'B','c');			
	}
}

void Histogram::WriteHistogramFile(int iHistCount[256], char cColor, char cType)
{
	unsigned char pcHistBorder[20][1024];
	int iRowMax;
	int iElemCount = 0;
	int iPixelCount = this->iRows*this->iCols;
	FILE *fp;
	char szHistFileName[30];

	//Find Max Histogram height
	iRowMax = iHistCount[0];
	for(int i = 0;i < 256;i++)
	{
		if(iRowMax < iHistCount[i+1] && i <255)
		{
			iRowMax = iHistCount[i+1];
		}

		iElemCount += iHistCount[i];
	}

	int iPos = this->szHistName.find('.');

	//Allocate Mem for pcHistData
	this->pcHistImage = new unsigned char* [iRowMax];	//iPixelCount

	for(int i = 0;i<iRowMax;i++)
	{
		this->pcHistImage[i] = new unsigned char[1024];
	}

	//Fill pcHisData with white background
	for(int i = 0;i < iRowMax;i++)
	{
		for(int j = 0;j < 1024;j++)
		{
			this->pcHistImage[i][j] = (unsigned char)255;
		}
	}

	//fatten the histograms
	for(int i = 0 ; i < 1024; i+=4)
	{
		for(int j = iRowMax - 1; j >= (iRowMax - iHistCount[i/4]); j--)		
		{
			this->pcHistImage[j][i] = (unsigned char)100;
			this->pcHistImage[j][i+1] = (unsigned char)100;
			this->pcHistImage[j][i+2] = (unsigned char)100;
			this->pcHistImage[j][i+3] = (unsigned char)100;				
		}
	}

	if(cType == 'c')
		sprintf(szHistFileName,"%s_CumHist_%c.raw",this->szHistName.substr(0,iPos).c_str(),cColor);
	else
		sprintf(szHistFileName,"%s_Hist_%c.raw",this->szHistName.substr(0,iPos).c_str(),cColor);
	
	fp = fopen(szHistFileName,"wb");

	//Write Histogram file with (max rows x 1024) size
	int k=0;
	if(iRowMax <1500)
	{
		for(int i = 0 ; i<(iRowMax); i++)
		{
			fwrite(this->pcHistImage[i],sizeof(unsigned char),1024,fp);
			k++;
		}
	}
	else
	{
		for(int i = 0 ; i<(iRowMax); i+=iRowMax/1024)
		{
			fwrite(this->pcHistImage[i],sizeof(unsigned char),1024,fp);
			k++;
		}
	}

	//border
	for(int i = 0;i<1024;i++)
	{
		pcHistBorder[0][i] = (unsigned char)0;
		pcHistBorder[1][i] = (unsigned char)0;
		pcHistBorder[18][i] = (unsigned char)0;
		pcHistBorder[19][i] = (unsigned char)0;
	}

	for(int i = 0;i<1024;i+=4)
	{
		for(int j = 2;j<18;j++)
		{
			pcHistBorder[j][i] = (unsigned char)(i/4);
			pcHistBorder[j][i+1] = (unsigned char)(i/4);
			pcHistBorder[j][i+2] = (unsigned char)(i/4);
			pcHistBorder[j][i+3] = (unsigned char)(i/4);				
		}
	}

	for(int i = 0;i<20;i++)
	{
		fwrite(pcHistBorder[i],sizeof(unsigned char),1024,fp);
	}

	//close histogram file
	fclose(fp);

	cout<<"\n*Histogram File Written:";
	cout<<"\n Name: "<<szHistFileName;
	cout<<"\n Size: "<<k + 20<<"x1024"<<"  Pixel Count; "<<iElemCount;
	if(cColor =='g')
		cout<<"\n Color: Grey";
	else
	cout<<"\n Color: "<<cColor;
	
}


//Old Functions
/*
void PlotHistogram(Image imInputImage)
{
	unsigned char **pcHist;
	int iHistogramR[256];
	int iHistogramG[256];
	int iHistogramB[256];	
	int iHistogramBW[256];
	int iPixelCount= imInputImage.iPixelCount;	//imInputImage.GetImageCols()*imInputImage.GetImageRows()/imInputImage.GetImageColorFormat();
	Pixel P, **pPixelArr = new Pixel*[imInputImage.GetImageRows()];

	for(int i = 0;i<imInputImage.GetImageRows();i++)
	{
		pPixelArr[i] = new Pixel[imInputImage.GetImageCols()];
	}

	//Init Hist image data
	pcHist = new unsigned char* [iPixelCount];	//iPixelCount

	for(int i = 0;i<iPixelCount;i++)
	{
		pcHist[i] = new unsigned char[1024];
	}

	//Init histogram arrays
	for (int i = 0; i <256; ++i)
	{
		iHistogramR[i] = 0;
		iHistogramG[i] = 0;
		iHistogramB[i] = 0;
		iHistogramBW[i] = 0;
	}

	//Get all the values of pixels in pixel array
	for(int i=0;i<imInputImage.GetImageRows();i++)
	{
		for(int j=0;j<imInputImage.GetImageCols();j++)
		{
			pPixelArr[i][j] = *(imInputImage.GetPixel(i,j));
		}
	}
	
	//compute count of each color from pixel array;
	for (int i = 0; i < imInputImage.GetImageRows(); i++)
	{
		for(int j = 0;j<imInputImage.GetImageCols();j++)
		{
			if(imInputImage.GetImageColorFormat() == 3)
			{
				iHistogramR[pPixelArr[i][j].GetRed()]++;
				iHistogramG[pPixelArr[i][j].GetGreen()]++;
				iHistogramB[pPixelArr[i][j].GetBlue()]++;
			}
			else if(imInputImage.GetImageColorFormat() == 1)
			{				
				iHistogramBW[pPixelArr[i][j].GetBW()]++;
			}
		}
	}

	if(imInputImage.GetImageColorFormat() == 1)
		WriteHistogramFile(imInputImage, iHistogramBW,pcHist,'g');
	else if(imInputImage.GetImageColorFormat() == 3)
	{
		WriteHistogramFile(imInputImage, iHistogramR,pcHist,'R');
		WriteHistogramFile(imInputImage, iHistogramG,pcHist,'G');
		WriteHistogramFile(imInputImage, iHistogramB,pcHist,'B');			
	}	
	
	delete [] pcHist;
}

void WriteHistogramFile(Image imImage, int iHistCount[256], unsigned char **pcHistData,char cColor)
{
	unsigned char pcHistBorder[20][1024];
	int iRowMax;
	int iElemCount = 0;
	int iPixelCount = imImage.iPixelCount;
	FILE *fp;
	char szHistFileName[30];

	//Find Max Histogram height
	iRowMax = iHistCount[0];
	for(int i = 0;i < 256;i++)
	{
		if(iRowMax < iHistCount[i+1] && i <255)
		{
			iRowMax = iHistCount[i+1];
		}

		iElemCount += iHistCount[i];
	}

	int iPos = imImage.GetImageName().find('.');

	sprintf(szHistFileName,"%s_Hist_%c.raw",imImage.GetImageName().substr(0,iPos).c_str(),cColor);
	
	fp = fopen(szHistFileName,"wb");

	//Fill pcHisData with white background
	for(int i = 0;i < iPixelCount;i++)
	{
		for(int j = 0;j < 1024;j++)
		{
			pcHistData[i][j] = (unsigned char)255;
		}
	}

	//fatten the histograms
	for(int i = 0 ; i < 1024; i+=4)
	{
		for(int j = iPixelCount - 1; j >= (iPixelCount - iHistCount[i/4]); j--)		
		{
			pcHistData[j][i] = (unsigned char)100;
			pcHistData[j][i+1] = (unsigned char)100;
			pcHistData[j][i+2] = (unsigned char)100;
			pcHistData[j][i+3] = (unsigned char)100;				
		}
	}

	//Write Histogram file with (max rows x 1024) size
	int k=0;
	for(int i = (iPixelCount - iRowMax) ; i<iPixelCount ; i+=iRowMax/1024)
	{
		fwrite(pcHistData[i],sizeof(unsigned char),1024,fp);
		k++;
	}

	//border
	for(int i = 0;i<1024;i++)
	{
		pcHistBorder[0][i] = (unsigned char)0;
		pcHistBorder[1][i] = (unsigned char)0;
		pcHistBorder[18][i] = (unsigned char)0;
		pcHistBorder[19][i] = (unsigned char)0;
	}

	for(int i = 0;i<1024;i+=4)
	{
		for(int j = 2;j<18;j++)
		{
			pcHistBorder[j][i] = (unsigned char)(i/4);
			pcHistBorder[j][i+1] = (unsigned char)(i/4);
			pcHistBorder[j][i+2] = (unsigned char)(i/4);
			pcHistBorder[j][i+3] = (unsigned char)(i/4);				
		}
	}

	for(int i = 0;i<20;i++)
	{
		fwrite(pcHistBorder[i],sizeof(unsigned char),1024,fp);
	}

	//close histogram file
	fclose(fp);
	//delete [] pcHistData;

	cout<<"\n*Histogram File Written:";
	cout<<"\n Name: "<<szHistFileName;
	cout<<"\n Size: "<<k + 20<<"x1024"<<"  Element Count; "<<iElemCount;
	cout<<"\n Color: "<<cColor;
}*/