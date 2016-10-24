#include "FramesDifference.h"

short FramesDifference::counter=0;

FramesDifference::FramesDifference(Mat old, Mat young)
{
	absdiff(old, young, difference);
	thresholding(difference, difference, 15, 255, THRESH_BINARY);
	movingPixels();
}

void FramesDifference::thresholding(Mat inImage, Mat outImage, double thres, double maxval, int type)
{
	threshold(inImage, outImage, thres, maxval, type);
}

void FramesDifference::movingPixels()
{
	//movement is detected if there are more than 0.07% non-zero pixels in frame
	int nonZero=(countNonZero(difference));
	int numberOfPixels=difference.cols*difference.rows;
	if (nonZero>0.0007*numberOfPixels) 
	{
		movement=true;
		counter=0;
	}
	else 
	{
		movement=false;
		counter++;
	}
	cout<<"\tpixels: "<<numberOfPixels<<"\tnon-zero: "<<nonZero
		<<"\tmovement: \a"<<movement<<endl;
}

void FramesDifference::alarm()
{
	cout<<"ALARM!!!\a\a";
}
