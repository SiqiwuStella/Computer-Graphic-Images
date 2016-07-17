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
#define maximum(x, y, z) ((x) > (y)? ((x) > (z)? (x) : (z)) : ((y) > (z)? (y) : (z)))
#define minimum(x, y, z) ((x) < (y)? ((x) < (z)? (x) : (z)) : ((y) < (z)? (y) : (z)))


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
     void	    SetData ( unsigned char *newdata) {data = newdata;}

//SetOHeight: input the original height of the image, set the original height of the image
     void	    SetOHeight( int seth ){ oheight = seth; }

//SetOWidth: input the original width of the image, set the original width of the image
     void	    SetOWidth( int setw ){ owidth = setw; }

//SetOChannel: input the original channel of the image, set the original channel of the image
     void	    SetOChannel ( int setc ) {ochannel = setc;}

//SetChannel: input the channel of the image, set the channel of the image
     void	    SetChannel( int setc ){ channel = setc; }
     void 	    ImageDisplay();
     unsigned char* ImageVertical();
     unsigned char* ImageHorizontal();
     unsigned char* ImageRed();
     unsigned char* ImageGreen();
     unsigned char* ImageBlue();
     unsigned char* Threshold(double, double, double, double);
     unsigned char* PV(float);
     unsigned char* SS(float);
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
           SetOWidth(spec.width);
           SetOHeight(spec.height);
           SetOChannel(spec.nchannels);
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
       glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
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
       	/*if(curwid != wid || curhei != hei){
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
	}*/
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

unsigned char *MyImage::Threshold(double thl, double thh, double thv, double ths)
{
	char * fname = GetFilename();
        int     wid  = GetWidth();
        int     hei  = GetHeight();
        int     chnl = GetOChannel();
	//get current data
        unsigned char * pixels = GetOriginalData();
        unsigned char *hsv = new unsigned char[wid*hei*4];

 	double red, green, blue;
 	double max, min, delta;
 	int num = wid*hei*chnl; 
 	unsigned char r,g,b;
        unsigned char trsprt=0, opague=255;
	double h,s,v,range;
        int j = 0;
	range = (thh-thl)*0.3;
        for(int i = 0; i<num; i++)
 	{	
		if((i%(chnl))==0)
	 	  {
            	  //set the other two channels to the same value as the blue channel to display a greyscale image
		        r=pixels[i];
			g=pixels[i+1];
			b=pixels[i+2];
			red = r / 255.0; 
			green = g / 255.0; 
			blue = b / 255.0;
 			 /* r , g, b to 0 1 scale */
 			max = maximum(red, green, blue);
 			min = minimum(red, green, blue);
 			v = max;
			/*value is maximum of r , g, b */
 			if(max == 0) {
 			/*saturation and hue 0 if value is 0*/
 			s = 0;
  			h = 0;
  			}else{
   	 			s = (max - min) / max;
    				/*saturation is color purity on scale 0 1*/
    				delta = max - min;
    				if(delta == 0) {
    				/*hue doesn 't matter if saturation is 0 */
    				h = 0;
    				}else{
       					if(red == max) {
       					/*otherwise , determine hue on scale 0 360 */
       					h = (green - blue) / delta;
       					}else if(green == max) {
      					h = 2.0 + (blue - red) / delta;
      					}else{
      					/*(blue == max)*/
      					h = 4.0 + (red - green) / delta;
      					}
    					h = h * 60.0;
    					if(h < 0) {
      					h = h + 360.0;
     					}
   				}
 			}
			hsv[j] = pixels[i];
			j++;
			hsv[j] = pixels[i+1];
			j++;
			hsv[j] = pixels[i+2];
			j++;
			if ((v>thv)&&(s>ths))
			{
			  if ((h>=thl+range)&&(h<=thh-range))
			  {
			    hsv[j]=0;
			    j++;
			  }
			  else if ((h>thl-range)&&(h<thl+range))
			  {
			    hsv[j]=(h-(thl-range))*255/(2*range);
			    j++;
			  }
			  else if ((h>thh-range)&&(h<thh+range))
			  {
			    hsv[j]=(h-(thh-range))*255/(2*range);
			    j++;
			  }
			  else
			  {
			    hsv[j]=opague;
			    j++;
			  }
			}
			else{
			  hsv[j]=opague;
			  j++;
			}
       		  }
                      
        }
	//SetData(hsv);
	SetChannel(4);
	return hsv;
}

unsigned char * MyImage::PV(float k)
{
	char * fname = GetFilename();
        int     wid  = GetOWidth();
        int     hei  = GetOHeight();
        int     chnl = GetOChannel();
	//get current data
        unsigned char * pixels = GetOriginalData();
        unsigned char *pv = new unsigned char[wid*hei*4];

 	double red, green, blue;
 	double max, min, delta;
 	int num = wid*hei*chnl; 
 	unsigned char r,g,b;
        unsigned char trsprt=0, opague=255;
	//double h,s,v;
        int j = 0;
        for(int i = 0; i<num; i++)
	{
	   if((i%(chnl))==1)//green channel value
	   {
	     r=pixels[i-1];
	     g=pixels[i];
	     b=pixels[i+1];
	     pv[j]=r;
	     j++;
	     pv[j]=g;
	     j++;
	     pv[j]=b;
	     j++;
	     if(((float)g)>k*(float)b)//if G>kB
	     {
		pv[j]=trsprt;
		j++;
	     }
	     else
	     {
		pv[j]=opague;
		j++;
	     }
	   }
	}
	SetChannel(4);
	return pv;
}




unsigned char * MyImage::SS(float k)
{
	char * fname = GetFilename();
        int     wid  = GetWidth();
        int     hei  = GetHeight();
        int     chnl = GetChannel();
	//get current data
        unsigned char * pixels = ImageGetData();
        //unsigned char *ss = new unsigned char[wid*hei*4];

 	double max, min, delta;
 	int num = wid*hei*chnl; 
 	unsigned char r,g,b;
        for(int i = 0; i<num; i++)
	{
	   if((i%(chnl))==1)//suppressing green
	   {
	     r=pixels[i-1];
	     g=pixels[i];
	     b=pixels[i+1];
	     pixels[i]=minimum(r,g,b);
	   }
	}
	return pixels;
}
