#include "FramesDifference.h"

short FramesDifference::counterZero=0;
float FramesDifference::ElapsedTime=0;

FramesDifference::FramesDifference(Mat old, Mat young, Mat first,
			int scenario, string txtExport, short substractionType,
			 bool ifWeights, short typeOfSum, float firstTimeOfLive)
{
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
			cout<<"GaussBlurScenario\n";
			difference=GaussBlurScenario(minued, young);
			break;
		case 2:
			cout<<"MedianBlurScenario\n";
			difference=MedianBlurScenario(minued, young);
			break;
		case 3:
			cout<<"BoxBlurScenario\n";
			difference=BoxBlurScenario(minued, young);
			break;
		case 4:
			cout<<"DilatBlurScenario\n";
			difference=DilatBlurScenario(minued, young);
			break;
		case 5:
			cout<<"SobelBlurScenario\n";
			difference=SobelBlurScenario(minued, young);
			break;
		case 6:
			cout<<"ThresBoxThresScenario\n";
			difference=ThresBoxThresScenario(minued, young);
			break;
		case 7:
			cout<<"CannyBlurScenario\n";
			difference=CannyBlurScenario(minued, young);
			break;
		case 8:
			cout<<"ThresholdScenario\n";
			difference=ThresholdScenario(minued, young);
			break;

		//with weights "live"
		case 11:
			cout<<"GaussBlurScenarioLiveWeightsColRow\n";
			difference=GaussBlurScenarioLiveWeightsColRow(minued, young, old);
			break;
		case 13:
			cout<<"BoxBlurScenarioLiveWeightsColRow\n";
			difference=BoxBlurScenarioLiveWeightsColRow(minued, young, old);
			break;
		case 17:
			cout<<"GaussBlurScenarioLiveWeightsEachPix\n";
			difference=GaussBlurScenarioLiveWeightsEachPix(minued, young, old);
			break;
		case 19:
			cout<<"BoxBlurScenarioLiveWeightsEachPix\n";
			difference=BoxBlurScenarioLiveWeightsEachPix(minued, young, old);
			break;
			
		//with delayed weights
		case 101:
			cout<<"GaussBlurScenarioDelayedWeightsAreas\n";
			difference=GaussBlurScenarioDelayedWeightsAreas(minued, young,
										first, firstTimeOfLive);
			break;
		case 103:
			cout<<"BoxBlurScenarioDelayedWeightsAreas\n";
			difference=BoxBlurScenarioDelayedWeightsAreas(minued, young, 
										first, firstTimeOfLive);
			break;
		case 107:
			cout<<"GaussBlurScenarioDelayedWeightsEachPix\n";
			difference=GaussBlurScenarioDelayedWeightsEachPix(minued, young, 
										first, firstTimeOfLive);
			break;
		case 109:
			cout<<"BoxBlurScenarioDelayedWeightsEachPix\n";
			difference=BoxBlurScenarioDelayedWeightsEachPix(minued, young, 
										first, firstTimeOfLive);
			break;
	}
	
	movingPixels(txtExport);
}

//the value of threshold is equal to the short number
void FramesDifference::thresholding(Mat inImage, Mat outImage, short thres, 
											double maxval, int type)
{
	threshold(inImage, outImage, thres, maxval, type);
}

//the value of threshold is equal to the double number
//of standard deviation of a mean value
//assumption, that the mean value is equal 0 
//(it is true if there is not threshold before)
void FramesDifference::thresholding(Mat inImage, Mat outImage, double thres,
											double maxval, int type)
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

//movement is detected if there are more than 0.07% non-zero pixels in frame
void FramesDifference::movingPixels(string txtExport)
{
	int nonZero=(countNonZero(difference));
	int numberOfPixels=difference.cols*difference.rows;
	//export data to the txt file
	const char* txtExp = txtExport.c_str();
	ofstream plik(txtExp, ios::app);
	plik << ElapsedTime <<"\t"<< ((float)nonZero/numberOfPixels) << endl; 
	plik.close();
	//stwierdzenie wykrycia ruchu- można usunąć z ostatecznej wersji programu
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

//można usunąć z ostatecznej wersji programu
void FramesDifference::alarm()
{
	cout<<"ALARM!!!\a\a";
}

//można usunąć z ostatecznej wersji programu
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

//to slow??
Mat multiply (Mat frame, float* weights)
{
	for (int i=0; i<frame.rows; i++)
	{
		for (int j=0; j<frame.cols; j++)
		{
			frame.at<uchar>(i,j)*=weights[i*frame.cols+j];
		}
	}
	return frame;
}
	
Mat multiply (Mat frame, float* colsWeights, float* rowsWeights)
{
	for (int i=0; i<frame.cols; i++)
	{
		frame.col(i)*=colsWeights[i];
	}
	for (int j=0; j<frame.rows; j++)
	{
		frame.row(j)*=rowsWeights[j];
	}
	return frame;
}

Mat multiply (Mat frame, float* weights, short yDist, short xDist, float piksOnYDist, float piksOnXDist)
{
	for (int i=0; i<yDist; i++)
	{
		for (int j=0; j<xDist; j++)
		{
			for (int l=0; l<piksOnXDist; l++)
			{
				for (int k=0; k<piksOnYDist; k++)
				{
					frame.at<uchar>(k+i*piksOnYDist, l+j*piksOnXDist)*=weights[i*xDist+j];
				}
			}
		}
	}

	return frame;
}

//work only for DelayedWeightsAreas scenarios
//the highest areas are pointed
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
	rectangle(frame, Point(xMax*piksOnXDist, yMax*piksOnYDist), Point((xMax+1)*piksOnXDist, (yMax+1)*piksOnYDist), Scalar(255, 0, 0), 3, 8, 0);
	for (int i=0; i<yDist; i++)
	{
		for (int j=0; j<xDist; j++)
		{
			if (weights[i*xDist+j]>(0.7*max))
			{
					rectangle(frame, Point(j*piksOnXDist, i*piksOnYDist), Point((j+1)*piksOnXDist, (i+1)*piksOnYDist), Scalar(255, 0, 0), 2, 8, 0);
			}
			else
			{
				if (weights[i*xDist+j]>(0.6*max))
				{
					rectangle(frame, Point(j*piksOnXDist, i*piksOnYDist), Point((j+1)*piksOnXDist, (i+1)*piksOnYDist), Scalar(255, 0, 0), 1, 8, 0);
				}
			}		
		}
	}	
	return frame;
}

//printing weights on the frame
Mat printWeights (Mat frame, float* weights, short yDist, short xDist, float piksOnYDist, float piksOnXDist)
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
	rectangle(frame, Point(xMax*piksOnXDist, yMax*piksOnYDist), Point((xMax+1)*piksOnXDist, (yMax+1)*piksOnYDist), Scalar(255, 0, 0), 3, 8, 0);
	
	for (int i=0; i<yDist; i++)
	{
		for (int j=0; j<xDist; j++)
		{
			char str[20]  = "";
			sprintf(str, "%.2f", weights[i*xDist+j]);
			putText(frame, str, Point(j*piksOnXDist+2, (i+0.5)*piksOnYDist), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 0, 0), 1, 8, false );
			if (weights[i*xDist+j]>(0.55*max))
			{
					rectangle(frame, Point(j*piksOnXDist, i*piksOnYDist), Point((j+1)*piksOnXDist, (i+1)*piksOnYDist), Scalar(255, 0, 0), 2, 8, 0);
			}
			else
				rectangle(frame, Point(j*piksOnXDist, i*piksOnYDist), Point((j+1)*piksOnXDist, (i+1)*piksOnYDist), Scalar(255, 0, 0), 1, 8, 0);		
		}
	}	
	return frame;
}
