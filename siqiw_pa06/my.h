/*
  This is a header file for 'warper.cpp'. This file defines a class for image.

  Author: Siqi Wu
  Email: siqiw@g.clemson.edu
  Date: 11/11/2015
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
     int ochannel;
     int oheight;
     int owidth;
     float *data;//Store the data of the image. Will change after manipulation.
     float* originaldata;//Store the original data of the image. This won't change after manipulation.
   
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
     float* ImageGetData(){ return data; }

//GetOriginalData: return the original data of the image
     float* GetOriginalData(){ return originaldata; }

//GetHeight: return the height of the image
     int	    GetHeight(){ return height; }

//GetOHeight: return the original height of the image
     int	    GetOHeight(){ return oheight; }

//GetWidth: return the width of the image
     int 	    GetWidth(){ return width; }

//GetOWidth: return the original width of the image
     int 	    GetOWidth(){ return owidth; }

//GetChannel: return the channel of the image
     int	    GetChannel(){ return channel; }

//GetOChannel: return the original channel of the image
     int	    GetOChannel(){ return ochannel; }

//GetFilename: return the filename of the image
     char* 	    GetFilename(){ return filename; }

//SetHeight: input the height of the image, set the height of the image
     void	    SetHeight( int seth ){ height = seth; }

//SetWidth: input the width of the image, set the width of the image
     void	    SetWidth( int setw ){ width = setw; }

//SetData: input the data of the image, set the current data of the image
     void	    SetData ( float *newdata) {data = newdata;}

//SetOHeight: input the original height of the image, set the original height of the image
     void	    SetOHeight( int seth ){ oheight = seth; }

//SetOWidth: input the original width of the image, set the original width of the image
     void	    SetOWidth( int setw ){ owidth = setw; }

//SetOChannel: input the original channel of the image, set the original channel of the image
     void	    SetOChannel ( int setc ) {ochannel = setc;}

//SetChannel: input the channel of the image, set the channel of the image
     void	    SetChannel( int setc ){ channel = setc; }
     void 	    ImageDisplay();

    };

//constructor (when create a new image, initialize the filename, height, width and channel of the image)
    MyImage::MyImage(char * name, int h, int w, int c):filename(name), height(h), width(w), channel(c){};

//constructor to store an existing image (when no parameters passed, initialize height, width and channel to zero)
    MyImage::MyImage(): height(0), width(0), channel(0){};

//destructor
    MyImage::~MyImage()
    {
       delete data;
       delete originaldata;
       std::cout<<"Image " << this -> filename << " is destroyed." << std::endl;    }


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
           SetOWidth(spec.width);
           SetOHeight(spec.height);
           SetOChannel(spec.nchannels);
  	 //declare memory, open and read it
           originaldata = new float[width*height*channel];

            //read in image as FLOAT	
            if (!in->read_image (TypeDesc::FLOAT, originaldata)) {
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
           data = new float[width*height*channel];
	
           if (!in->read_image (TypeDesc::FLOAT, data)) {
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
       glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
       int wid  = GetWidth();
       int hei  = GetHeight();
       int chnl = GetChannel();
       float * data = ImageGetData();
   
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
           glDrawPixels(wid,hei,GL_RGBA,GL_FLOAT,data);
	}
        //if the channel number is 3, draw the image using 'GL_RGB'
        else if (chnl == 3)
          {
              glDrawPixels(wid,hei,GL_RGB,GL_FLOAT,data);    
          } 
        //if the channel number is neither 4 or 3, output cannot display the image
        else if (chnl == 1)
        {
	      glDrawPixels(wid,hei,GL_LUMINANCE,GL_FLOAT,data);
        }
        else
	{
	   std::cout << "Cannot display the image. The image is neither 4 channel, 3 channel." << std::endl; 
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
        float * pixels = ImageGetData();

	ImageOutput *out = ImageOutput::create(wname);
	if (!out) {
	  std::cerr << "Could not create: " << geterror();
	  exit(-1);
	  }

   //create the ImageSpec that describes how you will write the output data
	ImageSpec spec (wid, hei, chnl, TypeDesc::FLOAT);
	out->open(wname, spec);

   //it is possible that this TypeDesc does not match the ImageSpec, 
   //in which case it will convert the raw data into the spec. 
   //But it MUST match the datatype of raw data
	out->write_image(TypeDesc::FLOAT, pixels);

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


