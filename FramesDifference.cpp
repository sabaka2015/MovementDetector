#include "FramesDifference.h"

short FramesDifference::counter=0;

FramesDifference::FramesDifference(Mat old, Mat young)
{
	absdiff(old, young, difference);
	//cout<<endl<<(old.at<int>(100,100))<<"\t"<<(young.at<int>(100,100))
	//	<<"\t"<<(difference.at<int>(100,100));
	thresholding(difference, difference, 20, 255, THRESH_BINARY);
	//cout<<"\t"<<(difference.at<int>(100,100));
	//cout<<"\t"<<sizeof(difference.at<int>(100,100));
	//non-zero pixels
	movingPixels();
}

void FramesDifference::thresholding(Mat inImage, Mat outImage, double thres, double maxval, int type)
{
	threshold(inImage, outImage, thres, maxval, type);
}

void FramesDifference::movingPixels()
{
	//movement is detected if there are more than 1.5% on-zero pixels in frame
	int nonZero=(countNonZero(difference));
	int numberOfPixels=difference.cols*difference.rows;
	if (nonZero>0.0015*numberOfPixels) movement=true;
	else 
	{
		movement=false;
		counter++;
	}
	cout<<"\tpixels: "<<numberOfPixels<<"\tnon-zero: "<<nonZero
		<<"\tmovement: \a"<<movement<<endl;
}

void FramesDifference::alarm(int frequency, int time)
{
	//sound(frequency);
	cout<<"ALARM!!!\a\a\a\a\a\a\a\a\a\a";
}
