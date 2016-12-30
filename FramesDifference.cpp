#include "FramesDifference.h"

short FramesDifference::counterZero=0;
float FramesDifference::ElapsedTime=0;
//Mat FramesDifference::colsWeights=Mat();
//Mat FramesDifference::rowsWeights=Mat();

FramesDifference::FramesDifference(Mat old, Mat young, int scenario, string txtExport)
{
	/*if (ElapsedTime==0.924)
	ofstream plik(txtExp, ios::app);
	plik << ElapsedTime <<"\t"<< ((float)nonZero/numberOfPixels) << endl; 
	plik.close();
	*/
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
		case 6:
			difference=ThresBoxThresScenario(old, young);
			break;
		case 7:
			difference=HistoryScenario(old, young);
			break;
		case 8:
			difference=WeightsScenario(old, young);
			break;
		case 9:
			difference=WeightsScenarioThird(old, young);
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
	thresholding(difference_help, difference_help, (double)2.5, 255, THRESH_BINARY);
	return difference_help;
}

//the result is a median of size x size kernel
Mat FramesDifference::MedianBlurScenario(Mat old, Mat young)
{
	Mat difference_helper, difference_help;
	absdiff(old, young, difference_helper);
	//difference_helper=old;
	medianBlur(difference_helper, difference_help, 11);
	thresholding(difference_help, difference_help, (short)5, 255, THRESH_BINARY);
	return difference_help;
}

//the result is a mean value ??
Mat FramesDifference::BoxBlurScenario(Mat old, Mat young)
{
	Mat difference_helper, difference_help;
	if ((ElapsedTime>=2.5)&&(ElapsedTime<=2.6))
	{
		//Histogram(old, "before2");
		//ofstream plik2 ("before0.924.txt", ios::app);
		//plik2 << old ; 
		//plik2.close();
	}
	absdiff(old, young, difference_helper);
	if ((ElapsedTime>=2.5)&&(ElapsedTime<=2.6))
	{
		//Histogram(difference_helper, "afterAbs2");
		//ofstream plik2 ("afterAbs0.924.txt", ios::app);
		//plik2 << difference_helper ; 
		//plik2.close();
	}
	//difference_helper=old;
	boxFilter(difference_helper, difference_help, -1, Size(13, 13), Point(-1,-1), true, BORDER_DEFAULT );
	if ((ElapsedTime>=2.5)&&(ElapsedTime<=2.6))
	{
		//Histogram(difference_help, "afterBox2");
		//ofstream plik2 ("afterBox0.924.txt", ios::app);
		//plik2 << difference_help ; 
		//plik2.close();
	}
	thresholding(difference_help, difference_help, (double)3, 255, THRESH_BINARY);
	return difference_help;
}

Mat FramesDifference::DilatBlurScenario(Mat old, Mat young)
{
	Mat difference_helper, difference_help, difference_help2;
	absdiff(old, young, difference_helper);
	//difference_helper=old;
	erode(difference_helper, difference_help, Mat(), Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	dilate(difference_help, difference_help2, Mat(), Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	thresholding(difference_help2, difference_help2, (short)7, 255, THRESH_BINARY);
	return difference_help2;
}

Mat FramesDifference::SobelBlurScenario(Mat old, Mat young)
{
	Mat difference_helper, difference_help, difference_help2, difference_help3,difference_help4, difference_help5;
	absdiff(old, young, difference_helper);
	//difference_helper=old;
	boxFilter(difference_helper, difference_help, -1, Size(13, 13), Point(-1,-1), true, BORDER_DEFAULT );
	thresholding(difference_help, difference_help, (short)5, 255, THRESH_BINARY);
	erode(difference_help, difference_help2, Mat()/*Mat(3, 3, difference_help.type())*/, Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	dilate(difference_help2, difference_help3, Mat(5, 5, difference_help.type()), Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	erode(difference_help3, difference_help4, Mat(5, 5, difference_help.type()), Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	dilate(difference_help4, difference_help5, Mat(7, 7, difference_help.type()), Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	//Sobel(difference_help, difference_help2, difference_help.depth(), 1, 1, 3, 1, 0, BORDER_DEFAULT);
	//boxFilter(difference_helper, difference_help, -1, Size(13, 13), Point(-1,-1), true, BORDER_DEFAULT );
	//thresholding(difference_help, difference_help, 5, 255, THRESH_BINARY);
	return difference_help5;
}

Mat FramesDifference::ThresBoxThresScenario(Mat old, Mat young)
{
	Mat difference_helper, difference_help, difference_help2, difference_help3;
	absdiff(old, young, difference_helper);
	//difference_helper=old;
	thresholding(difference_helper, difference_helper, (double)3, 255, THRESH_TOZERO);
	boxFilter(difference_helper, difference_help, -1, Size(13, 13), Point(-1,-1), true, BORDER_DEFAULT );
	thresholding(difference_help, difference_help, (double)10, 255, THRESH_BINARY);
	erode(difference_help, difference_help2, Mat(), Point(-1,-1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	dilate(difference_help2, difference_help3, Mat(5, 5, difference_help.type()), Point(-1,-1), 3, BORDER_CONSTANT, morphologyDefaultBorderValue());
	return difference_help3;
}

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

Mat FramesDifference::WeightsScenario(Mat old, Mat young)
{
	Mat difference_helper, difference_help, difference_help2;
	absdiff(old, young, difference_helper);
	//difference_helper=old;
	difference_help=WeightsMatrixSecond(difference_helper);	
	//thresholding(difference_helper, difference_helper, (double)1, 255, THRESH_BINARY);
	//erode(difference_helper, difference_help, Mat(), Point(-1,-1), 3, BORDER_CONSTANT, morphologyDefaultBorderValue());
	boxFilter(difference_helper, difference_help, -1, Size(13, 13), Point(-1,-1), true, BORDER_DEFAULT );
	thresholding(difference_help, difference_help, (double)12, 255, THRESH_BINARY);
	return difference_help;
}

Mat FramesDifference::WeightsScenarioThird(Mat old, Mat young)
{
	Mat difference_helper, difference_help, difference_help2;
	absdiff(old, young, difference_helper);
	//difference_helper=old;
	difference_help=WeightsMatrixThird(difference_helper);	
	//thresholding(difference_helper, difference_helper, (double)1, 255, THRESH_BINARY);
	//erode(difference_helper, difference_help, Mat(), Point(-1,-1), 3, BORDER_CONSTANT, morphologyDefaultBorderValue());
	boxFilter(difference_helper, difference_help, -1, Size(13, 13), Point(-1,-1), true, BORDER_DEFAULT );
	thresholding(difference_help, difference_help, (double)3, 255, THRESH_BINARY);
	return difference_help;
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

Mat FramesDifference::HistoryMatrix(Mat frame)
{
	Mat weights;
	if (ElapsedTime==0)
	{
		//weights=Mat(frame.rows, frame.cols, CV_32FC1);
		weights=frame;
	}
	else
	{
		weights=weights/2+frame/2;
	}
	
	return weights;
}

Mat FramesDifference::WeightsMatrix(Mat frame)
{
	if (ElapsedTime==0)
	{
		//weights=Mat(frame.rows, frame.cols, CV_32FC1);
		//colsWeights=frame.row(1);
		//rowsWeights=frame.col(1);
		//colsWeights=1;
		//rowsWeights=1;
		colsWeights=new float[frame.rows];
		rowsWeights=new float[frame.cols];
		for (int i=0; i<frame.rows; i++)
			colsWeights[i]=1;
		for (int i=0; i<frame.cols; i++)
			rowsWeights[i]=1;	
		//cout<<"uwaga!"<<colsWeights.rows<<"\t";
		//cout<<colsWeights.cols<<endl;
		//cout<<"uwaga!"<<rowsWeights.rows<<"\t";
		//cout<<rowsWeights.cols<<endl;
	}
	else
	{
		int colMax=0, rowMax=0;
		float colTab[frame.rows]={0}, rowTab[frame.cols]={0};
		for (int i=0; i<frame.rows; i++)
		{
			for (int j=0; j<frame.cols; j++)
			{
				colTab[i]+=(float)frame.at<uchar>(i,j);
				//cout<<"!!!"<<colTab[i]<<"!!"<<endl;
				//if (i==478) cout<<"! "<<(float)frame.at<uchar>(i, j)<<"! ";
			}
			if (colTab[i]>colMax) colMax=colTab[i];
			//if (i==478) cout<<"@ "<<colTab[i]<<" @ ";
			//cout<<"! "<<(float)frame.at<uchar>(638, 478)<<"! ";
		}
		for (int i=0; i<frame.rows; i++)
		{
			colsWeights[i]+=(colTab[i]/colMax);
			colsWeights[i]/=2;
			//cout<<"@ "<<colsWeights[i]<<" @ ";
		}
		
		for (int i=0; i<frame.cols; i++)
		{
			for (int j=0; j<frame.rows; j++)
			{
				rowTab[i]+=(float)frame.at<uchar>(j,i);	
			}
			if (rowTab[i]>rowMax) rowMax=rowTab[i];
		}
		//cout<<"!!!!"<<endl;
		for (int i=0; i<frame.cols; i++)
		{
			rowsWeights[i]+=(rowTab[i]/rowMax);
			rowsWeights[i]/=2;
			//cout<<rowsWeights[i]<<" "<<i<<" ";
		}
		//cout<<"!!!!"<<endl;
		//weights=weights/2+frame/2;
	}
	return multiply(frame, colsWeights, rowsWeights);
}

Mat FramesDifference::WeightsMatrixSecond(Mat frame)
{
	int amount=frame.rows*frame.cols;
	if (ElapsedTime==0)
	{
		//weights=Mat(frame.rows, frame.cols, CV_32FC1);
		//colsWeights=frame.row(1);
		//rowsWeights=frame.col(1);
		//colsWeights=1;
		//rowsWeights=1;
		Weights=new float[amount];
		for (int i=0; i<amount; i++)
			Weights[i]=1;
		//cout<<"uwaga!"<<colsWeights.rows<<"\t";
		//cout<<colsWeights.cols<<endl;
		//cout<<"uwaga!"<<rowsWeights.rows<<"\t";
		//cout<<rowsWeights.cols<<endl;
		cout<<"hooop";
	}
	else
	{
		//int colMax=0, rowMax=0;
		int max=0;
		float tab[amount]={0};
		//float colTab[frame.rows]={0}, rowTab[frame.cols]={0};
		for (int i=0; i<frame.rows; i++)
		{
			for (int j=0; j<frame.cols; j++)
			{
				tab[i*frame.cols+j]=(float)frame.at<uchar>(i,j);
				if (tab[i*frame.cols+j]>max) max=tab[i*frame.cols+j];
				//cout<<"!!!"<<colTab[i]<<"!!"<<endl;
				//if (i==478) cout<<"! "<<(float)frame.at<uchar>(i, j)<<"! ";
			}
			//if (tab[i]>colMax) colMax=colTab[i];
			//if (i==478) cout<<"@ "<<colTab[i]<<" @ ";
			//cout<<"! "<<(float)frame.at<uchar>(638, 478)<<"! ";
		}
		cout<<"max"<<max;
		for (int i=0; i<amount; i++)
		{
			Weights[i]*=2;
			Weights[i]+=(tab[i]/max);
			Weights[i]/=3;
			//cout<<" # "<<Weights[i];
		}
		#if 0
		float maxim=0;
		for (int i=0; i<amount; i++)
		{
			if (Weights[i]>maxim)
				maxim=Weights[i];
		}
		float minim=1;
		for (int i=0; i<amount; i++)
		{
			if (Weights[i]<minim)
				minim=Weights[i];
		}
		cout<<"maxim "<<maxim;
		cout<<" minim "<<minim;
		
		for (int i=0; i<frame.rows; i++)
		{
			colsWeights[i]+=(colTab[i]/colMax);
			colsWeights[i]/=2;
			//cout<<"@ "<<colsWeights[i]<<" @ ";
		}
		
		for (int i=0; i<frame.cols; i++)
		{
			for (int j=0; j<frame.rows; j++)
			{
				rowTab[i]+=(float)frame.at<uchar>(j,i);	
			}
			if (rowTab[i]>rowMax) rowMax=rowTab[i];
		}
		//cout<<"!!!!"<<endl;
		for (int i=0; i<frame.cols; i++)
		{
			rowsWeights[i]+=(rowTab[i]/rowMax);
			rowsWeights[i]/=2;
			//cout<<rowsWeights[i]<<" "<<i<<" ";
		}
		//cout<<"!!!!"<<endl;
		//weights=weights/2+frame/2;
		#endif
	}
	return multiply(frame, Weights);
}

Mat FramesDifference::WeightsMatrixThird(Mat frame)
{
	int amount=frame.rows*frame.cols;
	static int nr=1;
	if (ElapsedTime==0)
	{
		//weights=Mat(frame.rows, frame.cols, CV_32FC1);
		//colsWeights=frame.row(1);
		//rowsWeights=frame.col(1);
		//colsWeights=1;
		//rowsWeights=1;
		Weights=new float[amount];
		for (int i=0; i<amount; i++)
			Weights[i]=1;
		//cout<<"uwaga!"<<colsWeights.rows<<"\t";
		//cout<<colsWeights.cols<<endl;
		//cout<<"uwaga!"<<rowsWeights.rows<<"\t";
		//cout<<rowsWeights.cols<<endl;
		cout<<"hooop";
	}
	else
	{
		//int colMax=0, rowMax=0;
		int max=0;
		float tab[amount]={0};
		//float colTab[frame.rows]={0}, rowTab[frame.cols]={0};
		for (int i=0; i<frame.rows; i++)
		{
			for (int j=0; j<frame.cols; j++)
			{
				if (tab[i*frame.cols+j]<(float)frame.at<uchar>(i,j))
					tab[i*frame.cols+j]=(float)frame.at<uchar>(i,j);
				if (tab[i*frame.cols+j]>max) max=tab[i*frame.cols+j];
				//cout<<"!!!"<<colTab[i]<<"!!"<<endl;
				//if (i==478) cout<<"! "<<(float)frame.at<uchar>(i, j)<<"! ";
			}
			//if (tab[i]>colMax) colMax=colTab[i];
			//if (i==478) cout<<"@ "<<colTab[i]<<" @ ";
			//cout<<"! "<<(float)frame.at<uchar>(638, 478)<<"! ";
		}
		cout<<"max"<<max;
		cout<<"@ "<<nr<<endl;
		if(FramesDifference::ElapsedTime>=1*nr)
		{
			for (int i=0; i<amount; i++)
			{
				Weights[i]*=5;
				Weights[i]+=(tab[i]/max);
				Weights[i]/=6;
				//cout<<" # "<<Weights[i];
			}
			nr++;
			if (nr==20)
			{
				//ofstream plik("wagi.txt", ios::app);
				for (int i=0; i<amount; i++)
					//plik << Weights[i]<<"\t";// <<"\t"<< ((float)nonZero/numberOfPixels) << endl; 
				//plik.close();
				cout<<Weights[i]<<"\t";
			}
		}
		cout<<" # "<<Weights[220];
		#if 0
		float maxim=0;
		for (int i=0; i<amount; i++)
		{
			if (Weights[i]>maxim)
				maxim=Weights[i];
		}
		float minim=1;
		for (int i=0; i<amount; i++)
		{
			if (Weights[i]<minim)
				minim=Weights[i];
		}
		cout<<"maxim "<<maxim;
		cout<<" minim "<<minim;
		
		for (int i=0; i<frame.rows; i++)
		{
			colsWeights[i]+=(colTab[i]/colMax);
			colsWeights[i]/=2;
			//cout<<"@ "<<colsWeights[i]<<" @ ";
		}
		
		for (int i=0; i<frame.cols; i++)
		{
			for (int j=0; j<frame.rows; j++)
			{
				rowTab[i]+=(float)frame.at<uchar>(j,i);	
			}
			if (rowTab[i]>rowMax) rowMax=rowTab[i];
		}
		//cout<<"!!!!"<<endl;
		for (int i=0; i<frame.cols; i++)
		{
			rowsWeights[i]+=(rowTab[i]/rowMax);
			rowsWeights[i]/=2;
			//cout<<rowsWeights[i]<<" "<<i<<" ";
		}
		//cout<<"!!!!"<<endl;
		//weights=weights/2+frame/2;
		#endif
	}
	return multiply(frame, Weights);
}
