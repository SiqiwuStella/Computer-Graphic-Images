/*
  This file 'tonemap.cpp' is a tone mapping tool for images. There are three 
modes in this program which are three different algorithms of tone mapping. 
Users are able to read a image, choose a mode to tone map it and switch between
the original image and the tone mapped image. User can also write a image if
write filename is specified.

  Author: Siqi Wu
  Email: siqiw@clemson.edu
  Date: 10/27/2015
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
char *writename = NULL, *fname = NULL, *kernelname = NULL;
int kernelnum = 0;
int stag = 0, gflag = 0, cflag = 0, bflag = 0;
float gam, * img = NULL;
MyImage Image;

//display function
void RenderScene(){
  Image.ImageDisplay();
}


void handleKey(unsigned char key, int x, int y){
  switch(key){

      case 's':
      case 'S':
      {
        float * oimg = NULL;
        //switch between original and tone mapped image by pressing 's'
        if (stag % 2 == 0){
	  oimg = Image.GetOriginalData();
	  Image.SetData(oimg);
        }
        if (stag % 2 == 1){
	  Image.SetData(img);
        }
        stag = stag + 1;
        glutPostRedisplay();
      }
      break;


//use key 'w' or 'W' to write image
      case 'w':	
      case 'W':
        {
        if (writename != NULL)
        {
            Image.ImageWrite(writename);
        }
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
   // command line arguments handling
   std::cout << "argument number " << argc <<std::endl;
   float ** ker = NULL;
   int i,j;
   if (argc == 4){
     std::string arg1 = argv[1];
     if (arg1 == "-g"){
       gflag = 1;
       std::cout<<"gflag set"<<std::endl;
       fname = argv[2];
       gam = atof(argv[3]);
     }
     else if (arg1 == "-c"){
       std::cout << "A kernel name must be specified when using '-c'." << std::endl;
       std::cout << "Usage: [flag][filename][gamma]([kernelname])([writename])." << std::endl;
       exit(-1);
     }
     else {
       std::cout << "Usage: [flag][filename][gamma]([kernelname])([writename])." << std::endl;
       exit(-1);
     }
   }
   else if (argc == 5){
     std::string arg1 = argv[1];
     if (arg1 == "-c"){
       std::cout<<"cflag set"<<std::endl;
       cflag = 1;
       fname = argv[2];
       gam = atof(argv[3]);
       kernelname = argv[4];
     }
     else if (arg1 == "-b"){
       std::cout<<"bflag set"<<std::endl;
       cflag = 1;
       bflag = 1;
       fname = argv[2];
       gam = atof(argv[3]);
       kernelname = argv[4];
     }
     else if (arg1 == "-g"){
       gflag = 1;
       fname = argv[2];
       gam = atof(argv[3]);
       writename = argv[4];
       std::cout << "g flag set." <<std::endl;
       std::cout << "write name is " << writename << std::endl;
     }
     else{
       std::cout << "Usage: [flag][filename][gamma]([kernelname])([writename])." << std::endl;
       exit(-1);
     }
   }
   else if (argc == 6){
     std::string arg1 = argv[1];
     if (arg1 == "-c"){
	cflag = 1;
	fname = argv[2];
	gam = atof(argv[3]);
	kernelname = argv[4];
	writename = argv[5];
     }
     else if (arg1 == "-b"){
	cflag = 1;
	bflag = 1;//-b mode
	fname = argv[2];
	gam = atof(argv[3]);
	kernelname = argv[4];
	writename = argv[5];
     }
     else{
	std::cout << "Usage: [flag][filename][gamma]([kernelname])([writename])." << std::endl;
        exit(-1);	
     }
   }
   else{
     std::cout << "Usage: [flag][filename][gamma]([kernelname])([writename])." << std::endl;
     exit(-1);
   }
   std::cout<< fname << std::endl;
   Image.SetFilename(fname);
   //read image
   Image.ImageRead();
   float* data = Image.ImageGetData();
   int wid = Image.GetWidth();
   int hei = Image.GetHeight();
   int ch  = Image.GetChannel();
   Image.SetOriginalData();
   std::cout << kernelname <<std::endl;

   // if "-g" mode, implement simple tone mapping
   if (gflag == 1){
     img = Image.simpleTM(gam);
   }
   else if (cflag == 1){
     // read kernel file
     std::ifstream inFile;
     inFile.open(kernelname);

     //check for error
     if (inFile.fail()){
        std::cerr << "Error Opening File" << std::endl;
        exit(1);
     }
     inFile >> kernelnum;
     std::cout << "kernel number " << kernelnum << std::endl;
     ker = new float*[kernelnum];
     for (i = 0; i < kernelnum; i++){
       ker[i] = new float[kernelnum];
     }
     for (i = 0; i < kernelnum; i++){
       for (j = 0; j < kernelnum; j++){
          inFile >> ker[i][j];    
       }
     }
     inFile.close();
     //after reading kernel
     //implement tone mapping with convolution 
     //passing bflag which determines to do bilateral filter or not
     img = Image.convolveTM(ker, kernelnum, gam, bflag);
     }else{
     std::cout << "You didn't specify a mode." << std::endl;
     exit(-1);
     }

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
   gluOrtho2D(0, wid, 0, hei);
     
  // specify window clear (background) color to be opaque white
   glClearColor(1, 1, 1, 1);

//glut loops
   glutMainLoop();
   delete data; 
   for(i = 0; i < kernelnum; i++){
     delete[]ker[i];
   }
   delete[]ker;
   return 0;
}
