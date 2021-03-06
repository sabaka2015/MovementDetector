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
	float * colsWeights;
	float * rowsWeights;
	float * Weights;
	
	public:
	static short counterZero;
	static float ElapsedTime;
	FramesDifference(Mat old, Mat young, Mat first, int scenario,
			string txtExport, short substractionType, bool ifWeights,
			 short typeOfSum, float firstTimeOfLive);
	Mat getDifference(void) {return difference;};
	void thresholding(Mat, Mat, short, double, int); 
		//the value of threshold is equal to the short number
	void thresholding(Mat, Mat, double, double, int); 
		//the value of threshold is equal to the double number 
		//of standard deviation of the mean value
	void movingPixels(string txtExport);
	static void alarm();
	//scenarios
	Mat GaussBlurScenario(Mat, Mat);
	Mat MedianBlurScenario(Mat, Mat);
	Mat BoxBlurScenario(Mat, Mat);
	Mat DilatBlurScenario(Mat, Mat);
	Mat SobelBlurScenario(Mat, Mat);
	Mat ThresBoxThresScenario(Mat, Mat);
	Mat CannyBlurScenario(Mat, Mat);
	Mat ThresholdScenario(Mat, Mat);
	Mat HistoryScenario(Mat, Mat);
	
	Mat WeightsScenario(Mat, Mat);
	Mat WeightsScenarioThird(Mat, Mat);
	Mat WeightsScenarioFourth(Mat, Mat);
	
	Mat GaussBlurScenarioLiveWeightsColRow(Mat, Mat, Mat);
	Mat BoxBlurScenarioLiveWeightsColRow(Mat, Mat, Mat);
	Mat GaussBlurScenarioLiveWeightsEachPix(Mat, Mat, Mat);
	Mat BoxBlurScenarioLiveWeightsEachPix(Mat, Mat, Mat);
	
	Mat GaussBlurScenarioDelayedWeightsAreas(Mat, Mat, Mat, float);
	Mat BoxBlurScenarioDelayedWeightsAreas(Mat, Mat, Mat, float);
	Mat GaussBlurScenarioDelayedWeightsEachPix(Mat, Mat, Mat, float);
	Mat BoxBlurScenarioDelayedWeightsEachPix(Mat, Mat, Mat, float);
	
	void Histogram(Mat, string);
	//weights matrixes
	Mat HistoryMatrix(Mat);
	
	Mat WeightsMatrix(Mat);
	Mat WeightsMatrix(Mat, Mat, Mat);
	
	Mat WeightsMatrixSecond(Mat);
	Mat WeightsMatrixSecond(Mat, Mat, Mat);
	
	Mat WeightsMatrixThird(Mat);
	Mat WeightsMatrixThird(Mat, Mat, Mat, float);
	
	Mat WeightsMatrixFourth(Mat);
	Mat WeightsMatrixFourth(Mat, Mat, Mat, short&, short&, float&, float&, float);
	
};


Mat selectHighestArea (Mat, float*, short, short, float, float);
Mat printWeights (Mat, float*, short, short, float, float);
Mat multiply (Mat, float*);
Mat multiply (Mat, float*, float*);
Mat multiply (Mat, float*, short, short, float, float);
Mat selectHighestArea (Mat, float*, short, short, float, float);

#endif
