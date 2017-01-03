#include "FramesDifference.h"

short FramesDifference::counterZero=0;
float FramesDifference::ElapsedTime=0;
//Mat FramesDifference::colsWeights=Mat();
//Mat FramesDifference::rowsWeights=Mat();

FramesDifference::FramesDifference(Mat old, Mat young, Mat first, int scenario, string txtExport, short substractionType, bool ifWeights, short typeOfSum)
{
	/*if (ElapsedTime==0.924)
	ofstream plik(txtExp, ios::app);
	plik << ElapsedTime <<"\t"<< ((float)nonZero/numberOfPixels) << endl; 
	plik.close();
	*/
	Mat minued;
	if (substractionType)
		minued=old;
	else
		minued=first;
	if (ifWeights)
	{
		if (typeOfSum)
			scenario+=10;
		else
			scenario+=100;
	}
	switch (scenario)
	{
		//without weights
		case 1:
			difference=GaussBlurScenario(minued, young);
			break;
		case 2:
			difference=MedianBlurScenario(minued, young);
			break;
		case 3:
			difference=BoxBlurScenario(minued, young);
			break;
		case 4:
			difference=DilatBlurScenario(minued, young);
			break;
		case 5:
			difference=SobelBlurScenario(minued, young);
			break;
		case 6:
			difference=ThresBoxThresScenario(minued, young);
			break;
		/*case 7:
			difference=HistoryScenario(old, young);
			break;
		case 8:
			difference=WeightsScenario(old, young);
			break;
		case 9:
			difference=WeightsScenarioThird(old, young);
			break;
		case 10:
			difference=WeightsScenarioFourth(old, young);
			break;
		*/
		//with weights "live"
		#if 0
		case 11:
			difference=GaussBlurScenario(old, young);
			break;
		case 12:
			difference=MedianBlurScenario(old, young);
			break;
		case 13:
			difference=BoxBlurScenario(old, young);
			break;
		case 14:
			difference=DilatBlurScenario(old, young);
			break;
		case 15:
			difference=SobelBlurScenario(old, young);
			break;
		case 16:
			difference=ThresBoxThresScenario(old, young);
			break;
		//with delayed weights
		case 101:
			difference=GaussBlurScenario(old, young);
			break;
		case 102:
			difference=MedianBlurScenario(old, young);
			break;
		case 103:
			difference=BoxBlurScenario(old, young);
			break;
		case 104:
			difference=DilatBlurScenario(old, young);
			break;
		case 105:
			difference=SobelBlurScenario(old, young);
			break;
		case 106:
			difference=ThresBoxThresScenario(old, young);
			break;
		#endif
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

void FramesDifference::thresholding(Mat inImage, Mat outImage, short thres, double maxval, int type)
{
	threshold(inImage, outImage, thres, maxval, type);
}

void FramesDifference::thresholding(Mat inImage, Mat outImage, double thres, double maxval, int type)
{
	float stdDeviation=0;
	int amount=inImage.cols*inImage.rows;
	for (int i=0; i<inImage.rows; i++)
	{
		for (int j=0; j<inImage.cols; j++)
		{
			stdDeviation+=pow((float)inImage.at<uchar>(i,j), 2);
		}
	}
	stdDeviation/=(amount-1);
	stdDeviation=sqrt(stdDeviation);
	cout<<"!!"<<stdDeviation<<"!"<<(thres*stdDeviation)<<endl;
	threshold(inImage, outImage, (thres*stdDeviation), maxval, type);
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

//this function is making a histogram from values in pixels in frame
void FramesDifference::Histogram(Mat frame, string name)
{
	for (int k=0; k<256; k++)
	{
		int counter=0;
		for (int i=0; i<frame.cols; i++)
		{
			for (int j=0; j<frame.rows; j++)
			{
				if (frame.at<uchar>(i,j)==k)
					counter++;
			}
		}
		const char* name2 = name.c_str();
		ofstream plik(name2, ios::app);
		plik << k << "\t" << counter << endl; 
		plik.close();
	}
	ofstream skrypt("skryptHist.txt");
	skrypt << "set yrange [0:130000]\n";
	skrypt << "set xrange [0:28]\n";
	skrypt << "plot \"" << name <<"\" with linespoints pt 2 ps 2 lt 2 lw 1";
	skrypt.close();
	system("gnuplot -p -e \"load 'skryptHist.txt'\"");
}

//to slow
Mat multiply (Mat frame, float* weights)
{
	//Mat result;
	for (int i=0; i<frame.rows; i++)
	{
		for (int j=0; j<frame.cols; j++)
		{
			frame.at<uchar>(i,j)*=weights[i*frame.cols+j];
			//result.at<uchar>(i,j)=frame.at<uchar>(i,j)*weights.at<uchar>(i,j);
			//result.at<int>(i,j)=(int)frame.at<uchar>(i,j);
			//cout<<i<<" "<<j<<" "<<result.at<int>(i,j)<<endl;
			//cout<<"cos";
		}
	}
	//cout<<"cos";
	return frame;
}
	
Mat multiply (Mat frame, float* colsWeights, float* rowsWeights)
{
	for (int i=0; i<frame.cols; i++)
	{
		frame.col(i)*=colsWeights[i];//(float)colsWeights.at<uchar>(1,i);
		//cout<<"!! "<<colsWeights.at<float>(1,i)<<"!! ";
	}
	for (int j=0; j<frame.rows; j++)
	{
		frame.row(j)*=rowsWeights[j];//(float)rowsWeights.at<uchar>(j,1);
	}
	return frame;
}

Mat multiply (Mat frame, float* weights, short yDist, short xDist, float piksOnYDist, float piksOnXDist)
{
	for (int i=0; i<yDist; i++)
	{
		for (int j=0; j<xDist; j++)
		{
			for (int l=0; l<piksOnXDist; l++/*int k=0; k<piksOnYDist; k++*/)
			{
				for (int k=0; k<piksOnYDist; k++/*int l=0; l<piksOnXDist; l++*/)
				{
					frame.at<uchar>(k+i*piksOnYDist, l+j*piksOnXDist)*=weights[i*xDist+j];
					//tab[i*xDist+j]+=(float)frame.at<uchar>(k+i*yDist,l+j*xDist);
				}
			}
		}
	}
	cout<<"\twymnozone\t";
	//Mat result;
	#if 0
	for (int i=0; i<frame.rows; i++)
	{
		for (int j=0; j<frame.cols; j++)
		{
			frame.at<uchar>(i,j)*=weights[i*frame.cols+j];
			//result.at<uchar>(i,j)=frame.at<uchar>(i,j)*weights.at<uchar>(i,j);
			//result.at<int>(i,j)=(int)frame.at<uchar>(i,j);
			//cout<<i<<" "<<j<<" "<<result.at<int>(i,j)<<endl;
			//cout<<"cos";
		}
	}
	//cout<<"cos";
	#endif
	return frame;//selectHighestArea (frame, weights, yDist, xDist, piksOnYDist, piksOnXDist);
}

Mat selectHighestArea (Mat frame, float* weights, short yDist, short xDist, float piksOnYDist, float piksOnXDist)
{
	float max=0;
	short xMax, yMax;
	for (int i=0; i<yDist; i++)
	{
		for (int j=0; j<xDist; j++)
		{
			if (weights[i*xDist+j]>max)
			{
				max=weights[i*xDist+j];
				xMax=j;
				yMax=i;
			}
		}		
	}	
	rectangle(frame, Point(xMax*piksOnXDist, yMax*piksOnYDist), Point((xMax+1)*piksOnXDist, (yMax+1)*piksOnYDist), Scalar(255, 0, 0), 1, 8, 0);
	return frame;
}
