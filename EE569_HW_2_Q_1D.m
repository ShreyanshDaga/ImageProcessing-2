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

movieObj1 = VideoWriter('Movie1.avi');
movieObj1.FrameRate = 10;
open(movieObj1);

for i = 1:20
    str = int2str(i-1);
    szName = strcat('Harry_Tran_F_',str,'.raw');
    
    C = ReadRGBRaw(szName,256,256);   
    imshow(C);
    frame = getframe;
    writeVideo(movieObj1,frame);
end

for i = 1:20
    str = int2str(i-1);
    szName = strcat('Lena_Tran_F_',str,'.raw');

    C = ReadRGBRaw(szName,256,256);
    imshow(C);
    frame = getframe;   
    writeVideo(movieObj1,frame);
end

for i = 1:20
    str = int2str(i-1);
    szName = strcat('Naruto_Tran_F_',str,'.raw');
    
    C = ReadRGBRaw(szName,256,256);
    imshow(C);
    frame = getframe;   
    writeVideo(movieObj1,frame);
end

for i = 1:64
    str = int2str(i-1);
    szName = strcat('Harry_Slide_TD_F_',str,'.raw');
    
    C = ReadRGBRaw(szName,256,256);   
    imshow(C);
    frame = getframe;
    writeVideo(movieObj1,frame);
end

for i = 1:64
    str = int2str(i-1);
    szName = strcat('Lena_Slide_TD_F_',str,'.raw');

    C = ReadRGBRaw(szName,256,256);
    imshow(C);
    frame = getframe;   
    writeVideo(movieObj1,frame);
end

for i = 1:64
    str = int2str(i-1);
    szName = strcat('Naruto_Slide_TD_F_',str,'.raw');
    
    C = ReadRGBRaw(szName,256,256);
    imshow(C);
    frame = getframe;   
    writeVideo(movieObj1,frame);
end

for i = 1:64
    str = int2str(i-1);
    szName = strcat('Harry_Slide_LR_F_',str,'.raw');
    
    C = ReadRGBRaw(szName,256,256);   
    imshow(C);
    frame = getframe;
    writeVideo(movieObj1,frame);
end

for i = 1:64
    str = int2str(i-1);
    szName = strcat('Lena_Slide_LR_F_',str,'.raw');

    C = ReadRGBRaw(szName,256,256);
    imshow(C);
    frame = getframe;   
    writeVideo(movieObj1,frame);
end

for i = 1:64
    str = int2str(i-1);
    szName = strcat('Naruto_Slide_LR_F_',str,'.raw');
    
    C = ReadRGBRaw(szName,256,256);
    imshow(C);
    frame = getframe;   
    writeVideo(movieObj1,frame);
end

close(movieObj1);
disp(movieObj1);