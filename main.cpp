/* Author: Mateusz Osiński
 * The program is detecting movemets in film
 */
 #include <iostream>
 #include <opencv2/opencv.hpp>
 
 #include "FramesDifference.h"
 
 using namespace std;
 using namespace cv;
 
void captureArm (int choice, string nameOfFile, VideoCapture *arm)
 {
	if (choice==1)
	{
		*arm=VideoCapture(nameOfFile);
	}
	else
	{
		*arm=VideoCapture(0); //0- default camera
	} 
 }
 
 int main ()
 {
	//capture the video
	cout<<"Nagrany film: 1, na żywo: 0 : ";
	short choice;
	cin>>choice;
	string nameOfFile;
	if (choice==1)
	{
		cout<<"Podaj nazwę pliku: ";
		cin>>nameOfFile;
	}
	
	VideoCapture arm;
	
	captureArm(choice, nameOfFile/*"dlon.avi"*/, &arm);

	if (!arm.isOpened())
	{
		cout<<"brak pliku lub niewłaściwe nawiązanie połączenia z kamerą\n";
		return -1;
	}
	
	//read the first frame
	Mat olderFrame, youngerFrame, olderFrameConv, youngerFrameConv;
	arm>>olderFrame;
	cvtColor(olderFrame, olderFrameConv, CV_RGB2GRAY); //conversion from RGB to other color space- to save the memory
	//to Luv-4B, to gray-4B, to XYZ-4B, to XSV-4B

	//frames per second
	float fps=25;//25;
	//if(choice) fps = cvGetCaptureProperty(arm, CV_CAP_PROP_FPS);
	//if(choice) fps=arm.get(CV_CAP_PROP_FPS);
	cout << "klatki" << fps;
	//distance between frames
	double framesDistance = 1000 / fps;
	//making a window
	namedWindow("window", WINDOW_AUTOSIZE);
	while (true)
	{
		if(!arm.grab()) break;
		arm>>youngerFrame;
		cvtColor(youngerFrame, youngerFrameConv, CV_RGB2GRAY);
		FramesDifference difference=FramesDifference(olderFrameConv, youngerFrameConv);
		youngerFrameConv.copyTo(olderFrameConv);
		//alarm if time without movement is longer than 20s
		if (FramesDifference::counter*framesDistance>=20000)
		{
			FramesDifference::alarm(10000, 1000);
			FramesDifference::counter=0;
		}
		
		int stop = waitKey(framesDistance);
		if (stop+1) break; //breaking with any key
	}
	
	return 0;
	 
 }
