READ ME

1. Description

This program is a image fluid dripping effect tool which can be used to generate a line on the image and start to drip like fluid.
The project includes 5 files, which are 'my.h', 'project.cpp', ‘Makefile’, ‘README.txt' and a test image 'oceantest.png'. 
To compile on linux, use the Makefile that is included in the folder of this program.

Author: Siqi Wu
Email:  siqiw@clemson.edu 
Data:   12/01/2015

2. Data Structure And Methods

1). Object-Oriented Data Structure
In this program, a class is built for image. The class includes the basic information and some manipulation functions of the image.

Class Name: MyImage
Basic Information:
char *filename;
int    height;
int    width;
int    channel;
int    ochannel;
int    oheight;
int    owidth;
float* data;
float* originaldata;

Manipulation Functions:
void 	    SetFilename(char *newname){ filename = newname;}
void	    SetOriginalData ();
void 	    ImageRead();
void        ImageWrite(char *);
float*      ImageGetData(){ return data; }
float*      GetOriginalData(){ return originaldata; }
int	    GetHeight(){ return height; }
int	    GetOHeight(){ return height; }
int 	    GetWidth(){ return width; }
int	    GetOWidth(){ return owidth; }
int	    GetChannel(){ return channel; }
int	    GetOChannel(){return ochannel;}
char* 	    GetFilename(){ return filename; }
void	    SetHeight( int seth ){ height = seth; }
void	    SetWidth( int setw ){ width = setw; }
void	    SetData( float* newdata){data = newdata;}
void	    SetOHeight( int seth ){ oheight = seth; }
void	    SetOWidth( int setw ){ owidth = setw; }
void	    SetOChannel( int setc ){ ochannel = setc; }
void	    SetChannel( int setc ){ channel = setc; }
void 	    ImageDisplay();

3. Introduction of Functions in file 'project.cpp'
void	RenderScene()
void	Twirl(float, float, float)
void	Transform(float*, int, int, int, int, int, int, int, float*, Matrix3x3)
void	process_input(Matrix3x3)
void	mouseClick(int btn, int state, int x, int y)
void    handleKey( unsigned char key, int x, int y)


4. User Instructions
1). Command Line Arguments
This program is able to accept 2 command line parameters. The first one is input image name, which is required. The second one is file name to write the image, which is optional. When a write filename is not specified, users are not allowed to write the image.
Usage: ./project [input filename]([writename]).

2). Input Parameter
After running the program, users are asked to input a speed control parameter. This parameter controls the wave of the dripping, including the speed and the shape. The parameter is restrict to -3 to 3.5, as when it falls outside of the range, some parts of the image would go up instead of drip down. When the parameter is 0, it has the least strength of curve. When the absolute value of it gets larger, it has a stronger curve and drips faster.

3). Mouse Manipulation
User can use the mouse to click on the image to generate a initial line at the clicked point. And the part of image that is above is line would drip.
Each time the user clicks on the image, it starts a new dripping process. If the last dripping process is not done, it would stop at the current position and starts another process on top of it using the original image. 

4). Keyboard Manipulation 
Affine Warp
I. Press 's' or 'S' to start a new dripping process from the initialize position(vertically top 40% of the image) if the image is not yet being clicked. If the dripping process is currently stopped, using 's' or 'S' can make it continue to drip.
II. Press 'p' or 'P' to stop the current dripping process.
III. Press 'w' or 'W' to write the displaying image as the file name of the second argument (if the second argument is specified).
IV. Press 'o' or 'O' to display the original image
V. Press 'q' or 'Q' to quit the program.

