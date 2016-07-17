/*Input RGB color primary values : r , g, and b on scale 0
255
Output HSV colors : h on scale 0
360, s and v on scale 0
1
*/
#include <OpenImageIO/imageio.h>
#include <string>
OIIO_NAMESPACE_USING
//#include <typeinfo>
#include <iostream>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include<GL/glut.h>
#endif

#include "my.h"
//global variable image filename and filename to write this image

static char* writename = NULL, * filename = NULL;
static double tl = 110, th = 145, ths = 0.2, thv = 0.2;
static int pvtag = 0, sstag = 0;
static float k = 2.13, ks = 2;


//global Object Image
MyImage Image;

void Reshape(int current_w, int current_h){
  
}

void RenderScene(){
  Image.ImageDisplay();
}

void mask()
{
      if ((pvtag == 0)&&(sstag == 1))
      {
        unsigned char *hsv, *ss;
        hsv = Image.Threshold(tl,th,thv,ths); 
        Image.SetData(hsv);
	ss = Image.SS(ks);
	Image.SetData(ss);
      }
      if ((pvtag == 1)&&(sstag == 0))
      {
        unsigned char *pv;
        pv = Image.PV(k); 
        Image.SetData(pv);
      }
      if ((pvtag == 0)&&(sstag == 0))
      {
        unsigned char *hsv;
        hsv = Image.Threshold(tl,th,thv,ths); 
        Image.SetData(hsv);
      }
      if ((pvtag == 1)&&(sstag == 1))
      {
        unsigned char *pv, *ss;
        pv = Image.PV(k); 
        Image.SetData(pv);
	ss = Image.SS(ks);
	Image.SetData(ss);
      }
     // glutPostRedisplay();
}

void kfunc(int key, int x, int y)
{ 
    switch(key)
    {
	case GLUT_KEY_UP: 
	if (pvtag == 1)
	{
	  k+=0.01;
	  std::cout<<"k is " <<k<<std::endl;
	  mask();
	  glutPostRedisplay();
	}
	break;

        case GLUT_KEY_DOWN:
	if (pvtag == 1)
	{
	  k-=0.01;
	  std::cout<<"k is " <<k<<std::endl;
	  mask();
	  glutPostRedisplay();
	}
	break;

	case GLUT_KEY_F1: 
	if (pvtag == 0)
	{
	  th-=5;
	  std::cout<<"th is " <<th<<std::endl;
	  mask();
	  glutPostRedisplay();
	}
	break;

        case GLUT_KEY_F2:
	if (pvtag == 0)
	{
	  th+=5;
	  std::cout<<"th is " <<th<<std::endl;
	  mask();
	  glutPostRedisplay();
	}
	break;

	case GLUT_KEY_F3: 
	if (pvtag == 0)
	{
	  tl-=5;
	  std::cout<<"tl is " <<tl<<std::endl;
	  mask();
	  glutPostRedisplay();
	}
	break;

        case GLUT_KEY_F4:
	if (pvtag == 0)
	{
	  tl+=5;
	  std::cout<<"tl is " <<tl<<std::endl;
	  mask();
	  glutPostRedisplay();
	}
	break;

	case GLUT_KEY_F5: 
	if (pvtag == 0)
	{
	  ths-=0.02;
	  std::cout<<"ths is " <<ths<<std::endl;
	  mask();
	  glutPostRedisplay();
	}
	break;

        case GLUT_KEY_F6:
	if (pvtag == 0)
	{
	  ths+=0.02;
	  std::cout<<"ths is " <<ths<<std::endl;
	  mask();
	  glutPostRedisplay();
	}
	break;

        case GLUT_KEY_F7:
	if (sstag == 1)
	{
	  ks-=0.1;
	  std::cout<<"ks is " <<ks<<std::endl;
	  mask();
	  glutPostRedisplay();
	}
	break;

        case GLUT_KEY_F8:
	if (sstag == 1)
	{
	  ks+=0.1;
	  std::cout<<"ks is " <<ks<<std::endl;
	  mask();
	  glutPostRedisplay();
	}
	break;

	case GLUT_KEY_LEFT: 
	if (pvtag == 0)
	{
	  thv-=0.02;
	  std::cout<<"thv is " <<thv<<std::endl;
	  mask();
	  glutPostRedisplay();
	}
	break;

        case GLUT_KEY_RIGHT:
	if (pvtag == 0)
	{
	  thv+=0.02;
	  std::cout<<"thv is " <<thv<<std::endl;
	  mask();
	  glutPostRedisplay();
	}
	break;	
	
    }
 
}
//keyboard handle function to implement image manipulation
void handleKey(unsigned char key, int x, int y){
  switch(key){


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
   std::cout<<"argc"<<argc<<std::endl;
   if (argc < 2 || argc > 5) {
      std::cerr << "Usage: " << argv[0] << " [filename][writename][option] " << std::endl;
      exit(1);
   }
   //if second parameter is specified, store the file name to write the image
   else if (argc == 3)
   {
     std::string arg = argv[2];
     if (arg == "-pv"){ pvtag = 1; std::cout << "pvtag set." << std::endl; }
     else if (arg == "-ss"){ sstag = 1; std::cout << "sstag set." << std::endl;}
     else
     {
       writename = argv[2];
       std::cout << "writename is " << writename << std::endl;
     }
   }
   
   else if (argc == 4)
   {
     std::string arg = argv[3];
     std::string arg2 = argv[2];
     if (arg == "-pv" && arg2 != "-ss")
     { 
	pvtag = 1;        
	writename = argv[2];
        std::cout << "writename is " << writename << std::endl;
        std::cout << "pvtag is set." << std::endl;
     }	
     else if (arg == "-ss" && arg2 != "-pv")
     { 
	sstag = 1;       
	writename = argv[2]; 
        std::cout << "writename is " << writename << std::endl;
	std::cout << "sstag is set." << std::endl;
     }
     else if ((arg == "-ss" && arg2 == "-pv") || (arg == "-pv" && arg2 == "-ss")) 
     { 
	sstag = 1;
	pvtag = 1;       
	std::cout << "sstag is set." << std::endl;
	std::cout << "pvtag is set." << std::endl;
     }
     else
     {
        std::cerr << "Usage: " << argv[0] << " [filename][writename][option] " << std::endl;
        exit(1);
     }
   }
   else //argc == 5
   {
     std::string arg1 = argv[3], arg2 = argv[4];
     if (((arg1 == "-pv") && (arg2 == "-ss"))||((arg1 == "-ss") && (arg2 == "-pv")))
     {
	pvtag = 1;
	sstag = 1;
	writename = argv[2]; 
        std::cout << "writename is " << writename << std::endl;
	std::cout << "sstag and pvtag are set." << std::endl;
     }
   }

   //get the filename of the image, set the filename and read the image to get basic information of it
   filename = argv[1];
   std::cout << filename << std::endl;
   Image.SetFilename(filename);
   Image.ImageRead();
   Image.SetOriginalData();
   int h = Image.GetHeight();
   int w = Image.GetWidth();
   int c = Image.GetChannel();
   mask();
   //start up the glut utilities
   glutInit(&argc, argv);
   
   //create the graphics window, giving width, height and title text
   glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH | GLUT_RGBA);
   glutInitWindowSize((Image.GetWidth()),Image.GetHeight());
   glutCreateWindow("Image");

   //glut keyboard function, display function and mouse function
   glutKeyboardFunc(handleKey);
   glutDisplayFunc(RenderScene);
   glutSpecialFunc(kfunc);
   //use glblend to display the alpha range 0~255 only in this program
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glBlendEquation(GL_FUNC_ADD);
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
