#ifndef FramesDifference_h
#define FramesDifference_h

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
 
using namespace std;
using namespace cv;

class FramesDifference
{
	Mat difference;
	bool movement;
	//static Mat colsWeights;
	//static Mat rowsWeights;
	float * colsWeights;
	float * rowsWeights;
	float * Weights;
	public:
	static short counterZero;
	static float ElapsedTime;
	FramesDifference(Mat old, Mat young, int scenario, string txtExport);
	Mat getDifference(void) {return difference;};
	void thresholding(Mat, Mat, short, double, int); 
		//threshold is equal to the short number
	void thresholding(Mat, Mat, double, double, int); 
		//threshold is equal to the double number of standard deviation of mean value
	void movingPixels(string txtExport);
	static void alarm();
	Mat GaussBlurScenario(Mat, Mat);
	Mat MedianBlurScenario(Mat, Mat);
	Mat BoxBlurScenario(Mat, Mat);
	Mat DilatBlurScenario(Mat, Mat);
	Mat SobelBlurScenario(Mat, Mat);
	Mat ThresBoxThresScenario(Mat, Mat);
	Mat HistoryScenario(Mat, Mat);
	Mat WeightsScenario(Mat, Mat);
	Mat WeightsScenarioThird(Mat, Mat);
	void Histogram(Mat, string);
	Mat HistoryMatrix(Mat);
	Mat WeightsMatrix(Mat);
	Mat WeightsMatrixSecond(Mat);
	Mat WeightsMatrixThird(Mat);
	
};

#endif
