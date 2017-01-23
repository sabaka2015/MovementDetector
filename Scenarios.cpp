#include "FramesDifference.h"

Mat FramesDifference::GaussBlurScenario(Mat minued, Mat young)
{
	Mat difference_helper, difference_help;
	absdiff(minued, young, difference_helper);
	GaussianBlur(difference_helper, difference_help, Size(23, 23), 0, 0);
	thresholding(difference_help, difference_help, (short)5, 255, THRESH_BINARY);
	return difference_help;
}

//the result is a median of size x size kernel
Mat FramesDifference::MedianBlurScenario(Mat minued, Mat young)
{
	Mat difference_helper, difference_help;
	absdiff(minued, young, difference_helper);
	medianBlur(difference_helper, difference_help, 15);
	thresholding(difference_help, difference_help, (short)4, 255, THRESH_BINARY);
	return difference_help;
}

//the result is a mean value ??
Mat FramesDifference::BoxBlurScenario(Mat minued, Mat young)
{
	Mat difference_helper, difference_help;
	//wszystkie if są do wycięcia w ostatecznej wersji programu
	if ((ElapsedTime>=2.5)&&(ElapsedTime<=2.6))
	{
		//Histogram(old, "before2");
		//ofstream plik2 ("before0.924.txt", ios::app);
		//plik2 << old ; 
		//plik2.close();
	}
	absdiff(minued, young, difference_helper);
	if ((ElapsedTime>=2.5)&&(ElapsedTime<=2.6))
	{
		//Histogram(difference_helper, "afterAbs2");
		//ofstream plik2 ("afterAbs0.924.txt", ios::app);
		//plik2 << difference_helper ; 
		//plik2.close();
	}
	//difference_helper=old;
	boxFilter(difference_helper, difference_help, -1, Size(17, 17), Point(-1,-1), true, BORDER_DEFAULT );
	if ((ElapsedTime>=2.5)&&(ElapsedTime<=2.6))
	{
		//Histogram(difference_help, "afterBox2");
		//ofstream plik2 ("afterBox0.924.txt", ios::app);
		//plik2 << difference_help ; 
		//plik2.close();
	}
	//warto: 13x13 4, 5, 6, 17x17 4, 5, 21x21
	thresholding(difference_help, difference_help, (short)5, 255, THRESH_BINARY);
	return difference_help;
}

Mat FramesDifference::DilatBlurScenario(Mat minued, Mat young)
{
	Mat difference_helper, difference_help, difference_help2;
	absdiff(minued, young, difference_helper);
	//erode(difference_helper, difference_help, Mat(), Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	dilate(difference_helper, difference_help, Mat(), Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	thresholding(difference_help, difference_help, (short)20, 255, THRESH_BINARY);
	return difference_help;
}

Mat FramesDifference::SobelBlurScenario(Mat minued, Mat young)
{
	Mat difference_helper, difference_help, difference_help2, difference_help3,difference_help4, difference_help5;
	absdiff(minued, young, difference_helper);
	//boxFilter(difference_helper, difference_help, -1, Size(13, 13), Point(-1,-1), true, BORDER_DEFAULT );
	//thresholding(difference_help, difference_help, (short)5, 255, THRESH_BINARY);
	//erode(difference_help, difference_help2, Mat()/*Mat(3, 3, difference_help.type())*/, Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	//dilate(difference_help2, difference_help3, Mat(5, 5, difference_help.type()), Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	//erode(difference_help3, difference_help4, Mat(5, 5, difference_help.type()), Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	//dilate(difference_help4, difference_help5, Mat(7, 7, difference_help.type()), Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	Sobel(difference_helper, difference_help, difference_helper.depth(), 1, 1, 3, 1, 0, BORDER_DEFAULT);
	//boxFilter(difference_helper, difference_help, -1, Size(13, 13), Point(-1,-1), true, BORDER_DEFAULT );
	thresholding(difference_help, difference_help, (short)16, 255, THRESH_BINARY);
	return difference_help;
}

Mat FramesDifference::ThresBoxThresScenario(Mat minued, Mat young)
{
	Mat difference_helper, difference_help, difference_help2, difference_help3;
	absdiff(minued, young, difference_helper);
	thresholding(difference_helper, difference_helper, (double)3, 255, THRESH_TOZERO);
	boxFilter(difference_helper, difference_help, -1, Size(13, 13), Point(-1,-1), true, BORDER_DEFAULT );
	thresholding(difference_help, difference_help, (double)10, 255, THRESH_BINARY);
	erode(difference_help, difference_help2, Mat(), Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	dilate(difference_help2, difference_help3, Mat(5, 5, difference_help.type()), Point(-1,-1), 3, BORDER_CONSTANT, morphologyDefaultBorderValue());
	return difference_help3;
}

Mat FramesDifference::CannyBlurScenario(Mat minued, Mat young)
{
	Mat difference_helper, difference_help;//, difference_help2, difference_help3,difference_help4, difference_help5;
	absdiff(minued, young, difference_helper);
	//boxFilter(difference_helper, difference_help, -1, Size(13, 13), Point(-1,-1), true, BORDER_DEFAULT );
	//thresholding(difference_help, difference_help, (short)5, 255, THRESH_BINARY);
	//erode(difference_help, difference_help2, Mat()/*Mat(3, 3, difference_help.type())*/, Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	//dilate(difference_help2, difference_help3, Mat(5, 5, difference_help.type()), Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	//erode(difference_help3, difference_help4, Mat(5, 5, difference_help.type()), Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	//dilate(difference_help4, difference_help5, Mat(7, 7, difference_help.type()), Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	Canny(difference_helper, difference_help, 15, 30, 3, false );
	//Sobel(difference_helper, difference_help, difference_helper.depth(), 1, 1, 3, 1, 0, BORDER_DEFAULT);
	//boxFilter(difference_helper, difference_help, -1, Size(13, 13), Point(-1,-1), true, BORDER_DEFAULT );
	//thresholding(difference_help, difference_help, (short)16, 255, THRESH_BINARY);
	return difference_help;
}

Mat FramesDifference::ThresholdScenario(Mat minued, Mat young)
{
	Mat difference_helper;//, difference_help;//, difference_help2, difference_help3,difference_help4, difference_help5;
	absdiff(minued, young, difference_helper);
	//boxFilter(difference_helper, difference_help, -1, Size(13, 13), Point(-1,-1), true, BORDER_DEFAULT );
	//thresholding(difference_help, difference_help, (short)5, 255, THRESH_BINARY);
	//erode(difference_help, difference_help2, Mat()/*Mat(3, 3, difference_help.type())*/, Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	//dilate(difference_help2, difference_help3, Mat(5, 5, difference_help.type()), Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	//erode(difference_help3, difference_help4, Mat(5, 5, difference_help.type()), Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	//dilate(difference_help4, difference_help5, Mat(7, 7, difference_help.type()), Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	//Canny(difference_helper, difference_help, 20, 100, 3, false );
	//Sobel(difference_helper, difference_help, difference_helper.depth(), 1, 1, 3, 1, 0, BORDER_DEFAULT);
	//boxFilter(difference_helper, difference_help, -1, Size(13, 13), Point(-1,-1), true, BORDER_DEFAULT );
	thresholding(difference_helper, difference_helper, (short)16, 255, THRESH_BINARY);
	return difference_helper;
}


#if 0
//bardzo kiepski pomysł, do wyrzucenia z ostatecznej wersji programu
Mat FramesDifference::HistoryScenario(Mat old, Mat young)
{
	Mat difference_helper, difference_help;
	absdiff(old, young, difference_helper);
	//difference_helper=old;
	difference_helper=HistoryMatrix(difference_helper);
	//boxFilter(difference_helper, difference_help, -1, Size(13, 13), Point(-1,-1), true, BORDER_DEFAULT );
	thresholding(difference_helper, difference_helper, (short)30, 255, THRESH_BINARY);
	return difference_helper;
}
#endif

Mat FramesDifference::GaussBlurScenarioLiveWeightsColRow(Mat minued, Mat young, Mat old)
{
	Mat difference_helper, difference_help, difference_help2;
	absdiff(minued, young, difference_helper);
	difference_help=WeightsMatrix(difference_helper, old, young);
	GaussianBlur(difference_help, difference_help2, Size(15, 15), 0, 0);
	thresholding(difference_help2, difference_help2, (double)3, 255, THRESH_BINARY);
	return difference_help2;
}

Mat FramesDifference::BoxBlurScenarioLiveWeightsColRow(Mat minued, Mat young, Mat old)
{
	Mat difference_helper, difference_help, difference_help2;
	absdiff(minued, young, difference_helper);
	difference_help=WeightsMatrix(difference_helper, old, young);
	boxFilter(difference_help, difference_help2, -1, Size(13, 13), Point(-1,-1), true, BORDER_DEFAULT );
	thresholding(difference_help2, difference_help2, (double)2, 255, THRESH_BINARY);
	return difference_help2;
}

Mat FramesDifference::GaussBlurScenarioLiveWeightsEachPix(Mat minued, Mat young, Mat old)
{
	Mat difference_helper, difference_help, difference_help2;
	absdiff(minued, young, difference_helper);
	difference_help=WeightsMatrixSecond(difference_helper, old, young);
	GaussianBlur(difference_help, difference_help2, Size(15, 15), 0, 0);
	thresholding(difference_help2, difference_help2, (double)3, 255, THRESH_BINARY);
	return difference_help2;
}

Mat FramesDifference::BoxBlurScenarioLiveWeightsEachPix(Mat minued, Mat young, Mat old)
{
	Mat difference_helper, difference_help, difference_help2;
	absdiff(minued, young, difference_helper);
	difference_help=WeightsMatrixSecond(difference_helper, old, young);
	boxFilter(difference_help, difference_help2, -1, Size(13, 13), Point(-1,-1), true, BORDER_DEFAULT );
	thresholding(difference_help2, difference_help2, (double)2, 255, THRESH_BINARY);
	return difference_help2;
}

Mat FramesDifference::GaussBlurScenarioDelayedWeightsAreas(Mat minued, Mat young, Mat first, float firstTimeOfLive)
{
	Mat difference_helper, difference_help, difference_help2, difference_help3;
	short yDist, xDist;
	float piksOnYDist, piksOnXDist;
	absdiff(minued, young, difference_helper);
	difference_help=WeightsMatrixFourth(difference_helper, first, young, yDist, xDist, piksOnYDist, piksOnXDist, firstTimeOfLive);
	GaussianBlur(difference_help, difference_help2, Size(23, 23), 0, 0);
	thresholding(difference_help2, difference_help2, (short)3, 255, THRESH_BINARY);
	//to see rectangles on real video, you can comment it
	//young.copyTo(difference_help2); 
	//difference_help3=selectHighestArea(difference_help2, Weights, yDist, xDist, piksOnYDist, piksOnXDist);
	//to print weights on real video, you can commit it
	//difference_help3=printWeights(difference_help2, Weights, yDist, xDist, piksOnYDist, piksOnXDist);
	//thresholding(difference_help3, difference_help3, (short)4, 255, THRESH_BINARY_INV);
	return difference_help2;
}

Mat FramesDifference::BoxBlurScenarioDelayedWeightsAreas(Mat minued, Mat young, Mat first, float firstTimeOfLive)
{
	Mat difference_helper, difference_help, difference_help2, difference_help3;
	short yDist, xDist;
	float piksOnYDist, piksOnXDist;
	absdiff(minued, young, difference_helper);
	difference_help=WeightsMatrixFourth(difference_helper, first, young, yDist, xDist, piksOnYDist, piksOnXDist, firstTimeOfLive);
	boxFilter(difference_help, difference_help2, -1, Size(21, 21), Point(-1,-1), true, BORDER_DEFAULT );
	thresholding(difference_help2, difference_help2, (short)2, 255, THRESH_BINARY);
	//young.copyTo(difference_help2); //to see rectangles on real video, you can comment it
	//difference_help3=selectHighestArea(difference_help2, Weights, yDist, xDist, piksOnYDist, piksOnXDist);
	//to print weights on real video, you can commit it
	//difference_help3=printWeights(difference_help2, Weights, yDist, xDist, piksOnYDist, piksOnXDist);
	//thresholding(difference_help3, difference_help3, (short)4, 255, THRESH_BINARY_INV);
	return difference_help2;
}

Mat FramesDifference::GaussBlurScenarioDelayedWeightsEachPix(Mat minued, Mat young, Mat first, float firstTimeOfLive)
{
	Mat difference_helper, difference_help, difference_help2;
	absdiff(minued, young, difference_helper);
	difference_help=WeightsMatrixThird(difference_helper, first, young, firstTimeOfLive);
	GaussianBlur(difference_help, difference_help2, Size(15, 15), 0, 0);
	thresholding(difference_help2, difference_help2, (double)3, 255, THRESH_BINARY);
	return difference_help2;
}

Mat FramesDifference::BoxBlurScenarioDelayedWeightsEachPix(Mat minued, Mat young, Mat first, float firstTimeOfLive)
{
	Mat difference_helper, difference_help, difference_help2;
	absdiff(minued, young, difference_helper);
	difference_help=WeightsMatrixThird(difference_helper, first, young, firstTimeOfLive);
	boxFilter(difference_help, difference_help2, -1, Size(13, 13), Point(-1,-1), true, BORDER_DEFAULT );
	thresholding(difference_help2, difference_help2, (double)3, 255, THRESH_BINARY);
	return difference_help2;
}
