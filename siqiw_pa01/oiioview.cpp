/*
  Program to display image and implement image manipulation. For detailed instruction, see README.txt.
 
  Author: Siqi Wu
  Email: siqiw@clemson.edu
  Date: 09/06/2015
*/

#include <OpenImageIO/imageio.h>
OIIO_NAMESPACE_USING

#include <iostream>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include<GL/glut.h>
#endif

#include "my.h"

//global variable image filename and filename to write this image
static char* writename = NULL, * filename = NULL;

//global Object Image
MyImage Image;

//To encapsulate the member function MyImage::ImageDisplay for glutDisplayFunc() 
void RenderScene(){
   Image.ImageDisplay();
}

//mouseclick function to output the coordinates and values of the pixel that is clicked on
void mouseClick(int btn, int state, int x, int y)
{
   unsigned char pixel[3];
   int wid  = Image.GetWidth();
   int hei  = Image.GetHeight();
   int chnl = Image.GetChannel();
   int i    = (x+y*wid)*chnl;
   if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        glReadPixels(x,glutGet(GLUT_WINDOW_HEIGHT)-y,1,1,GL_RGB,GL_UNSIGNED_BYTE,&pixel[0]);
	std::cout << "Position: x=" << x << ", y=" << y << "  Color: " << "R: " <<  static_cast<int>(pixel[0]) << "  G: " <<  static_cast<int>(pixel[1]) << "  B: " <<  static_cast<int>(pixel[2]) << std::endl;
    }
}

//keyboard handle function to implement image manipulation
void handleKey(unsigned char key, int x, int y){

  switch(key){

//use key 'i' or 'I' to flip the image vertically
    case 'i':
    case 'I':
      unsigned char *verticalImage;
      verticalImage = Image.ImageVertical();
      Image.SetData(verticalImage);
      glutPostRedisplay();
    break;
    
//use key 'f' or 'F' to flip the image horizontally 
    case 'f':
    case 'F':
      unsigned char *horizontalImage;
      horizontalImage = Image.ImageHorizontal();
      Image.SetData(horizontalImage);
      glutPostRedisplay();
    break;

//use key 'o' or 'O' to display original image
    case 'o':
    case 'O':
      unsigned char *originalImage;
      Image.SetOriginalData(); 
      originalImage = Image.GetOriginalData();
      Image.SetData(originalImage);
      glutPostRedisplay();
    break;
 
//use key 'r' or 'R' to display red channel greyscale image    
    case 'r':		
    case 'R':
      unsigned char *red;
      Image.SetOriginalData(); 
      originalImage = Image.GetOriginalData();
      Image.SetData(originalImage);
      red = Image.ImageRed();
      Image.SetData(red);
      glutPostRedisplay();
    break;

//use key 'g' or 'G' to display green channel greyscale image
    case 'g':		
    case 'G':
      unsigned char *green;
      Image.SetOriginalData(); 
      originalImage = Image.GetOriginalData();
      Image.SetData(originalImage);
      green = Image.ImageGreen();
      Image.SetData(green);
      glutPostRedisplay();
    break;

//use key 'b' or 'B' to display blue channel greyscale image
    case 'b':		
    case 'B':
      unsigned char *blue;
      Image.SetOriginalData(); 
      originalImage = Image.GetOriginalData();
      Image.SetData(originalImage);
      blue = Image.ImageBlue();
      Image.SetData(blue);
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



int main(int argc, char** argv) {

   //if neither one or two parameters are input in command line, output the instruction of the usage of the program
   if (argc < 2 || argc > 3) {
      std::cerr << "Usage: " << argv[0] << " [filename][writename] " << std::endl;
      exit(1);
   }

   //if second parameter is specified, store the file name to write the image
   else if (argc == 3)
   {
     writename = argv[2];
     std::cout << "writename is" << writename << std::endl;
   }

   //get the filename of the image, set the filename and read the image to get basic information of it
   filename = argv[1];
   std::cout << filename << std::endl;
   Image.SetFilename(filename);
   Image.ImageRead();
   int h = Image.GetHeight();
   int w = Image.GetWidth();
   int c = Image.GetChannel();

   //start up the glut utilities
   glutInit(&argc, argv);
   
   //create the graphics window, giving width, height and title text
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
   glutInitWindowSize((Image.GetWidth()),Image.GetHeight());
   glutCreateWindow("Image");

   //glut keyboard function, display function and mouse function
   glutKeyboardFunc(handleKey);
   glutDisplayFunc(RenderScene);
   glutMouseFunc(mouseClick);

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
