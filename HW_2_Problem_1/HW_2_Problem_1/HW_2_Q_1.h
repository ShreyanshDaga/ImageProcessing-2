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

#ifndef HW_2_H
#define HW_2_H

#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<iostream>

#include"HW_2_Q_1.h"
#include"Pixel.h"
#include"Image.h"
#include"Histogram.h"

int PrintMenu();

void Problem_1_A();		
void Problem_1_B_a();		
void Problem_1_B_b();
void Problem_1_C();		
void Problem_1_D_a();		
void Problem_1_D_b();		
void Problem_1_E();

void Problem_2_A();
void Problem_2_B();
void Problem_2_C();
void Problem_2_D();

void Problem_3_A();
void Problem_3_B();
void Problem_3_C();

void DeNoiseMedian(Image *pimImage,int iWinSize);
void DeNoiseNLM(Image *pimImage);
float** LoG(Image *pimImage,int iWinSize,Image *pimLapMap);
void DeNoiseGaussian(Image *pimImage, int sigma);
bool ConvertToBMP(char* szPathName, unsigned char** lpBits, int w, int h);

int HitMiss_S_Bond_1(int iaHitWin[3][3]);
int HitMiss_S_Bond_2(int iaHitWin[3][3]);
int HitMiss_S_Bond_3(int iaHitWin[3][3]);
int HitMiss_TK_Bond_4(int iaHitWin[3][3]);
int HitMiss_STK_Bond_4(int iaHitWin[3][3]);
int HitMiss_ST_Bond_5(int iaHitWin[3][3]);
int HitMiss_ST_Bond_6(int iaHitWin[3][3]);
int HitMiss_STK_Bond_6(int iaHitWin[3][3]);
int HitMiss_STK_Bond_7(int iaHitWin[3][3]);
int HitMiss_STK_Bond_8(int iaHitWin[3][3]);
int HitMiss_STK_Bond_9(int iaHitWin[3][3]);
int HitMiss_STK_Bond_10(int iaHitWin[3][3]);
int HitMiss_K_Bond_11(int iaHitWin[3][3]);
int UnMask_ST_1(int iaMwin[3][3]);
int UnMask_ST_2(int iaMwin[3][3]);
int UnMask_ST_3(int iaMwin[3][3]);
int UnMask_ST_4(int iaMwin[3][3]);
int UnMask_ST_5(int iaMwin[3][3]);
int UnMask_ST_6(int iaMwin[3][3]);
int UnMask_ST_7(int iaMwin[3][3]);
int UnMask_ST_8(int iaMwin[3][3]);
int UnMask_ST_9(int iaMwin[3][3]);
int UnMask_K_1(int iaMwin[3][3]);
int UnMask_K_2(int iaMwin[3][3]);
int UnMask_K_3(int iaMwin[3][3]);
int UnMask_K_4(int iaMwin[3][3]);
int UnMask_K_5(int iaMwin[3][3]);
int UnMask_K_6(int iaMwin[3][3]);
int UnMask_K_7(int iaMwin[3][3]);


#endif