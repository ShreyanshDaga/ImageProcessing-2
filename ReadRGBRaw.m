%%
%EE 569 Homework #2
%Date:		October 13, 2013
%Name:		Shreyansh Dnyanesh Daga
%ID:				6375-3348-33	
%Email:		sdaga@usc.edu
%
%Compiled and build on Matlab 2010
%OS: Windows 8
%File: ReadRGBRaw.m
%%

function C = ReadRGBRaw(szFileName,iRows,iCols)

id = fopen(szFileName, 'r');

if(id == -1)
    disp('Can Not Open File !!');
    pause;
end

x = fread(id,inf,'uint8')'; 
x = x/255;

fclose(id);

width = iCols;
height = iRows;

N = width*height;

ind1 = 1:3:3*N;
ind2 = 2:3:3*N+1;
ind3 = 3:3:3*N+2;

Ir = x(ind1);
Ig = x(ind2);
Ib = x(ind3);

Ir1 = reshape(Ir,width,height);
Ig1 = reshape(Ig,width,height);
Ib1 = reshape(Ib,width,height);

figure(1); clf;
C = zeros(height,width,3);

C(:,:,1)=Ir1';
C(:,:,2)=Ig1';
C(:,:,3)=Ib1';

clear x;
clear id;

imagesc(C);