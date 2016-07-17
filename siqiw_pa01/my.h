/*
  This is a header file for 'oiioview.cpp'. This file contains a class for image, including member functions to implement image read, image write, image display, red channel image, green channel image, horizontally flipped image, vertically flipped image and original image display functions.

  Author: Siqi Wu
  Email: siqiw@clemson.edu
  Date: 09/06/2015
*/

#include <OpenImageIO/imageio.h>

OIIO_NAMESPACE_USING

#include <stdio.h>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif


//define a class for image 
class MyImage{
//define the basic information of an image
   private:
     char *filename;
     int height;
     int width;
     int channel;

     unsigned char *data;//Store the data of the image. Will change after manipulation.

     unsigned char *originaldata;//Store the original data of the image. This won't change after manipulation.
   
//declare constructor, destructor and member functions          
   public:
     MyImage();
     MyImage(char *, int, int, int);
     ~MyImage();

//SetFilename: input a filename, specify a filename for the image
     void 	    SetFilename(char *newname){ filename = newname;}
     void	    SetOriginalData ();
     void 	    ImageRead();
     void           ImageWrite(char *);

//ImageGetData: return the current data of the image
     unsigned char* ImageGetData(){ return data; }

//GetOriginalData: return the original data of the image
     unsigned char* GetOriginalData(){ return originaldata; }

//GetHeight: return the height of the image
     int	    GetHeight(){ return height; }

//GetWidth: return the width of the image
     int 	    GetWidth(){ return width; }

//GetChannel: return the channel of the image
     int	    GetChannel(){ return channel; }

//GetFilename: return the filename of the image
     char* 	    GetFilename(){ return filename; }

//SetHeight: input the height of the image, set the height of the image
     void	    SetHeight( int seth ){ height = seth; }

//SetWidth: input the width of the image, set the width of the image
     void	    SetWidth( int setw ){ width = setw; }

//SetData: input the data of the image, set the current data of the image
     void	    SetData ( unsigned char *newdata) {data = newdata;}

//SetChannel: input the channel of the image, set the channel of the image
     void	    SetChannel( int setc ){ channel = setc; }
     void 	    ImageDisplay();
     unsigned char* ImageVertical();
     unsigned char* ImageHorizontal();
     unsigned char* ImageRed();
     unsigned char* ImageGreen();
     unsigned char* ImageBlue();

    };

//constructor (when create a new image, initialize the filename, height, width and channel of the image)
    MyImage::MyImage(char * name, int h, int w, int c):filename(name), height(h), width(w), channel(c){};

//constructor to store an existing image (when no parameters passed, initialize height, width and channel to zero)
    MyImage::MyImage(): height(0), width(0), channel(0){};

//destructor
    MyImage::~MyImage()
    {
       std::cout<<"Image " << this -> filename << " is destroyed." << std::endl;}


//Set OriginalData(used after setting the filename to store the original data in *originaldata in order to show original image after any manipulation)
    void MyImage::SetOriginalData (){
       char * fname = GetFilename();

    if (!fname){
       std::cout << "Your Image doesn't have a filename." << std::endl;
 	exit(-1);
    }
    else
    {
        ImageInput *in = ImageInput::open(fname);
           if (!in) {
              std::cerr << "Could not create: " << geterror();
              exit(-1);
           }

  	 //after opening the image we can access
  	 //information about it
           const ImageSpec &spec = in->spec();
           SetWidth(spec.width);
           SetHeight(spec.height);
           SetChannel(spec.nchannels);
  	 //declare memory, open and read it
           originaldata = new unsigned char[width*height*channel];
	
 	  //TypeDesc::UINT8 maps the data into a desired type (unsigned char),
 	  //even if it wasn’t originally of that type
           if (!in->read_image (TypeDesc::UINT8, originaldata)) {
              std::cerr << "Could not read pixels from " << filename;
              std::cerr << ", error = " << in->geterror() << std::endl;
              delete in;
              exit(-1); 
           }
          
           if (!in->close ()) {
                std::cerr << "Error closing " << filename;
                std::cerr << ", error = " << in->geterror() << std::endl;
 	   delete in;
                exit(-1);
           }
           delete in;
        }

    }


//Function to Read a image after setting file name, store the data of the image in *data. *data changes after image manipulation
    void MyImage::ImageRead(){
    char * fname = GetFilename();
    std::cout<< fname << std::endl;
    if (!fname){
        std::cout << "Your Image doesn't have a filename." << std::endl;
 	exit(-1);
    }
    else
    {
        ImageInput *in = ImageInput::open(fname);
           if (!in) {
              std::cerr << "Could not create: " << geterror();
              exit(-1);
           }

  	 //after opening the image we can access
  	 //information about it
           const ImageSpec &spec = in->spec();
           SetWidth(spec.width);
           SetHeight(spec.height);
           SetChannel(spec.nchannels);

           std::cout << "x resolution: " << width << std::endl;
           std::cout << "y resolution: " << height << std::endl;
           std::cout << "num channels: " << channel << std::endl;

  	 //declare memory, open and read it
           data = new unsigned char[width*height*channel];
	
 	  //TypeDesc::UINT8 maps the data into a desired type (unsigned char),
 	  //even if it wasn’t originally of that type
           if (!in->read_image (TypeDesc::UINT8, data)) {
              std::cerr << "Could not read pixels from " << filename;
              std::cerr << ", error = " << in->geterror() << std::endl;
              delete in;
              exit(-1); 
           }
           if (!in->close ()) {
                std::cerr << "Error closing " << filename;
                std::cerr << ", error = " << in->geterror() << std::endl;
 	   delete in;
                exit(-1);
           }
           delete in;
        }
    }  
    

//ImageDisplay: To display the image using current data
    void MyImage::ImageDisplay(){
       glClear(GL_COLOR_BUFFER_BIT);
       int wid  = GetWidth();
       int hei  = GetHeight();
       int chnl = GetChannel();
       unsigned char * data = ImageGetData();
   
       //get the current window width and height
       int curwid = glutGet(GLUT_WINDOW_WIDTH);
       int curhei = glutGet(GLUT_WINDOW_HEIGHT);

       //change the origin to left top
       glRasterPos2i (0,hei);

       //change the drawing direction to right and down
       glPixelZoom(1,-1);
       glViewport(0,0,wid,hei);

       //if the channel number is 4, draw the image using 'GL_RGBA'
       if (chnl == 4)
	{
           glDrawPixels(wid,hei,GL_RGBA,GL_UNSIGNED_BYTE,data);
	}
        //if the channel number is 3, draw the image using 'GL_RGB'
        else if (chnl == 3)
          {
              glDrawPixels(wid,hei,GL_RGB,GL_UNSIGNED_BYTE,data);    
          } 
        //if the channel number is neither 4 or 3, output cannot display the image
        else
	{
	   std::cout << "Cannot display the image. The image is neither 4 channel, 3 channel." << std::endl; 
	}

        //if current window width and height are not equal to the width and height of the image, get the ratio of them and display the image in the middle of the window 
       	if(curwid != wid || curhei != hei){
	   float xratio = wid/curwid;
	   float yratio = hei/curhei;

           //when the width and height of the window is larger than those of the image
	   if(xratio <= 1 && yratio <=1){
	 	glRasterPos2i (0,hei);
       		//change the drawing direction to right and down
      		glPixelZoom(xratio,(-1)*yratio);
     		glViewport(curwid/2-wid/2,hei/2-curhei/2,curwid,curhei);
		if (chnl == 4)
		{
	           glDrawPixels(wid,hei,GL_RGBA,GL_UNSIGNED_BYTE,data);
		}
	        //if the channel number is 3, draw the image using 'GL_RGB'
	        else if (chnl == 3)
	          {
	              glDrawPixels(wid,hei,GL_RGB,GL_UNSIGNED_BYTE,data);    
	          } 
		else
		{
		   std::cout << "Cannot display the image. The image is neither 4 channel, 3 channel." << std::endl; 
		}

	   }
	}
       glutSwapBuffers();
       glFlush();
    }



//Function to write the image using current data. Input a filename to store the image. 
    void MyImage::ImageWrite(char * wname){

        int wid  = GetWidth();
        int hei  = GetHeight();
        int chnl = GetChannel();

	//get the image data we want to write
        unsigned char * pixels = ImageGetData();

	ImageOutput *out = ImageOutput::create(wname);
	if (!out) {
	  std::cerr << "Could not create: " << geterror();
	  exit(-1);
	  }

   //create the ImageSpec that describes how you will write the output data
	ImageSpec spec (wid, hei, chnl, TypeDesc::UINT8);
	out->open(wname, spec);

   //it is possible that this TypeDesc does not match the ImageSpec, 
   //in which case it will convert the raw data into the spec. 
   //But it MUST match the datatype of raw data
	out->write_image(TypeDesc::UINT8, pixels);

	if (!out->close ()) {
   	  std::cerr << "Error closing " << wname;
     	  std::cerr << ", error = " << out->geterror() << std::endl;
    	  delete out;
    	  exit(-1);
  	 }
         //output the successfully written message
         else{
          std::cout << wname << " is successfully written!" << std::endl;
         }
       	  delete out;
    }


//function to flip the image vertically. Return a pointer to the vertically flipped image   
    unsigned char * MyImage::ImageVertical(){
	char * fname = GetFilename();
        int     wid  = GetWidth();
        int     hei  = GetHeight();
        int     chnl = GetChannel();

	//get the current data of the image
        unsigned char * pixels = ImageGetData();
        // exchange the datas
        unsigned char swap = 0;
        int nd = 0;
	int num = wid*hei*chnl;
	for(int i = 0; i<(num/2); i++){
           nd = i+(wid*chnl)*(hei-2*(i/(wid*chnl))-1);
	   swap = pixels[i];
           pixels[i] = pixels[nd];
           pixels[nd] = swap;
	}
     return pixels;
     }


//function to flip the image horizontally. Return a pointer to the horizontally flipped image   
    unsigned char * MyImage::ImageHorizontal(){
	char * fname = GetFilename();
        int     wid  = GetWidth();
        int     hei  = GetHeight();
        int     chnl = GetChannel();

	//get the current data
        unsigned char * pixels = ImageGetData();
        // exchange the datas
        unsigned char swap = 0;
        int nd = 0;
	int num = wid*hei*chnl;
	for(int i = 0; i<num; i++){
	   if((i%(wid*chnl))<(wid*chnl/2))
	   {
              nd = (wid-(2*((i%(wid*chnl))/chnl)+1)+1)*chnl+i;
	      swap = pixels[i];
              pixels[i] = pixels[nd];
              pixels[nd] = swap;
           }
	}
     return pixels;
     }


//function to get the red channel greyscale image. Return a pointer to the red channel greyscale image  
     unsigned char * MyImage::ImageRed(){
	char * fname = GetFilename();
        int     wid  = GetWidth();
        int     hei  = GetHeight();
        int     chnl = GetChannel();

	//get current data
        unsigned char * pixels;
        pixels = ImageGetData();

        // get red data
        int j = 0;
	int num = wid*hei*chnl;
        for(int i = 0; i<num; i++){
	   if((i%(chnl))==0)
	   {
              //set the other two channels to the same value as the red channel to display a greyscale image
              pixels[i+1]=pixels[i];
              pixels[i+2]=pixels[i];
           }
	}
        return pixels;
    }


//function to get the green channel greyscale image. Return a pointer to the green channel greyscale image 
    unsigned char * MyImage::ImageGreen(){
	char * fname = GetFilename();
        int     wid  = GetWidth();
        int     hei  = GetHeight();
        int     chnl = GetChannel();

	//get current data
        unsigned char * pixels = ImageGetData();
        // get green data
        int j = 0;
	int num = wid*hei*chnl;
        for(int i = 0; i<num; i++){
	   if((i%(chnl))==1)
	   {
              //set the other two channels to the same value as the green channel to display a greyscale image
              pixels[i-1]=pixels[i];
              pixels[i+1]=pixels[i];
           }
	}
        return pixels;
    }


//function to get the blue channel greyscale image. Return a pointer to the blue channel greyscale image 
     unsigned char * MyImage::ImageBlue(){
	char * fname = GetFilename();
        int     wid  = GetWidth();
        int     hei  = GetHeight();
        int     chnl = GetChannel();

	//get current data
        unsigned char * pixels = ImageGetData();
        // get blue data
        int j = 0;
	int num = wid*hei*chnl;
        for(int i = 0; i<num; i++){
	   if((i%(chnl))==2)
	   {
              //set the other two channels to the same value as the blue channel to display a greyscale image
              pixels[i-1]=pixels[i];
              pixels[i-2]=pixels[i];
           }
	}
        return pixels;
    }

