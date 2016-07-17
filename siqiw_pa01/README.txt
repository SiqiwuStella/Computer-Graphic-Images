*********************************************************************
Discription:

This program is image tool which can be used to read a image and manipulate this image. The manipulation includes horizontally fliping, vertically fiping, red channel greyscale image displaying, green channel greyscale image displaying, blue channel greyscale image displaying returning to the original image and image writing. These functions are implemented through keyboard. When the window of the image is adjusted and larger than the image, the image is displayed in the middle of the window. And there is another mouse function that can output the coodinates and value of the pixel on click on the left botton.

The program includes 4 files, which are 'my.h', 'oiioview.cpp', 'Makefile' and 'Readme.txt'.

To compile on linux, use the Makefile that is included in the folder of this program.

Author: Siqi Wu
Email:  siqiw@clemson.edu 
Data:   09/06/2015

*********************************************************************
Data Structure

1). Object-Oriented Data Structure
In this program, a class is bulit for image. The class includes the bsic information and some manipulation functions of the image.

Class Name: MyImage
Basic Information:
	char *filename;
    	 int height;
    	 int width;
    	 int channel;
    	 unsigned char *data;
    	 unsigned char *originaldata;

Manipulation Functions:
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

2. Function in file 'oiioview.cpp'
     RenderScene()
     handleKey()
     mouseClick()

*********************************************************************
Functions that Implements the Requirements

Basic Requirements
1). Image Display
Use MyImage::ImageDisplay() and RenderScene() to Display Image.

2). Reading Images
Use MyImage::ImageRead() to read image.

3). Writing Images
Use MyImage::ImageWrite() to write image.

4). Image Manipulation
Use MyImage::ImageHorizontal() to get the horizontally flipped image and use the Display module to display the image.

5). Command Line Behavior
The program is able to read two command line arguments. The part that controls the command line behavior is in main() function.

6). Interaction
Use handleKey() to implement the keyboard interaction and use mouseClick() function to implement the mouse click behavior.

Advanced Requirements
1). Use MyImage::ImageRed(), MyImage::ImageGreen(), MyImage::ImageBlue() and the Display module to display the Red, Green and Blue channel greyscale image. Use MyImage::SetOriginalData() to read the original data again. And use MyImage::GetOriginalData() to get unsigned character *originaldata of the image object and display the original image through the Display module.

2). Use MyImage::ImageVertical() to get the vertically flipped image and use the Display module to display. As there is a MyImage::SetData() function call, the flipped image will be stored correctly when MyImage::ImageWrite() is called.

3)*. When the display window is resized and larger than the image, the image would display in the middle of the window. But if the image is smaller than the window, it is not scaled down. (Known bug: When the resize operation is not from the lower right corner, it won't work.)

*********************************************************************
Instructions for Use

1). Command Line Arguments
This program is able to accept two command line arguments. The first one is the file name of the image to read, which is required. The second one is the file name when users want to write the image. This argument is optional. When the second argument is empty, users cannot write the image. 
Example 1: ./oiioview teapot.jpg
Example 2: ./oiioview teapot.jpg teapot1.jpg

2). Keyboard Manipulation 
I.   Press 'i' or 'I' to flip the image vertically.
II.  Press 'f' or 'F' to flip the image horizonally.
III. Press 'o' or 'O' to display the original image.
IV.  Press 'r' or 'R' to display the red channel greyscale image.
V.   Press 'g' or 'G' to display the green channel greyscale image. 
VI.  Press 'b' or 'B' to display the blue channel greyscale image.
VII. Press 'w' or 'W' to write the displaying image as the file name of the second argument.
VIII.Press 'q' or 'Q' to quit the program.

3). Mouse Manipulation
When click on the display window, output the coordinates of the clicked pixel and the R, G, B value of this pixel using 0~255.  
