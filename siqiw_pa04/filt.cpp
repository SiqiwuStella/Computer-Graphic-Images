/*
  This is the main cpp file for program 'filt'.

  Author: Siqi Wu
  Email: siqiw@clemson.edu
  Date: 10/15/2015
*/

#include <OpenImageIO/imageio.h>
#include <string>
#include <cmath>
OIIO_NAMESPACE_USING
//#include <typeinfo>
#include <iostream>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include<GL/glut.h>
#endif

#include <fstream>
#include "my.h"
//global variable image filename, kernelname and filename to write this image
static char *writename = NULL, *filename = NULL, *kernelname = NULL;
//global variable for size number of kernel and tag to check if it is garbor filter
static int kernelnum=0, gbtag=0;
static float theta, sigma, period;
const float PI = 3.141592653;//define PI

//global Object Image
MyImage Image;

//display function
void RenderScene(){
  Image.ImageDisplay();
}

//Function to read kernel file. Input kernel file name, output a 2D array of float for the kernel 
float** kernel(char* file){
  int num, i, j;
  float **kernel; 
  float k;
  std::ifstream inFile;
  inFile.open(file);

  //check for error
  if (inFile.fail()){
     std::cerr << "Error Opening File" << std::endl;
     exit(1);
  }
    inFile >> kernelnum;
  std::cout << kernelnum << std::endl;
  kernel = new float*[kernelnum];
  for (i = 0; i < kernelnum; i++)
  {
     kernel[i] = new float[kernelnum];
  }

  for (i = 0; i < kernelnum; i++)
  {
    for (j = 0; j < kernelnum; j++)
    {
        inFile >> kernel[i][j];
        //std::cout << kernel[i][j] << std::endl;      
    }
  }
  inFile.close();
  return kernel;
}

//Function to generate a garbor filter. Input theta, sigma and period, output a 2D array of float for the garbor filter based on these parameters
float** garbor(float theta, float sigma, float period)
{
   int i, x, y;
   float xp,yp;
   float **filt;
   unsigned char* gar = new unsigned char[(int)sigma*2+1];
   filt = new float*[(int)sigma*2+1];
   for (i = 0; i <(int)sigma*2+1; i++)
   {
      filt[i] = new float[(int)sigma*2+1];
   }
   for (x=-(int)sigma; x<=(int)sigma; x++)
   {
     for (y=-(int)sigma; y<=(int)sigma; y++)
     {
	xp=float(x*cos(theta)+(y*sin(theta)));
	yp=float(-x*sin(theta)+(y*cos(theta)));	
	filt[y+(int)sigma][x+(int)sigma] = exp(-(xp*xp+yp*yp)/(2*sigma*sigma))*cos(2*PI*xp/period);
     }
   }

   return filt;
}

//Function to implement convolution. Input an image object, kernel array and num of the size of the kernel, output an image of the convolution result.
unsigned char * convolve(MyImage image, float** kernel, int num)
{
   int i, j, r, c, total;
   float count, countneg, scale;
   //initialize minimum value and maximum value in three channel
   float maxr = -100000, minr = 100000, 
	 maxg = -100000, ming = 100000,
	 maxb = -100000, minb = 100000,
         omaxr = -100000, ominr = 100000, 
	 omaxg = -100000, oming = 100000,
	 omaxb = -100000, ominb = 100000;
  unsigned char* data = image.ImageGetData();
   int height = image.GetHeight();
   int width = image.GetWidth();
   int channel = image.GetChannel();
   float k, red[width][height], green[width][height], blue[width][height];
   //If the image is 4 channel, output error.
   if (channel == 4) 
   {
     std::cout << "Does not suppport 4 channel convolution." << std::endl;
     exit(-1);
   }
   total = height*width*channel;
  //assign a 1D array for the result image
   unsigned char* result = new unsigned char[total];
  //assign a float array to store the middle result before normalization
   float *middle = new float[total];
   std::cout << "kernelnumber " << num <<std::endl;
  for (i = 0; i < total; i++)
   {
     //store the image into three seperate channels(if has) using 2D array
     //scale the image values to 0~1
     if((i%(channel))==0)
     {
	//find the original maximum and minimum values in three channels
	red[i/(width*channel)][(i/channel)%width] = (float)data[i]/255;
	if((float)data[i]>omaxr){ omaxr = (float)data[i]; }
	if((float)data[i]<ominr){ ominr = (float)data[i]; }
	if (channel>1)
	{
	  green[i/(width*channel)][(i/channel)%width] = (float)data[i+1]/255;
	  if((float)data[i]>omaxg){ omaxg = (float)data[i]; }
	  if((float)data[i]<oming){ oming = (float)data[i]; }
	  blue[i/(width*channel)][(i/channel)%width] = (float)data[i+2]/255;
  	  if((float)data[i]>omaxb){ omaxb = (float)data[i]; }
	  if((float)data[i]<ominb){ ominb = (float)data[i]; }
	}
     }
   }
   int m=0;
  //assign space for the reflected kernel
  float ** fkernel;
  fkernel = new float*[kernelnum];
  for (i = 0; i < kernelnum; i++)
  {
     fkernel[i] = new float[kernelnum];
  }
  //reflect the kernel
  for (i = 0; i < kernelnum; i++)
  {
    for (j = 0; j < kernelnum; j++)
    {
        fkernel[i][j] = kernel[kernelnum-i-1][kernelnum-j-1];
        //std::cout << kernel[i][j] << std::endl;      
    }
  }

   std::cout << kernelnum << std::endl;

   int p,q;
   for (c = 0; c < width; c++)
   {
     for (r = 0; r < height; r++)
     {
	count = 0;
	countneg = 0;
        float calr=0,calg=0,calb=0,rval,gval,bval;
 	for (i = -num/2; i <=num/2; i++)
        {
	  for (j = -num/2; j <=num/2; j++)
	  {
	    if ((c+i<0)||(c+i>=width)||(r+j<0)||(r+j>=height)) 
            { k=0;rval=0;gval=0;bval=0;}
            else 
            {
              k = fkernel[num/2+i][num/2+j];
              rval = red[c+i][r+j];
              //for three channel images
	      if (channel>1)
	      {
                gval = green[c+i][r+j];
                bval = blue[c+i][r+j];
	      }
	    }
            //calculate the scale factor, use all the values greater than 0
	    if (k>0) {count=count+k;}
	    //if all the effective weights are less than 0, use the negative weights
	    if (k<0) {countneg = countneg+abs(k);}
	    calr = calr + rval*k;
            if (channel>1)
            {
		calg = calg + gval*k;
		calb = calb + bval*k;
            }
	  }
	}
	if (count>0) { scale = count; }
	else { scale = countneg; }
        //scale the convolution result, normalize the image back to 0~255
        calr = calr/scale;
  	//find the maximum and minimum values in image after convolution
        if (calr > maxr) { maxr = calr; }
        if (calr < minr) { minr = calr; }
	middle[m] = calr;
	m++;
        if (channel>1)
	{
	  calg = calg/scale;
          if (calg > maxg) { maxg = calg; }
          if (calg < ming) { ming = calg; }
	  calb = calb/scale;
          if (calb > maxb) { maxb = calb; }
          if (calb < minb) { minb = calb; }
	  middle[m] = calg;
	  m++;
	  middle[m] = calb;
	  m++;
        }
      }
   }
   
   //normalize the image using clamping
   for (m = 0; m < total; m++)
   {
     if (m%channel == 0)
     result[m] = (middle[m]-minr)*(omaxr-ominr)/(maxr-minr);
     if (m%channel == 1)
     result[m] = (middle[m]-ming)*(omaxg-oming)/(maxg-ming);
     if (m%channel == 2)
     result[m] = (middle[m]-minb)*(omaxb-ominb)/(maxb-minb);
   }
   return result;
}
//keyboard handle function to implement image manipulation
void handleKey(unsigned char key, int x, int y){
  switch(key){

      case 'c':
      case 'C':
      unsigned char * convlv;
      float ** filkernel;
      if (gbtag == 1)
      {
	filkernel = garbor(theta, sigma, period);
	kernelnum = (int)sigma*2+1;
      }
      else {filkernel = kernel(kernelname);}
      convlv = convolve(Image, filkernel, kernelnum);
      Image.SetData(convlv);
      glutPostRedisplay();
      break;

//use key 'r' or 'R' to display the original image
      case 'r':
      case 'R':
      unsigned char* origin;
      origin = Image.GetOriginalData();
      Image.SetData(origin);
      glutPostRedisplay(); 
      break;

//use key 'w' or 'W' to write image
      case 'w':	
      case 'W':
      if (writename != NULL)
      {
          Image.ImageWrite(writename);

      }
      //if the second parameter is not specified in command line, output the message 'cannot write the image'
      else{
          std::cout << "You cannot write the image because you didn't specify a filename." << std::endl;
      }
      break;

//use key 'q' or 'Q' to quit the program      
    case 'q':		// q - quit
    case 'Q':
    case 27:		// esc - quit
      exit(0);
      
    default:		// not a valid key -- just ignore it
      return;
  }
}


int main(int argc, char* argv[]) {
//handle the commandline inputs
    std::cout << argc <<std::endl;
    if (argc == 3)
    {
	filename = argv[2];
	kernelname = argv[1];
    }else if (argc == 4)
    {
	filename = argv[2];
	kernelname = argv[1];
	writename = argv[3];
    }
    else if ((argc == 7)&&(strcmp("-g",argv[3])==0))
    {
	gbtag = 1;
	filename = argv[1];
	writename = argv[2];
	theta = atof(argv[4]);
	sigma = atof(argv[5]);
	period = atof(argv[6]);
    }
    else if ((argc == 6)&&(strcmp("-g",argv[2])==0))
    {
	gbtag = 1;
	filename = argv[1];
	theta = atof(argv[3]);
	sigma = atof(argv[4]);
	period = atof(argv[5]);	
   std::cout<<"theta "<<theta<<std::endl;
   std::cout<<"sigma "<<sigma<<std::endl;
   std::cout<<"period "<<period<<std::endl;
    }
   //if invalid input are detected, output usage of the program
    else 
    {
	std::cerr << "Usage: " << argv[0] << " [kernelname][filename]([writename])" << std::endl << "OR " << "Usage " << argv[0] << " [filename]([writename])[-g][theta][sigma][period] " << std::endl;
      exit(1);

    }
   //get the filename of the image, set the filename and read the image to get basic information of it
   std::cout << filename << std::endl;
   Image.SetFilename(filename);
   Image.ImageRead();
   Image.SetOriginalData();
   int h = Image.GetHeight();
   int w = Image.GetWidth();
   int c = Image.GetChannel();

   //start up the glut utilities
   glutInit(&argc, argv);
   
   //create the graphics window, giving width, height and title text
   glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH | GLUT_RGBA);
   glutInitWindowSize((Image.GetWidth()),Image.GetHeight());
   glutCreateWindow("Image");

   //glut keyboard function, display function and mouse function
   glutKeyboardFunc(handleKey);
   glutDisplayFunc(RenderScene);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   //define the drawing coordinate system on the viewport
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0, w, 0, h);
     
  // specify window clear (background) color to be opaque white
   glClearColor(1, 1, 1, 1);

//glut loops
   glutMainLoop();
   return 0;
}
