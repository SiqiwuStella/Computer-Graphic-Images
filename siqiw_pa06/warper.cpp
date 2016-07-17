/*
   Description:A image warpping tool to implement several kinds of affine warp.
   Usage: ./warper input.png [write.png]
 
   Reference:
   Program template for PA06 to warp an image using matrix-based warps
   By Joshua A. Levine, 11/3/15

   Author: Siqi Wu
   Email: siqiw@g.clemson.edu
   Date: 11/11/2015 
*/

#include "Matrix.h"
#include "my.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
    
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
char *writename = NULL, *fname = NULL;
int width, height, channel, neww, newh;
using namespace std;

/*
   Convert the string s to lower case
*/


MyImage Image;

//display function
void RenderScene(){
  Image.ImageDisplay();
}


void lowercase(char *s){
   int i;

   if(s != NULL) {
      for(i = 0; s[i] != '\0'; i++) {
         if(s[i] >= 'A' && s[i] <= 'Z')
            s[i] += ('a' - 'A');
      }
   }
}



/* 
   Multiply M by a rotation matrix of angle theta
*/

void Rotate(Matrix3x3 &M, float theta){
   int row, col;
   Matrix3x3 R(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
   double rad, c, s;

   rad = PI * theta / 180.0;
   c = cos(rad);
   s = sin(rad);

   R[0][0] = c;
   R[0][1] = -s;
   R[1][0] = s;
   R[1][1] = c;

   Matrix3x3 Prod = R * M;

   for(row = 0; row < 3; row++) {
      for(col = 0; col < 3; col++) {
         M[row][col] = Prod[row][col];
      }
   }
}


void Translation(Matrix3x3 &M, float deltax, float deltay){
   int row, col;
   Matrix3x3 R(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
   R[0][2] = deltax;
   R[1][2] = deltay;

   Matrix3x3 Prod = R * M;

   for(row = 0; row < 3; row++) {
      for(col = 0; col < 3; col++) {
         M[row][col] = Prod[row][col];
      }
   }
}

void Shear(Matrix3x3 &M, float shx, float shy){
   int row, col;
   Matrix3x3 R(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
   R[0][1] = shx;
   R[1][0] = shy;

   Matrix3x3 Prod = R * M;

   for(row = 0; row < 3; row++) {
      for(col = 0; col < 3; col++) {
         M[row][col] = Prod[row][col];
      }
   }
}

void Scale(Matrix3x3 &M, float sx, float sy){
   int row, col;
   Matrix3x3 R(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
   R[0][0] = sx;
   R[1][1] = sy;

   Matrix3x3 Prod = R * M;

   for(row = 0; row < 3; row++) {
      for(col = 0; col < 3; col++) {
         M[row][col] = Prod[row][col];
      }
   }
}

void Twirl(float s, float cx, float cy){
  int total = width*height*channel;
  float * img = Image.ImageGetData();
  int i, j, Tx, Ty;
  float theta, r;
  int minDim = (width > height) ? width : height;
  //cx = cx * width;
  //cy = cy * height;
  float* newimg = new float[width*height*channel]();
  for (i = 0; i < width; i++){
    for (j = 0; j < height; j++){
      if ((i-cx*width) != 0){
        if (i-cx*width < 0){
          theta = atan((float)(j-cy*height)/(float(i-cx*width)));
        }else{
          if (j-cy*height<=0){
            theta = atan((float)(j-cy*height)/(float(i-cx*width)))+ PI;
          }else{
            theta = atan((float)(j-cy*height)/(float(i-cx*width)))- PI;
          }
        }
      }else{ //i-cx*width == 0
	if (j-cy*height <= 0){
          theta = 0.5 * PI;
        }else{
          theta = -0.5 * PI;
        }
      }
      theta = (theta - PI);
      r = sqrt((i-cx*width)*(i-cx*width)+(j-cy*height)*(j-cy*height));
      Tx = r*cos(theta+s*(r-minDim)/minDim) + cx*width;
      Ty = r*sin(theta+s*(r-minDim)/minDim) + cy*height;
      // check if falls out of the image
      if ((Ty>=0) && (Tx>=0) && (Ty<height) && (Tx<width)){
          newimg[j*width*channel+i*channel] = img[Ty*width*channel+Tx*channel];
        if (channel >= 3){
          newimg[j*width*channel+i*channel+1] = img[Ty*width*channel+Tx*channel+1];
          newimg[j*width*channel+i*channel+2] = img[Ty*width*channel+Tx*channel+2];
	  if (channel == 4){
             newimg[j*width*channel+i*channel+3] = img[Ty*width*channel+Tx*channel+3];
          }
       }
      }
    }
  }
  for (i = 0; i<total; i++){
    img[i] = newimg[i];
  }
  Image.SetData(img);
  delete newimg;
}

//Image transform function. Warp the input image with the given matrix. (Inverse warp)
void Transform(float* img, int wid, int hei, int nwid, int nhei, int minx, int miny, int chnl, float * output, Matrix3x3 M){
   int x, y, nw, nh;
   int total = wid * hei * chnl;
   Vector3d v3(0.0, 0.0, 1.0);
   Vector3d out;
   for (x = 0; x < nwid; x++){
     for (y = 0; y < nhei; y++){
     //add back the minimum value of x and y, because the image coordinates start from 0	
        v3.x = x+minx;
	v3.y = nhei-y+miny;//because of glPixelZoom
        out = M * v3;
     //coordinates of the original image
	nw = out.x;
	nh = out.y;
	if ((nh>=0) && (nw>=0) && (nh<hei) && (nw<wid)){
	  nh = hei-nh;//because of glPixelZoom
          output[y*nwid*chnl+x*chnl] = img[nh*wid*chnl+nw*chnl];
          // Handle images with different channel
          if (chnl >= 3){
	    output[y*nwid*chnl+x*chnl+1] = img[nh*wid*chnl+nw*chnl+1];
	    output[y*nwid*chnl+x*chnl+2] = img[nh*wid*chnl+nw*chnl+2];
            if (chnl == 4){
               output[y*nwid*chnl+x*chnl+3] = img[nh*wid*chnl+nw*chnl+3];
            }
          }
        }
     }
   }   
}

/*
   Routine to build a projective transform from input text, display, or
   write transformed image to a file
*/
void process_input(Matrix3x3 &M){
   char command[1024];
   bool done;
   float theta, sx, sy, shx, shy, deltax, deltay, s, cx, cy;

   /* build identity matrix */
   M.identity();

   for(done = false; !done;) {

      /* prompt and accept input, converting text to lower case */
      printf("> ");
      scanf("%s", command);
      lowercase(command);

      /* parse the input command, and read parameters as needed */
      if(strcmp(command, "d") == 0) {
         done = true;
      } else if(strlen(command) != 1) {
         printf("invalid command, enter r, s, t, h, d\n");
      } else {
         switch(command[0]) {

            case 'r':		/* Rotation, accept angle in degrees */
               if(scanf("%f", &theta) == 1)
               {
                  Rotate(M, theta);
               }
               else
                  fprintf(stderr, "invalid rotation angle\n");
               break;
            case 's':		/* Scale, accept scale factors */
	       if(scanf("%f", &sx) == 1){
		 if(scanf("%f", &sy) == 1){
                   //if scale by 0, output error
                   if ((sx == 0) || (sy == 0)){
  		     fprintf(stderr, "You cannot scale the image by 0. Please enter again.\n");
		   }
		   Scale(M, sx, sy);
		 }
	      	 else
		   fprintf(stderr, "Invalid scale factor. Please enter again\n");
               }
	       else
		   fprintf(stderr, "Invalid scale factor. Please enter again\n");
               break;
            case 't':		/* Translation, accept translations */
	       if(scanf("%f", &deltax) == 1){
		 if(scanf("%f", &deltay) == 1){
		   Translation(M, deltax, deltay);
		 }
	      	 else
		   fprintf(stderr, "Invalid translations. Please enter again\n");
               }
	       else
		   fprintf(stderr, "Invalid translations. Please enter again\n");
               break;
            case 'h':		/* Shear, accept shear factors */
	       if(scanf("%f", &shx) == 1){
		 if(scanf("%f", &shy) == 1){
		   Shear(M, shx, shy);
		 }
	      	 else
		   fprintf(stderr, "Invalid shear factor. Please enter again\n");
               }
	       else
		   fprintf(stderr, "Invalid shear factor. Please enter again\n");
               break;
            case 'n':		/* Nonlinear twirl warp, accept strenght and center */
	       if(scanf("%f", &s) == 1){
		 if(scanf("%f", &cx) == 1){
                   if(scanf("%f", &cy) == 1){
                     //if the center of twirl falls out of the image, output error.
 		     if(cx<0 || cy<0 || cx>1 || cy>1){
  			fprintf(stderr, "Invalid center coordinates. Please enter again\n");	
                     }
                     Twirl(s, cx, cy);
                   }
		   else
		     fprintf(stderr, "Invalid twirl center y coordinate. Please enter again\n");		      }
 		 else
		   fprintf(stderr, "Invalid twirl center x coordinate. Please enter again\n");
	       }
               else
		 fprintf(stderr, "Invalid strength factor of twirl. Please enter again\n");
               break;
            case 'd':		/* Done, that's all for now */
               done = true;
               break;
            default:
               printf("invalid command, enter r, s, t, h, n, d\n");
         }
      }
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



/*
   Main program to read an image file, then ask the user
   for transform information, transform the image and display
   it using the appropriate warp.  Optionally save the transformed
   images in  files.
*/
int main(int argc, char *argv[]){
   Matrix3x3 M(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);

   //handle command line parameters
   if (argc > 3){
     cout << "Usage: " << argv[0] << " inimage.png [outimage.png]." << endl;
     exit(-1); 
   }
   //If write name is specified, set writename
   if (argc == 3){
     writename = argv[2];
   }

   //read in the input image
   fname = argv[1];
   Image.SetFilename(fname);
   Image.ImageRead();
   float* data = Image.ImageGetData();
   width = Image.GetWidth();
   cout << width << endl;
   height = Image.GetHeight();
   cout << height << endl;
   channel  = Image.GetChannel();
   Image.SetOriginalData();

   //next, build the transformation matrix
   float * trsfm = NULL;
   process_input(M);
   float *img;
   img = Image.ImageGetData();
   int x, y;
   cout << "Accumulated Matrix: " << endl;
   cout << M << endl;

   //Decide the size of the output image
   int maxx = -10000000, minx = 10000000,
       maxy = -10000000, miny = 10000000;
   int i, j;
   Vector3d out;
   Vector3d v3(0.0, 0.0, 1.0); 
   for (i = 0; i <= 1; i++){
     for (j = 0; j <= 1; j++){
       v3.x = (float)(i*(width-1));
       v3.y = (float)(j*(height-1));
       out = M * v3;
       if(out.x < minx){minx = out.x;}
       if(out.x > maxx){maxx = out.x;}
       if(out.y < miny){miny = out.y;}
       if(out.y > maxy){maxy = out.y;}
     }
   }
   //Get the size of the output image
   neww = maxx-minx;
   newh = maxy-miny;
   trsfm = new float[neww*newh*channel]();
   Image.SetWidth(neww);
   Image.SetHeight(newh);

   //Get inverse matrix of M
   Matrix In = M.inv();

   //Transform image
   Transform(img, width, height, neww, newh, minx, miny, channel, trsfm, In);

   Image.SetData(trsfm);
   //finally display the warped image
 //start up the glut utilities
   glutInit(&argc, argv);
   
   //create the graphics window, giving width, height and title text
   glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH | GLUT_RGBA);
   glutInitWindowSize(Image.GetWidth(),Image.GetHeight());
   glutCreateWindow("Image");

   //glut keyboard function, display function and mouse function
   glutKeyboardFunc(handleKey);
   glutDisplayFunc(RenderScene);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   //define the drawing coordinate system on the viewport
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0,Image.GetWidth(), 0, Image.GetHeight());
     
  // specify window clear (background) color to be opaque black
   glClearColor(0, 0, 0, 1);

//glut loops
   glutMainLoop();

   delete trsfm;
   return 0;
}




