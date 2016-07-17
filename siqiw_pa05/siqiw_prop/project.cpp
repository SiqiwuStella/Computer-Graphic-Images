/* This is the .cpp file for fluid dripping effect project. The program reads a image and implement a fluid dripping effect on it.

Author: Siqi Wu
Email:  siqiw@clemson.edu
Date:   12/01/2015

*/
#include <OpenImageIO/imageio.h>
#include <string>
#include <time.h>
OIIO_NAMESPACE_USING
//#include <typeinfo>
#include <iostream>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include<GL/glut.h>
#endif

#include "my.h"
#include <unistd.h>
//global variable image filename and filename to write this image
char* writename = NULL, * filename = NULL;

/*Tag to check if mouse is on click and tag to check if process is paused*/
int mouseState = 0, stop = 0;

/*Parameter to control the drip speed and wave shape.*/
float speed = 2;
int w = 0, h = 0, c = 0;
static float line = 0;
float* newimg = NULL;

/*current curve coordinates and initial curve coordinates*/
float** coor = NULL, ** init = NULL;

//global Object Image
MyImage Image;

/*display function*/
void RenderScene(){
	Image.ImageDisplay();
}

//function to implement the fluid dripping effect
void Twirl(){
	if(mouseState == 1 && stop == 0){
		int width = Image.GetWidth();
		int height = Image.GetHeight();
		int channel = Image.GetChannel();
		int total = width*height*channel;
		float * img = Image.ImageGetData();
		int i, j, Tx, Ty;
		float theta, r, rn;
		int iter=0, p = 0, v = 0, value = 0;
		for (i = 0; i<width; i++){
			j = (int)coor[i][1];
			if (j>=0 && j<height){
				//image column warp to the current curve, p is the row number of each column
				for (p = 0; p < j; p++){
					v = (int)(p*init[i][1]/coor[i][1]);
					img[p*width*channel+i*channel] = newimg[v*width*channel+i*channel];
					img[p*width*channel+i*channel+1] = newimg[v*width*channel+i*channel+1];
					img[p*width*channel+i*channel+2] = newimg[v*width*channel+i*channel+2];
					img[p*width*channel+i*channel+3] = newimg[v*width*channel+i*channel+3];
				}
			/*If the fluid reaches the bottom line, stop there.*/
			}else if(j>=height){
				for (p = 0; p < height; p++){
					v = (int)(p*init[i][1]/height);
					img[p*width*channel+i*channel] = newimg[v*width*channel+i*channel];
					img[p*width*channel+i*channel+1] = newimg[v*width*channel+i*channel+1];
					img[p*width*channel+i*channel+2] = newimg[v*width*channel+i*channel+2];
					img[p*width*channel+i*channel+3] = newimg[v*width*channel+i*channel+3];
				}

			}
			/*Each iteration the current curve is mutiplied by the sinusoid function below to enhance the effect. There speed parameter is used here for user to control the dripping speed and shape of wave.*/
			coor[i][1] =  5 + coor[i][1] + (0.8/2*speed)*sin(0.01*i+width*0.001)+speed*cos(0.03*i+width*0.002)+(0.5/2*speed)*sin(0.04*i+0.003*width)+(1/2*speed)*cos(0.02*i+width*0.0015)+(0.5/2*speed)*cos(0.008*(i+2)+width*0.002);
		}
		glutPostRedisplay();
		/*Control the sleep time of each flame*/
		usleep(70000);

	}
}


void mouseClick(int button, int state, int x, int y){
	switch(button){
		/*Start the process only when the clicked mouse button is released.*/
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_UP){
				std::cout << "mouse click" << y << std::endl;
				int i;
				line = (float)y;
				stop = 0;
				mouseState = 1;
				/*Each time user clicks on the image, the program generates a initial curve.*/
				for (i = 0; i < w; i++){
					coor[i][0] = i;//x unchanged
					coor[i][1] = 0.008*h + line + sin(0.02*i+3)+1*cos(0.03*i)+0.5*sin(0.05*i)+3*cos(0.1*i)+sin(0.04*i+0.5);//create new y
					init[i][0] = coor[i][0];
					init[i][1] = coor[i][1]+sin(0.02*i+3)+1*cos(0.03*i)+0.5*sin(0.05*i)+3*cos(0.1*i)+sin(0.04*i+0.5);
				}
			}
			break;
	}

}


//keyboard handle function to implement image manipulation
void handleKey(unsigned char key, int x, int y){
	float * ori, *copy;
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

		/*Use 'p' to pause the dripping process.*/
		case 'p':
		case 'P':
			stop = 1;
			break;

		/*Use 's' to resume the process or start the first process after running the program.*/
		case 's':
		case 'S':
			stop = 0;
			mouseState = 1;
			break;

		/*Use 'o' to display the original image.*/
		case 'o':
		case 'O':
		stop = 1;
		Image.SetOriginalData();
		ori = Image.GetOriginalData();
		Image.SetData(ori);
		glutPostRedisplay();
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
	if ((argc < 2) || (argc > 3)) {
		std::cerr << "Usage: " << argv[0] << " [filename][writename] " << std::endl;
		exit(1);
	}
	if (argc == 3){
		writename = argv[2];
	}
	std::cout << "Please input a speed control parameter (float from -3 to 3.5)" << std::endl;
	std::cin >> speed;
	/*If the input speed is out of range, output error message.*/
	if (speed < -3 || speed > 3.5 ){
		std::cout << "Parameter is out of range." << std::endl;
		exit(1);
	}
	std::cout << "speed control is " << speed << std::endl;


	//get the filename of the image, set the filename and read the image to get basic information of it
	filename = argv[1];
	std::cout << filename << std::endl;
	Image.SetFilename(filename);
	Image.ImageRead();
	Image.SetOriginalData();
	float * image;
	image = Image.ImageGetData();
	h = Image.GetHeight();
	w = Image.GetWidth();
	c = Image.GetChannel();
	newimg = new float[w*h*c](); 
	int total = h*w*c;
	int i; 
	for (i = 0; i < total; i++){
		newimg[i] = image[i];
	}
	coor = new float*[w];
	init = new float*[w];
	for (i = 0; i < w; i++){
		coor[i] = new float[w];
		init[i] = new float[w];
	} 
	//initialize a position for the fluid effect as 40%(vertical) of the image
	line = h*0.4;
	//generate a sinusoid as a initial driping line of the image
	for (i = 0; i < w; i++){
		coor[i][0] = i;//x unchanged
		coor[i][1] = 0.008*h + line + sin(0.02*i+3)+1*cos(0.03*i)+0.5*sin(0.05*i)+3*cos(0.1*i)+sin(0.04*i+0.5);//create new y
		init[i][0] = coor[i][0];
		init[i][1] = coor[i][1]+sin(0.02*i+3)+1*cos(0.03*i)+0.5*sin(0.05*i)+3*cos(0.1*i)+sin(0.04*i+0.5);
	}

	//start up the glut utilities
	glutInit(&argc, argv);

	//create the graphics window, giving width, height and title text
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutInitWindowSize((Image.GetWidth()),Image.GetHeight());
	glutCreateWindow("Image");

	//glut keyboard function, display function and mouse function
	glutKeyboardFunc(handleKey);
	glutDisplayFunc(RenderScene);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glutMouseFunc(mouseClick);
	glutIdleFunc(Twirl);
	//define the drawing coordinate system on the viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);

	// specify window clear (background) color to be opaque white
	glClearColor(0, 0, 0, 1);

	//glut loops
	glutMainLoop();
	delete[] newimg;
	for (i = 0; i < w; i++){
		delete[] coor[i];
		delete[] init[i];
	} 
	delete[] coor;
	delete[] init;
	return 0;
}
