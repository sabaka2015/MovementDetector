/* Author: Mateusz Osiński
 * The program is detecting movements in video
 */
#include <time.h> 
 
#include "FramesDifference.h"

//this function gives an arm to the video. 
//the function returns fps 
//if we choose a live transmition, fps is counted- the get function do not work
float captureArm (int choice, string nameOfFile, VideoCapture *arm)
{
	float fps;
	if (choice==1)
	{
		*arm=VideoCapture(nameOfFile);
		fps=arm->get(CV_CAP_PROP_FPS);
	}
	else
	{
		*arm=VideoCapture(0); //0- default camera
		//start time
		time_t start, end;
		Mat lostFrames;
		time(&start);
		//grab 30 frames
		for(int i = 0; i < 30; i++)
		{
			(*arm) >> lostFrames;
		}
		//end Time
		time(&end);
		//total time
		double seconds = difftime (end, start);
		// Calculate frames per second
		cout<<seconds<<endl;
		fps  = 30 / seconds;
	}
	return fps; 
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
	float fps; //frames per second
	
	fps=captureArm(choice, nameOfFile/*"dlon.avi"*/, &arm);

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

	cout << "klatki" << fps << endl;
	//distance between frames in ms
	double framesDistance = 1000 / fps;
	//making a window
	namedWindow("window", WINDOW_AUTOSIZE);
	while (true)
	{
		if(!arm.grab()) break;
		arm>>youngerFrame;
		//conversion from RGB to Gray scale
		cvtColor(youngerFrame, youngerFrameConv, CV_RGB2GRAY);
		//object of the new class- difference between following frames
		FramesDifference difference=FramesDifference(olderFrameConv, youngerFrameConv);
		youngerFrameConv.copyTo(olderFrameConv);
		imshow("window", difference.getDifference());
		//alarm if time without movement is longer than 20s
		cout<<endl<<(FramesDifference::counter*framesDistance)<<endl;
		if (FramesDifference::counter*framesDistance>=20000)
		{
			FramesDifference::alarm();
		}
		
		int stop = waitKey(framesDistance);
		if (stop+1) break; //breaking with any key
	}
	
	return 0;
	 
}
