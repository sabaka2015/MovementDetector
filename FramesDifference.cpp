#include "FramesDifference.h"

short FramesDifference::counterZero=0;
float FramesDifference::ElapsedTime=0;

FramesDifference::FramesDifference(Mat old, Mat young, int scenario, string txtExport)
{
	switch (scenario)
	{
		case 1:
			difference=GaussBlurScenario(old, young);
			break;
		case 2:
			difference=MedianBlurScenario(old, young);
			break;
		case 3:
			difference=BoxBlurScenario(old, young);
			break;
		case 4:
			difference=DilatBlurScenario(old, young);
			break;
		case 5:
			difference=SobelBlurScenario(old, young);
			break;
	}
	#if 0
	Mat difference_helper;
	Mat a, b;
	GaussianBlur(old, a, Size(21, 21), 0, 0);
	//thresholding(a, a, 4, 255, THRESH_BINARY);
	GaussianBlur(young, b, Size(21, 21), 0, 0);
	//thresholding(b, b, 4, 255, THRESH_BINARY);
	absdiff(a, b, difference_helper);
	//absdiff(old, young, difference_helper);
	difference=difference_helper;
	//difference_helper=old;
	//GaussianBlur(difference_helper, difference, Size(21, 21), 0, 0);
	thresholding(difference, difference, 3, 255, THRESH_BINARY);
	#endif
	movingPixels(txtExport);
}

void FramesDifference::thresholding(Mat inImage, Mat outImage, double thres, double maxval, int type)
{
	threshold(inImage, outImage, thres, maxval, type);
}

void FramesDifference::movingPixels(string txtExport)
{
	//movement is detected if there are more than 0.07% non-zero pixels in frame
	int nonZero=(countNonZero(difference));
	int numberOfPixels=difference.cols*difference.rows;
	const char* txtExp = txtExport.c_str();
	ofstream plik(txtExp, ios::app);
	plik << ElapsedTime <<"\t"<< ((float)nonZero/numberOfPixels) << endl; 
	plik.close();
	if (nonZero>0.0007*numberOfPixels) 
	{
		movement=true;
		counterZero=0;
	}
	else 
	{
		movement=false;
		counterZero++;
	}
	cout<<"\tpixels: "<<numberOfPixels<<"\tnon-zero: "<<nonZero
		<<"\tmovement: \a"<<movement<<endl;
}

void FramesDifference::alarm()
{
	cout<<"ALARM!!!\a\a";
}

Mat FramesDifference::GaussBlurScenario(Mat old, Mat young)
{
	//Mat a, b, difference_helper;
	//GaussianBlur(old, a, Size(21, 21), 0, 0);
	//thresholding(a, a, 4, 255, THRESH_BINARY);
	//GaussianBlur(young, b, Size(21, 21), 0, 0);
	//thresholding(b, b, 4, 255, THRESH_BINARY);
	//absdiff(a, b, difference_helper);
	Mat difference_helper, difference_help;
	absdiff(old, young, difference_helper);
	//difference_helper=old;
	GaussianBlur(difference_helper, difference_help, Size(21, 21), 0, 0);
	thresholding(difference_help, difference_help, 5, 255, THRESH_BINARY);
	return difference_help;
}

//the result is a median of size x size kernel
Mat FramesDifference::MedianBlurScenario(Mat old, Mat young)
{
	Mat difference_helper, difference_help;
	absdiff(old, young, difference_helper);
	//difference_helper=old;
	medianBlur(difference_helper, difference_help, 11);
	thresholding(difference_help, difference_help, 5, 255, THRESH_BINARY);
	return difference_help;
}

//the result is a mean value ??
Mat FramesDifference::BoxBlurScenario(Mat old, Mat young)
{
	Mat difference_helper, difference_help;
	absdiff(old, young, difference_helper);
	//difference_helper=old;
	boxFilter(difference_helper, difference_help, -1, Size(13, 13), Point(-1,-1), true, BORDER_DEFAULT );
	thresholding(difference_help, difference_help, 5, 255, THRESH_BINARY);
	return difference_help;
}

Mat FramesDifference::DilatBlurScenario(Mat old, Mat young)
{
	Mat difference_helper, difference_help;
	absdiff(old, young, difference_helper);
	//difference_helper=old;
	erode(difference_helper, difference_help, Mat(), Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	//dilate(difference_helper, difference_help, Mat(), Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	//thresholding(difference_help, difference_help, 5, 255, THRESH_BINARY);
	return difference_help;
}

Mat FramesDifference::SobelBlurScenario(Mat old, Mat young)
{
	Mat difference_helper, difference_help, difference_help2, difference_help3,difference_help4, difference_help5;
	absdiff(old, young, difference_helper);
	//difference_helper=old;
	boxFilter(difference_helper, difference_help, -1, Size(13, 13), Point(-1,-1), true, BORDER_DEFAULT );
	thresholding(difference_help, difference_help, 5, 255, THRESH_BINARY);
	erode(difference_help, difference_help2, Mat(3, 3, difference_help.type()), Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	dilate(difference_help2, difference_help3, Mat(7, 7, difference_help.type()), Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	erode(difference_help3, difference_help4, Mat(5, 5, difference_help.type()), Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	dilate(difference_help4, difference_help5, Mat(11, 11, difference_help.type()), Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	//Sobel(difference_help, difference_help2, difference_help.depth(), 1, 1, 3, 1, 0, BORDER_DEFAULT);
	//boxFilter(difference_helper, difference_help, -1, Size(13, 13), Point(-1,-1), true, BORDER_DEFAULT );
	//thresholding(difference_help, difference_help, 5, 255, THRESH_BINARY);
	return difference_help5;
}
