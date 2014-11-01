IDE Used: Visual Studio 2012 Professional
OS: Widows 8/ Windows 7
Language: C++

The source code contains the following files
*Root Folder Name
HW_2_Problem_1		--Though the name says problem 1 all the problems are in it

*Headers Files:
HW_2_Q_1.h			--COntains prototype of genera fucntions used
Pixel.h				--Contains decln of Pixel class	
Image.h				--Contains decln of Image class
Histogram.h			--Contains decln of Histogram class

*Source Files:
Source.cpp			--Contains the main() and the start point for the program
Pixel.cpp			--Contains class functions
Image.cpp			--Contains class functions
Histogram.cpp			--Contains class functions
DigitalHalftoning.cpp		--Contains functions for Problem 3
MorphologicalProcessing.cpp	--COntains functions for Prolem 2
SpecialEffectImageFilter.cpp	--Contains functions for Problem 1


*All files are required to compile the program

*Output will be in the form of a console window with a main menu 

*Steps to compile in Visual Studio
  Build Menu -> Build Solution		(F6)

*Steps to run in Visual Studio
  Debug Menu -> Start without debugging (CTRL + F5)


*Running the program

1) Few Functions will ask the user to input image file info again, please consider. Unable to modify completely due to deadline constrains.
2) All the images should be in the root folder of the executable

for ex

HW2
|->(All the cpp files)
|->(All the header files)
|->(All the image files)

Also the output image files will be generated in the root directorty itself.
Some functions may require user to input the image info of few output images as well, again same reason please consider.

The Problems which i was unable to solve have empty functions

The naming convention used is a modified hungarian notation i have developed for myself

eg,

Pixel pxP;
Image imOcean, imBarara[i];
int iRows, iCols;

The small case letters in each variable name point to the type of the variable, this should make it easy to read the code as i have had very little time to comments
again please consider.


The program will display a lot of information like histograms and images
some problems ask to enter the threshold from histogram, this time i have mentioned it there and then what to enter rather than for the grader to open up the histogram and
check for the threshold or anything else
However is anything is a miss please consider the immidieate previous statement in the output to see the requires action

Threshold input information as arguments from user

problem 1B given in the report 
problem 1C given in the report
problem 1E given in the report (10)

problem 2A,2B,2C mentioned in the code itself
problem 3A,3B,3C Mentioned in the code itself

**MATLAB FILES**
ReadRGBRaw.m			--Code to read rgb raw image
EE569_HW_2_Q_1D.m		--Code to create movie files

name of movie file generates Movie1.avi

all the effects are in the movie file itself

you need to copy all the frames generated from the c++ program into the matalab folder so that they can be read using ReadRGBRaw.m function

sliding effect will generate 64 frames each
transition effect will generate 20 frames each