/*
  This is a header file for 'tonemap.cpp'. This file defines a class for image as well as
 a function: float * convolve(float * img, int wid, int hei, int ch, int number, float**
 kernel, int tag)

  Author: Siqi Wu
  Email: siqiw@clemson.edu
  Date: 10/27/2015
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
     float* 	    simpleTM(float);
     float*	    convolveTM(float **, int, float, int);
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
       	  delete out;
    }

//simple tone mapping function, input gamma, output image after gamma correction
float* MyImage::simpleTM(float gm){
  int i;
  int wid  = GetWidth();
  int hei  = GetHeight();
  int chnl = GetChannel();
  float* Ld = new float[wid*hei];
  float* Lw = new float[wid*hei];
  float* image = ImageGetData();
  for (i = 0; i < wid*hei; i++){
    //construct the luminance data for the image
    Lw[i] = (1.0/61.0)*(20*image[i*3]+40*image[i*3+1]+image[i*3+2]);
    Ld[i] = exp(gm*log(Lw[i]));
    image[i*3] = (Ld[i]/Lw[i])*image[i*3];//red channel correction
    image[i*3+1] = (Ld[i]/Lw[i])*image[i*3+1];//green channel correction
    image[i*3+2] = (Ld[i]/Lw[i])*image[i*3+2];//blue channel correction
  }

  delete Ld;
  delete Lw;
  return image;
}


//convolution function
//input image data, width, height, channel, kernel number, kernel data, btag
//if mode '-b', btag = 1, else, btag = 0
//output convolution result
float * convolve(float * img, int wid, int hei, int ch, int number, float** kernel, int tag){
  int i=0,j=0,r=0,c=0;
  float scale = 0, k = 0, val = 0, cvlv = 0, w = 1;
  float fkernel[number][number];

  //flip the kernel
  for (i=0; i < number; i++){
    for (j=0; j < number; j++){
      fkernel[i][j] = kernel[number-i-1][number-j-1];
    }
  }

  //start convolution
  for (r=0; r<hei; r++){
    for (c=0; c<wid; c++){
      scale = 0;
      cvlv = 0;
      //for each kernel window
      for (i = -number/2; i<=number/2; i++){
        for (j = -number/2; j<=number/2; j++){
          //if data falls out of the window, set both image data and kernel data to 0
          if ((c+i<0)||(c+i>=wid)||(r+j<0)||(r+j>=hei)){
            k = 0;
            val = 0;
          }else{
            //else get the corresponding kernel value and image value
            k = fkernel[number/2+i][number/2+j];
            val = img[(r+j)*wid+c+i];
          }
	  //if it is not in '-b' mode, do the simple convolution
          if (tag == 0){
	    cvlv = cvlv + val*k;
          }else{
            //else, '-b' mode is set. multiply by a weight w = exp(-clamp(d^2)) 
            w = exp((-1)*(val-img[r*wid+c])*(val-img[r*wid+c]));
            cvlv = cvlv + val*k*w;
          }
          // if '-b' mode is not set, w = 1; if '-b' is set, scale factor is changed by w
          scale = scale + k*w;
        }
      }
     img[r*wid+c] = cvlv/scale;
    }
  }
  return img;
}


//function of tone mapping with convolution
//input kernel data, number of kernel, gamma, btag
//if mode '-b', btag = 1, else, btag = 0
float* MyImage::convolveTM(float **kernel, int num, float gam, int tag){
   std::cout << "kernel num in function convolveTM " << num << std::endl;
   int newnum = num;
   int i;
   int hei = GetHeight();
   int wid = GetWidth();
   int chnl = GetChannel();
   //only supports 3 channel images
   if (chnl != 3){
     std::cout << "Tone mapping with convolution only supports 3 channel images." << std::endl;
     exit(-1);
   }
   float* L = new float[wid*hei];
   float* Lw = new float[wid*hei];
   float* Log = new float[wid*hei];
   float* B = NULL;
   float* image = ImageGetData();
   for (i = 0; i < wid*hei; i++){
     Lw[i] = (1.0/61.0)*(20*image[i*3]+40*image[i*3+1]+image[i*3+2]);
     L[i] = log(Lw[i]);//L stores log(Lw) and serves as a input to function convole
     //make a copy of log(Lw) to calculate the final output
     //because L changes after convolution
     Log[i] = log(Lw[i]);
   }
   // convolve with kernel
   B = convolve(L, wid, hei, 1, newnum, kernel, tag);
   for (i = 0; i < wid*hei; i++){
     Log[i] = exp(gam*B[i]+Log[i] - B[i]);
     image[i*3] = (Log[i]/Lw[i])*image[i*3];//red channel correction
     image[i*3+1] = (Log[i]/Lw[i])*image[i*3+1];//green channel correction
     image[i*3+2] = (Log[i]/Lw[i])*image[i*3+2];//blue channel correction     
   }  
  delete L;
  delete Lw;
  delete Log;
  return image;
}



