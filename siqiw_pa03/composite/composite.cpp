/*Input RGB color primary values : r , g, and b on scale 0
255
Output HSV colors : h on scale 0
360, s and v on scale 0
1
*/
#include <OpenImageIO/imageio.h>

OIIO_NAMESPACE_USING
//#include <typeinfo>
#include <iostream>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include<GL/glut.h>
#endif
//template <class P>
#include "my.h"
#define min(x, y) ((x) < (y)? (x) : (y))
//global variable image filename and filename to write this image

static char* writename = NULL, * filename = NULL, *filename2 = NULL;
//template <class T>

//global Object Image
MyImage Image;
MyImage Image2;
MyImage Imageout;
//MyImage<float> ImgFloat;


void composite(MyImage Imgf, MyImage Imgb)
{
       int widf  = Imgf.GetWidth();
       int heif  = Imgf.GetHeight();
       int chnlf = Imgf.GetChannel();
       if (chnlf != 4)
       {
	 std::cout << "Foreground image can only be 4 channel." << std::endl;
	 exit(-1);
       }
       unsigned char * dataf = Imgf.ImageGetData();
       int widb  = Imgb.GetWidth();
       int heib  = Imgb.GetHeight();
       int chnlb = Imgb.GetChannel();
       unsigned char * datab = Imgb.ImageGetData();   
       if((widb!=widf)||(heib!=heif))
	{
	  std::cout << "Background image must be the same size as forground image." << std::endl;
	  exit(-1);
	}
	else
	{
	  int num = widf*heif*4;
	  Imageout.SetWidth(widf);
	  Imageout.SetHeight(heif);
	  Imageout.SetChannel(4);
	  unsigned char * data;
	  unsigned char opague = 255;
          data = new unsigned char[num];
	  int j = 0;
	  for (int i=0; i<num; i++)
	  {
	    if ((i%chnlf)==3)
	      {
		float alfa=dataf[i]/255.0;
		data[i-3]=(unsigned char)(alfa*dataf[i-3])+(1-alfa)*(datab[j]);//red	
		data[i-2]=(unsigned char)(alfa*dataf[i-2])+(1-alfa)*(datab[j+1]);//blue	
		data[i-1]=(unsigned char)(alfa*dataf[i-1])+(1-alfa)*(datab[j+2]);//green	
		data[i]=opague;//alfa		
		j=j+chnlb;
	      }
	  }
	  Imageout.SetData(data);
      	  //delete data;
	}       

}
 
void RenderScene(){
       Imageout.ImageDisplay();
}
//keyboard handle function to implement image manipulation
void handleKey(unsigned char key, int x, int y){
  switch(key){

//use key 'w' or 'W' to write image
 //use key 'w' or 'W' to write image
      case 'w':	
      case 'W':
      if (writename != NULL)
      {
          Imageout.ImageWrite(writename);

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
   if (argc < 3 || argc > 4) {
      std::cerr << "Usage: " << argv[0] << " [foreground][background][output] " << std::endl;
      exit(1);
   }
   //if second parameter is specified, store the file name to write the image
   else 
   {
     if (argc == 4)
     {
       writename = argv[3];
       std::cout << "writename is" << writename << std::endl;
     }
   }

   //get the filename of the image, set the filename and read the image to get basic information of it
   filename = argv[1];
   filename2 = argv[2];
  // std::cout << filename << std::endl;
   Image2.SetFilename(filename2);
   Image.SetFilename(filename);
   Image.ImageRead();
   Image2.ImageRead();
   int h = Image.GetHeight();
   int w = Image.GetWidth();
   int c = Image.GetChannel();
   int h2 = Image.GetHeight();
   int w2 = Image.GetWidth();
   int c2 = Image.GetChannel();
   composite(Image, Image2);

   if (c!=4 || c2!=4)
   {
     std::cout << "Both images have to be 4 channel." << std::endl;
     exit(-1);
   }

   //start up the glut utilities
   glutInit(&argc, argv);
   
   //create the graphics window, giving width, height and title text
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
   glutInitWindowSize(min(w,w2),min(h,h2));
   glutCreateWindow("Image");

   //glut keyboard function, display function and mouse function
   glutKeyboardFunc(handleKey);
   glutDisplayFunc(RenderScene);
   //glutMouseFunc(mouseClick);

   //define the drawing coordinate system on the viewport
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0, w, 0, h);
     
  // specify window clear (background) color to be opaque white
   //glClearColor(1, 1, 1, 1);

//glut loops
   glutMainLoop();
   return 0;
}
