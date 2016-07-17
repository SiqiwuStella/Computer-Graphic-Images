*********************************************************************
Description:

This program project includes 2 programs, alphamask and composition.

1)Alphamask
This program is a tool to extract the foreground of green screen images. The program accepts 3 channel images and user can store the transformed image as a 4 channel image which contains an alpha channel. The program includes 3 methods, the basic transformation using thresholds in HSV channel, the Petro Vlahos algorithm and the spill suppression method. Users are able to choose between the basic transformation and the Petro Vlahos algorithm with options of the combination of spill suppression. The options are implied through command line parameters. By default, the program uses the basic transformtion.

Description of the three methods:
A. Basic Transformation
This algorithm is implemented using function unsigned char* MyImage::Threshold(double thl, double thh, double thv, double ths). This function read four parameters.These four parameters are the low threshold of H channel, high threshold of H channel, threshold of V channel and threshold of S channel. By default, they are set as thl=110, thh=145, ths=0.2, thv=0.2. User can change these four values using keyboard (See Instructions for Use below). In basic transformation, the alpha value is set using a full range of 0~255. Here we are using a smoother function at the border of the transition. We set range=(thh-thl)*0.2, where thh is the high threshold of the H channel and thl is the low threshold of the H channel. We take 20% of both the high threshold border and the low threshold border as the transition band. Within the transition band, alpha values are scaled to 0~255. Outside of the transition band, alpha values are set to 0 or 255.   

B. Petro Vlahos
This algorithm is implemented using function unsigned char * MyImage::PV(float k). Here we are using a binary alpha mask. When green channel value is greater than k*b(where b is the blue channel value), alpha is set to 0. Otherwise it is set to 255. User are able to change the value k using keyborad (See Instructions for Use below). By default, k is set to 2.13.

C. Spill Surpression
In the spill surpression algorithm, as we are using green screen images, the value we are controlling here is green channel. This algorithm is implemented using function unsigned char * MyImage::SS(float ks). Green channel value is set to be the minimum value of R, G, B values. User are able to change the value of ks using keyboard. By default, ks is set to 2.


The program includes 2 folders (alphamask and composite) and a README.txt file. Floder alphamask contains 3 files, which are 'my.h', 'alphamask.cpp' and 'Makefile'. Folder composite contains 4 files, which are 'my.h', 'composite.cpp', 'Makefile' and 'hulk.png'.

To compile the two programs on linux, use the Makefile that is included in each folder of this program. 

Author: Siqi Wu
Email:  siqiw@clemson.edu 
Date:   09/29/2015

*********************************************************************
Instructions for Use
Note: For the basic transformation of alphamask program, the optimal image is "levinej.jpg" with the default value of the parameters. For the Petro Vlahos algorithm of alphamask program, the optimal image is "hand.jpg" with the default value of the parameters. For the Spill Surpression algorithm of alphamask program, the optimal image is "hand.jpg" with the default value of the parameters. To composite the images, use the background image "hulk.png".

A. alphamask
1). Command Line Arguments
This program is able to accept 2 to 5 command line parameters. 
Usage: [filename][writename][option]. Users are able to choose options of "-pv" and "-ss" separately or together. Besides, users can either specify a write file name or leave it blank. If a write file name is not specified, users are not allowed to write the image.
Example 1: ./alphamask levinej.jpg
Example 2: ./alphamask levinej.jpg -ss
Example 3: ./alphamask levinej.jpg -pv
Example 4: ./alphamask levinej.jpg -pv -ss
Example 5: ./alphamask levinej.jpg levinej.png
Example 6: ./alphamask levinej.jpg levinej.png -ss
Example 7: ./alphamask levinej.jpg levinej.png -pv
Example 8: ./alphamask levinej.jpg levinej.png -pv -ss

2) Keyboard Manipulation
I.   Press 'UP_KEY' to increase k parameter in Petro Vlahos by 0.02.
II.  Press 'DOWN_KEY' to decrease k parameter in Petro Vlahos by 0.02.
III. Press 'F1' to decrease the high threshold of basic transformation by 5.
IV.  Press 'F2' to increase the high threshold of basic transformation by 5.
V.   Press 'F3' to decrease the low threshold of basic transformation by 5. 
VI.  Press 'F4' to increase the low threshold of basic transformation by 5.
VII. Press 'F5' to decrease the S channel threshold of basic transformation by 0.02.
VIII.Press 'F6' to increase the S channel threshold of basic transformation by 0.02.
IX.  Press 'F7' to decrease the ks parameter of Splill Surpression by 0.1.
X.   Press 'F8' to increase the ks parameter of Splill Surpression by 0.1.
XI.  Press 'LEFT_KEY' to decrease the V channel threshold of basic transformation by 0.02.
XII. Press 'RIGHT_KEY' to increase the V channel threshold of basic transformation by 0.02.
XIII.Press 'w' or 'W' to write the displaying image as the specified file name.
XIV.  Press 'q' or 'Q' to quit the program.


B. composition
1). Command Line Arguments
This program is able to accept 3 command line arguments.
Usage: [foreground][background][output]  
Users can either specify a write file name or leave it blank. If a write file name is not specified, users are not allowed to write the image. Foreground image need to be 4 channel. Background image can either be 3 channel or 4 channel. Output is a 4 channel image.
Example 1: ./composite levinej.jpg hulk.png
Example 2: ./composite levinej.jpg hulk.png out.png

2). Keyboard Manipulation 
I. Press 'w' or 'W' to write the displaying image as the specified file name.
II.Press 'q' or 'Q' to quit the program.

*********************************************************************
Data Structure

1). Object-Oriented Data Structure
In this program, a class is bulit for image. The class includes the bsic information and some manipulation functions of the image. This project is using the same 
head file as assignment 1. Therefore, "my.h" still includes functions that implement the manipulations of assignment 1.

Class Name: MyImage
Basic Information:
	char *filename;
    	 int height;
    	 int width;
    	 int channel;
    	 unsigned char *data;
    	 unsigned char *originaldata;

Functions:
	void 	    SetFilename(char *newname){ filename = newname;}
    	void	    SetOriginalData ();
     	void 	    ImageRead();
     	void        ImageWrite(char *);
     unsigned char* ImageGetData(){ return data; }
     unsigned char* GetOriginalData(){ return originaldata; }
     	int	    GetHeight(){ return height; }
     	int 	    GetWidth(){ return width; }
     	int	    GetChannel(){ return channel; }
     	char* 	    GetFilename(){ return filename; }
     	void	    SetHeight( int seth ){ height = seth; }
    	void	    SetWidth( int setw ){ width = setw; }
     	void	    SetData( unsigned char *newdata){data = newdata;}
     	void	    SetChannel( int setc ){ channel = setc; }
     	void 	    ImageDisplay();
     unsigned char* ImageVertical();
     unsigned char* ImageHorizontal();
     unsigned char* ImageRed();
     unsigned char* ImageGreen();
     unsigned char* ImageBlue();
     unsigned char* Threshold(double thl, double thh, double thv, double ths);
     unsigned char* PV(float k);
     unsigned char* SS(float k);


2).Image Data Structure
In this program, images are stored using a 1D array of unsigned characters. The size of the array is width*height*channel. For example, for a 3 channel RGB image, the array stores the image as the red channel value of the first pixel, the blue channel value of the first pixel, the green channel value of the first pixel and so on.

*********************************************************************
Introduction of Functions

1. Class Functions in file 'my.h'
        void        SetFilename(char *newname){ filename = newname;}
    	void	    SetOriginalData ();
     	void 	    ImageRead();
     	void        ImageWrite(char *);
     unsigned char* ImageGetData(){ return data; }
     unsigned char* GetOriginalData(){ return originaldata; }
     	int	    GetHeight(){ return height; }
     	int 	    GetWidth(){ return width; }
     	int	    GetChannel(){ return channel; }
     	char* 	    GetFilename(){ return filename; }
     	void	    SetHeight( int seth ){ height = seth; }
    	void	    SetWidth( int setw ){ width = setw; }
     	void	    SetData( unsigned char *newdata){data = newdata;}
     	void	    SetChannel( int setc ){ channel = setc; }
     	void 	    ImageDisplay();
     unsigned char* ImageVertical();
     unsigned char* ImageHorizontal();
     unsigned char* ImageRed();
     unsigned char* ImageGreen();
     unsigned char* ImageBlue();
     unsigned char* Threshold(double thl, double thh, double thv, double ths);
     unsigned char* PV(float k);
     unsigned char* SS(float k);

2. Function in file 'alphamask.cpp'
     RenderScene()
     kfunc()
     handleKey()
     mask()

3. Function in file 'composite.cpp'
     RenderScene()
     composite()
     handleKey()

*********************************************************************
Functions that Implements the Requirements

Basic Requirements
1). Basic Transformtion of Alphamask
Use MyImage::Threshold() to implement basic transformation and kfunc() and glutKeyboardFunc() to adjust parameters. 

2). Composition
Use composite() to implement composition.

Advance Requirements
1). Petro Vlahos
Use MyImage::PV() and kfunc() to implement Petro Vlahos and adjust the parameters. 

2). Spill Surpression
Use MyImage::SS() and kfunc() to implement Spill Surpression and adjust the parameters. 


