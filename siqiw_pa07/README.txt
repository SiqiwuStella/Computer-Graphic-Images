I. Description

This program is a image wrapping tool  which can be used to implement several kinds of affine warp including translation, rotation, scaling, shearing and twirling. 
The program includes 10 files, which are 'my.h', 'warper.cpp', ‘Makefile’, and ‘README.txt', 'Vector.cpp', 'Vector.h', 'Matrix.cpp', 'Matrix.h', 'Utility.cpp', 'Utility.h'. 
To compile on linux, use the Makefile that is included in the folder of this program.

Author: Siqi Wu
Email:  siqiw@clemson.edu 
Data:   11/24/2015

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
int			GetHeight(){ return height; }
int			GetOHeight(){ return height; }
int 	    GetWidth(){ return width; }
int			GetOWidth(){ return owidth; }
int			GetChannel(){ return channel; }
int			GetOChannel(){return ochannel;}
char* 	    GetFilename(){ return filename; }
void	    SetHeight( int seth ){ height = seth; }
void	    SetWidth( int setw ){ width = setw; }
void	    SetData( float* newdata){data = newdata;}
void	    SetOHeight( int seth ){ oheight = seth; }
void	    SetOWidth( int setw ){ owidth = setw; }
void	    SetOChannel( int setc ){ ochannel = setc; }
void	    SetChannel( int setc ){ channel = setc; }
void 	    ImageDisplay();

3. Introduction of Functions

1). Functions in file 'my.h'
void 	    SetFilename(char *newname){ filename = newname;}
void	    SetOriginalData ();
void 	    ImageRead();
void        ImageWrite(char *);
float*      ImageGetData(){ return data; }
float*      GetOriginalData(){ return originaldata; }
int			GetHeight(){ return height; }
int			GetOHeight(){ return height; }
int 	    GetWidth(){ return width; }
int			GetOWidth(){ return owidth; }
int			GetChannel(){ return channel; }
int			GetOChannel(){return ochannel;}
char* 	    GetFilename(){ return filename; }
void	    SetHeight( int seth ){ height = seth; }
void	    SetWidth( int setw ){ width = setw; }
void	    SetData( float* newdata){data = newdata;}
void	    SetOHeight( int seth ){ oheight = seth; }
void	    SetOWidth( int setw ){ owidth = setw; }
void	    SetOChannel( int setc ){ ochannel = setc; }
void	    SetChannel( int setc ){ channel = setc; }
void 	    ImageDisplay();

2). Function in file 'warper.cpp'
void     RenderScene()
void     lowercase(char *)
void     Rotate(Matrix3x3, float)
void     Translation(Matrix3x3, float, float)
void     Shear(Matrix3x3, float, float)
void     Scale(Matrix3x3, float, float)
void     Twirl(float, float, float)
void	 Bulge(float cx, float cy)
void     Transform(float*, int, int, int, int, int, int, int, float*, Matrix3x3)
float	 average(float* img, int wid, int hei, int nw, int nh, int c)
float	 calculate(float* img, int wid, int hei, int nw, int nh, float ratiox, float ratioy, int i)
void	 SuperSample(float* img, int wid, int hei, int nwid, int nhei, int minx, int miny, int chnl, float * output, Matrix3x3 M)
void	 Interpolation(float* img, int wid, int hei, int nwid, int nhei, int minx, int miny, int chnl, float * output, Matrix3x3 M)
void     process_input(Matrix3x3)
void     handleKey( unsigned char key, int x, int y )


4. User Instructions
1). Command Line Arguments
This program is able to accept 2 command line parameters. The first one is input image name, which is required. The second one is file name to write the image, which is optional. When a write filename is not specified, users are not allowed to write the image.
Usage: ./warper [input filename]([writename]).

2). Keyboard Manipulation 
Affine Warp
r	θ	    	 counter clockwise rotation about image origin, θ in degrees
s	sx sy        scale
b   bx by		 bulge
p	px py		 perspective warp
t	dx dy        translate
h 	hx hy        shear
n	s cx xy      nonlinear twirl warp  
II.   Press 'w' or 'W' to write the displaying image as the file name of the second argument.
III.  Press 'r' or 'R' to toggle between the original transformation, bilinear interpolation and supersampling result. 
IV.   Press 'q' or 'Q' to quit the program.


5. Extra Function
Implemented the Bulge effect. User can use 'b' option to bulge the image. bx and by is the coordinates of the center of the bulge.
