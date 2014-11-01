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
#include<stdlib.h>
#include<conio.h>
#include<string.h>
#include<iostream>

#include"Image.h"
#include"Histogram.h"

Image::Image()
{
	this->pImageDate = NULL;
}

Image::Image(unsigned char **ImageData,int Rows, int Cols, int ColorFormat, string ImageName)
{
	this->pImageDate = ImageData;
	this->iRows = Rows;
	this->iCols = Cols;
	this->iColorFormat  = ColorFormat;
	this->szImageName = ImageName;
}

Image::~Image()
{

}

string Image::GetImageName()
{
	return this->szImageName;
}

int Image::GetImageRows()
{
	return this->iRows;
}

int Image::GetImageCols()
{
	return this->iCols;
}

int Image::GetImageColorFormat()
{
	return this->iColorFormat;
}

void Image::SetImageRows(int iRows)
{
	this->iRows = iRows;
}

void Image::SetImageCols(int iCols)
{
	this->iCols = iCols;
}

void Image::SetImageColorFormat(int iColorFormat)
{
	this->iColorFormat = iColorFormat;
}

void Image::SetImageName(string szName)
{
	this->szImageName = szName;
}

void Image::SetPixel(int X,int Y, Pixel pxP)
{
	if(this->GetImageColorFormat() == 3)
	{
		this->pImageDate[X][Y*3] = pxP.GetRed();
		this->pImageDate[X][Y*3+1] = pxP.GetGreen();
		this->pImageDate[X][Y*3+2] = pxP.GetBlue();
	}
	else if(this->GetImageColorFormat() == 1)
	{
		this->pImageDate[X][Y] = pxP.GetBW();
	}
}

Pixel* Image::GetPixel(int X, int Y)
{
	int R, G, B, BW;
	Pixel *pPixel;

	if(this->GetImageColorFormat() == 1)
	{
		BW = this->pImageDate[X][Y];
	}
	else
	{
		R = this->pImageDate[X][3*Y];
		G = this->pImageDate[X][3*Y+1];
		B = this->pImageDate[X][3*Y+2];
	}

	if(this->GetImageColorFormat() == 1)
	{
		pPixel = new Pixel(BW ,X ,Y, true);
	}
	else if(this->GetImageColorFormat() == 3)
	{
		pPixel  = new Pixel(R, G, B, X, Y);
	}

	return pPixel;
}

Pixel* Image::GetNBPixel(Pixel pxP, int idX, int idY)
{
	Pixel *pNBP = new Pixel;
	int iCX, iCY;

	iCX = pxP.GetPixelX();
	iCY = pxP.GetPixelY();

	pNBP = this->GetPixel(iCX + idX,iCY + idY);

	return pNBP;
}

void Image::WriteImageData()
{
	cout<<"\n*Writing Image to File...";
	FILE *fpOutImageFile;
	char szOpImageName[30];
	int iImageSize;

	
	strcpy(szOpImageName,this->szImageName.c_str());

	fpOutImageFile = fopen(szOpImageName,"wb");	

	for(int i = 0;i<this->iRows;i++)
	{
		fwrite(this->pImageDate[i],sizeof(unsigned char),iCols*iColorFormat,fpOutImageFile);
	}
	fclose(fpOutImageFile);

	fopen(szOpImageName,"rb");
	
	fseek (fpOutImageFile, 0, SEEK_END); 
	iImageSize = ftell(fpOutImageFile);

	cout<<"\n*Output Image Data write complete:";
	cout<<"\n Name: "<<szOpImageName;
	cout<<"\n Size: "<<this->iCols<<"x"<<iRows<<", "<<iImageSize/1024<<" KBytes";
	cout<<"\n Color Format: "<<this->iColorFormat;

	fclose(fpOutImageFile);
}

void Image::ReadImageData()
{
	char szImageFileName[30];	
	FILE *fpImageFileIn;	
	int iImageSize;

	strcpy(szImageFileName,this->szImageName.c_str());
	while(1)
	{
		fpImageFileIn = fopen(szImageFileName,"rb");
		
		if(!fpImageFileIn)
		{
			cout<<"\nCan Not Open File. ";
			cout<<"Please Enter File name correctly.!";
			cout<<"\nFile Name: ";
			cin>>szImageFileName;
		}
		else
		{
			this->szImageName.assign(szImageFileName);
			break;
		}
	}

	for(int i = 0;i < this->GetImageRows() ;i++)
	{
		fread(this->pImageDate[i],sizeof(unsigned char),this->iCols*this->iColorFormat,fpImageFileIn);
	}

	fseek (fpImageFileIn, 0, SEEK_END);   
	iImageSize = ftell(fpImageFileIn);
	fclose(fpImageFileIn);	

	cout<<"\n*Input Image Data read complete:";
	cout<<"\n Name: "<<szImageFileName;
	cout<<"\n Size: "<<iImageSize/1024<<" KBytes";
	cout<<"\n Color Format: "<<this->iColorFormat;
}

void Image::ReadImageInfo()
{
	char szImageFileName[30];
	int iRows,iCols,iColorFormat;

	cout<<"\n Enter Image Name: ";
	cin>>szImageFileName;
	cout<<" Enter the Number of Rows: ";
	cin>>iRows;
	cout<<" Enter the Number of Cols: ";
	cin>>iCols;
	cout<<" Enter the Color Format (RBG: 3, GraScale: 1): ";
	cin>>iColorFormat;

	this->szImageName.assign(szImageFileName);
	this->iRows = iRows;
	this->iCols = iCols;
	this->iColorFormat = iColorFormat;
	this->iPixelCount = iRows*iCols;

	this->AllocImageMem();	
}

void Image::AllocImageMem()
{
	this->iPixelCount = this->iRows*this->iCols;
	this->pImageDate = new unsigned char* [this->GetImageRows()];	

	for(int i = 0;i< this->GetImageRows();i++)
	{
		this->pImageDate[i] = new unsigned char[this->GetImageCols()*this->GetImageColorFormat()];
	}

	this->hHist.InitHist(this->szImageName,this->GetImageRows(),this->GetImageCols(),this->GetImageColorFormat());
}

void Image::DrawLine(int X1, int Y1, int X2, int Y2, Pixel pxP)
{
	int idX = abs(X2 - X1), idY(Y2 - Y1);
	int iErr = idX - idY;
	int iSx,iSy;

	if(X1 < X2)
	{
		iSx = 1;
	}
	else
	{
		iSx = -1;
	}

	if(Y1 < Y2)
	{
		iSy = 1;
	}
	else
	{
		iSy = -1;
	}

	while(1)
	{
		this->SetPixel(X1,Y1,pxP);

		if(X1 == X2 && Y1 == Y2)
			break;

		if(iErr*2 > -idY)
		{
			iErr -= idY;
			X1 += iSx;
		}

		if(X1 == X2 && Y1 == Y2)
		{
			this->SetPixel(X1,Y1,pxP);
			break;
		}

		if(iErr*2 < idX)
		{
			iErr += idX;
			Y1 += iSy;
		}
	}
}

void Image::PlotHistogram()
{
	cout<<"\n*Ploting Histogram...";
	//Fill The Pixel Array used for Histogram
	Pixel *ppxP;

	for(int i=0;i<this->GetImageRows();i++)
	{
		for(int j=0;j<this->GetImageCols();j++)
		{
			ppxP = this->GetPixel(i,j);
			this->hHist.pxaPixArr[i][j] = *ppxP;
			delete ppxP;
		}
	}

	this->hHist.PlotHistogram();
}

void Image::PlotCumHistogram()
{
	//Fill The Pixel Array used for Histogram
	Pixel *ppxP;

	for(int i=0;i<this->GetImageRows();i++)
	{
		for(int j=0;j<this->GetImageCols();j++)
		{
			ppxP = this->GetPixel(i,j);
			this->hHist.pxaPixArr[i][j] = *ppxP;
			delete ppxP;
		}
	}

	this->hHist.PlotCumHistogram();
}

void Image::FreeResources()
{

	this->hHist.FreeHistogram();

	if(this->pImageDate ==NULL)
		return;

	for (int i = 0; i < this->iRows; i++)
  	{
		delete [] this->pImageDate[i];
  	}

	delete[] this->pImageDate;

	this->pImageDate = NULL;
}

Image* Image::ImageExtend(int iSize)
{
	cout<<"\n*Extending Image...";
	Image* imTemp = new Image();

	int iRow = this->iRows + iSize*2;
	int iCol = this->iCols + iSize*2;
	int iColorFormat = this->iColorFormat;
	int iPixelCount = iRow*iCol;
	char szName[45];

	imTemp->SetImageRows(iRow);
	imTemp->SetImageCols(iCol);
	imTemp->SetImageColorFormat(iColorFormat);
	imTemp->iPixelCount = iPixelCount;
	int iPos = this->GetImageName().find('.');
	sprintf(szName,"%s_Ex.raw",this->GetImageName().substr(0,iPos).c_str());
	string strFileName(szName);
	imTemp->SetImageName(strFileName);
	imTemp->AllocImageMem();	

	//Mid	
	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel  *pxP = this->GetPixel(i,j);
			imTemp->SetPixel(i+iSize,j+iSize,*pxP);
			delete pxP;
		}
	}

	//TopLeft
	Pixel *pxP = this->GetPixel(0,0);
	for(int i = 0;i<iSize;i++)
	{
		for(int j = 0;j<iSize;j++)
		{			
			imTemp->SetPixel(i,j,*pxP);						
		}
	}
	delete pxP;
	
	//TopRight
	pxP = this->GetPixel(0,iCols - 1);
	for(int i = 0;i<iSize;i++)
	{
		for(int j = 0;j<iSize;j++)
		{			
			imTemp->SetPixel(i,j + iCols + iSize,*pxP);			
		}
	}
	delete pxP;
	
	//BottomLeft
	pxP = this->GetPixel(iRows - 1,0);
	for(int i = 0;i<iSize;i++)
	{
		for(int j = 0;j<iSize;j++)
		{
			
			imTemp->SetPixel(iRows + i + iSize,j,*pxP);
			
		}
	}
	delete pxP;
	
	//BottomRight
	pxP = this->GetPixel(iRows - 1,iCols - 1);
	for(int i = 0;i<iSize;i++)
	{
		for(int j = 0;j<iSize;j++)
		{			
			imTemp->SetPixel(iRows + i + iSize,iCols + j + iSize,*pxP);			
		}
	}
	delete pxP;			

	//Left
	for(int i = 0;i<iRows;i++)
	{
		Pixel *pxP;
		pxP = this->GetPixel(i,0);
		for(int j = 0;j<iSize;j++)
		{			
			imTemp->SetPixel(i+iSize,j,*pxP);			
		}
		delete pxP;
	}
	
	//Right
	for(int i = 0;i<iRows;i++)
	{		
		Pixel *pxP;
		pxP = this->GetPixel(i,iCols - 1);		
		for(int j = iCols+iSize;j<iCols+iSize+iSize;j++)
		{			
			imTemp->SetPixel(i + iSize,j,*pxP);		
		}
		delete pxP;
	}

	//Top
	for(int j = 0;j<iCols;j++)
	{
		Pixel *pxP;
		pxP = this->GetPixel(0,j);
		for(int i = 0;i<iSize;i++)
		{			
			imTemp->SetPixel(i,j+iSize,*pxP);			
		}
		delete pxP;
	}

	//Bottom
	for(int j = 0;j<iCols;j++)
	{
		Pixel *pxP;
		pxP = this->GetPixel(iRows - 1,j);
		for(int i = iRows + iSize;i<iRows+iSize + iSize;i++)
		{			
			imTemp->SetPixel(i,j+iSize,*pxP);			
		}
		delete pxP;
	}
	
	return imTemp;
}

Image* Image::Convert2RGB()
{
	cout<<"\n*Converting Image to RGB...";
	Image *imTemp = new Image();

	int iRows = this->iRows;
	int iCols = this->iCols;
	int iColorFormat = 3;
	imTemp->SetImageRows(iRows);
	imTemp->SetImageCols(iCols);
	imTemp->SetImageColorFormat(iColorFormat);
	imTemp->AllocImageMem();

	Pixel *pxP;

	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			pxP = this->GetPixel(i,j);
			pxP->iR = pxP->iBW;
			pxP->iG = pxP->iBW;
			pxP->iB = pxP->iBW;
			imTemp->SetPixel(i,j,*pxP);
			delete pxP;
		}
	}	
	
	char szFileName[45];

	int iPos = this->GetImageName().find('.');
	sprintf(szFileName,"%s_RGB.raw",this->GetImageName().substr(0,iPos).c_str());

	string strFileName(szFileName);
	imTemp->SetImageName(strFileName);
	cout<<"\n*Converted "<<this->szImageName.c_str()<<" into RGB scale "<<szFileName<<" image";

	return imTemp;
}

Image* Image::Convert2Gray()
{
	cout<<"\n*Converting Image to Gray Scale...";
	Image *imTemp = new Image();

	int iRows = this->iRows;
	int iCols = this->iCols;
	int iColorFormat = 1;
	char szFileName[45];

	int iPos = this->GetImageName().find('.');
	sprintf(szFileName,"%s_Gray.raw",this->GetImageName().substr(0,iPos).c_str());

	string strFileName(szFileName);
	imTemp->SetImageName(strFileName);	
	imTemp->SetImageRows(iRows);
	imTemp->SetImageCols(iCols);
	imTemp->SetImageColorFormat(iColorFormat);
	imTemp->AllocImageMem();

	Pixel *pxP;

	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			pxP = this->GetPixel(i,j);
			imTemp->pImageDate[i][j] = (unsigned char)pxP->GetBW();
			delete pxP;
		}
	}	

	cout<<"\n*Converted "<<this->szImageName.c_str()<<" into gray scale "<<szFileName<<" image";
	return imTemp;
}

Image* Image::Convert2YUV()
{
	Image *imTemp = new Image();

	int iRows = this->iRows;
	int iCols = this->iCols;
	int iColorFormat = 3;
	imTemp->SetImageRows(iRows);
	imTemp->SetImageCols(iCols);
	imTemp->SetImageColorFormat(iColorFormat);
	imTemp->AllocImageMem();

	Pixel *pxP;
	unsigned char cY,cU,cV;

	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			pxP = this->GetPixel(i,j);

			cY = pxP->GetBW();
			cU = 0.492*(pxP->iB - cY);
			cV = 0.877*(pxP->iR - cY);

			pxP->iR = cY;
			pxP->iG = cU;
			pxP->iB = cV;

			imTemp->SetPixel(i,j,*pxP);	
			delete pxP;
		}
	}	
	
	char szFileName[45];

	int iPos = this->GetImageName().find('.');
	sprintf(szFileName,"%s_YUV.raw",this->GetImageName().substr(0,iPos).c_str());

	string strFileName(szFileName);
	imTemp->SetImageName(strFileName);
	cout<<"\n*Converted "<<this->szImageName.c_str()<<" into YUV scale "<<szFileName<<" image";

	return imTemp;
}

Image* Image::Convert2YCrCb()
{
	Image *imTemp = new Image();

	int iRows = this->iRows;
	int iCols = this->iCols;
	int iColorFormat = 3;
	imTemp->SetImageRows(iRows);
	imTemp->SetImageCols(iCols);
	imTemp->SetImageColorFormat(iColorFormat);
	imTemp->AllocImageMem();

	Pixel *pxP;
	unsigned char cY,cCr,cCb;

	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			pxP = this->GetPixel(i,j);

			cY = pxP->GetBW();
			cCr = -0.16874*pxP->iR - 0.33126*pxP->iG + 0.5*pxP->iB;
			cCb = 0.5*pxP->iR - 0.41869*pxP->iG - 0.08131*pxP->iB;

			pxP->iR = cY;
			pxP->iG = cCr;
			pxP->iB = cCb;

			imTemp->SetPixel(i,j,*pxP);			
			delete pxP;
		}
	}	
	
	char szFileName[45];

	int iPos = this->GetImageName().find('.');
	sprintf(szFileName,"%s_YCrCb.raw",this->GetImageName().substr(0,iPos).c_str());

	string strFileName(szFileName);
	imTemp->SetImageName(strFileName);
	cout<<"\n*Converted "<<this->szImageName.c_str()<<" into YCrCb scale "<<szFileName<<" image";

	return imTemp;
}

void Image::Convert2BMP()
{
	int iRows = this->GetImageRows();
	int iCols = this->GetImageCols();

	FILE *f;
	unsigned char *img = NULL;
	int filesize = 54 + 3*iRows*iCols;  //w is your image width, h is image height, both int
	if( img )
	    free( img );
	img = (unsigned char *)malloc(3*iRows*iCols);
	memset(img,0,sizeof(img));	

	int x,y,yres = 256;
	int w = 256,h=256;
	unsigned char r,g,b;

	for(int i=0; i<256; i++)
	{
	    for(int j=0; j<256; j++)
		{
		    x=i; y=j;
			Pixel *pxP = this->GetPixel(i,j);

			r = pxP->GetRed();
			g = pxP->GetGreen();
			b = pxP->GetBlue();
			
			if (r > 255) r=255;
			if (g > 255) g=255;
			if (b > 255) b=255;
			
			img[(y+x*w)*3+2] = (r);
			img[(y+x*w)*3+1] = (g);
			img[(y+x*w)*3+0] = (b);
		}
	}
	
	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
	unsigned char bmppad[3] = {0,0,0};
	
	bmpfileheader[2] = (unsigned char)(filesize    );
	bmpfileheader[3] = (unsigned char)(filesize>> 8);
	bmpfileheader[4] = (unsigned char)(filesize>>16);
	bmpfileheader[5] = (unsigned char)(filesize>>24);
	
	bmpinfoheader[4] =	(unsigned char)(       iCols    );
	bmpinfoheader[5] =	(unsigned char)(       iCols>> 8);
	bmpinfoheader[6] =	(unsigned char)(       iCols>>16);
	bmpinfoheader[7] =	(unsigned char)(       iCols>>24);
	bmpinfoheader[8] =	(unsigned char)(       iRows    );
	bmpinfoheader[9] =	(unsigned char)(       iRows>> 8);
	bmpinfoheader[10] =	(unsigned char)(       iRows>>16);
	bmpinfoheader[11] =	(unsigned char)(       iRows>>24);
	
	char szBMPName[45];
	int iPos = this->GetImageName().find('.');
	sprintf(szBMPName,"%s_BMP.bmp",this->GetImageName().substr(0,iPos).c_str());

	f = fopen(szBMPName,"wb");
	
	fwrite(bmpfileheader,1,14,f);
	
	fwrite(bmpinfoheader,1,40,f);
	
	for(int i=0; i<h; i++)
	{
	    fwrite(img+(w*(h-i-1)*3),3,w,f);
	    fwrite(bmppad,1,(4-(w*3)%4)%4,f);
	}
	fclose(f);

	cout<<"\n*Output Image Data write complete:";
	cout<<"\n Name: "<<szBMPName;
	cout<<"\n Size: "<<this->iCols<<"x"<<iRows<<", "<<filesize/1024<<" KBytes";
	cout<<"\n Color Format: Bitmap RGB";
}

int** Image::GetIntArrat()
{
	int **iaTemp;
	int iRows = this->iRows;
	int iCols = this->iCols;

	iaTemp = new int*[iRows];
	for(int i = 0;i<iRows;i++)
		iaTemp[i] = new int[iCols];

	for(int i = 0;i<iRows;i++)
	{
		for(int j = 0;j<iCols;j++)
		{
			Pixel *pxP = this->GetPixel(i,j);

			if(pxP->GetBW() == 0)
				iaTemp[i][j] = 1;
			else
				iaTemp[i][j] =0;
		}
	}

	return iaTemp;
}