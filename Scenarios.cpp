#include "FramesDifference.h"

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
	//difference_help=WeightsMatrixSecond(difference_helper);	
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
	//difference_help=WeightsMatrixThird(difference_helper);	
	//thresholding(difference_helper, difference_helper, (double)1, 255, THRESH_BINARY);
	//erode(difference_helper, difference_help, Mat(), Point(-1,-1), 3, BORDER_CONSTANT, morphologyDefaultBorderValue());
	boxFilter(difference_helper, difference_help, -1, Size(13, 13), Point(-1,-1), true, BORDER_DEFAULT );
	thresholding(difference_help, difference_help, (double)3.5, 255, THRESH_BINARY);
	return difference_help;
}

Mat FramesDifference::WeightsScenarioFourth(Mat old, Mat young)
{
	Mat difference_helper, difference_help, difference_help2;
	absdiff(old, young, difference_helper);
	//difference_helper=old;
	difference_help=WeightsMatrixFourth(difference_helper);	
	//thresholding(difference_helper, difference_helper, (double)1, 255, THRESH_BINARY);
	//erode(difference_helper, difference_help, Mat(), Point(-1,-1), 3, BORDER_CONSTANT, morphologyDefaultBorderValue());
	boxFilter(difference_help, difference_help2, -1, Size(13, 13), Point(-1,-1), true, BORDER_DEFAULT );
	//difference_helper=WeightsMatrixFourth(difference_help);	
	thresholding(difference_help2, difference_help2, (double)4, 255, THRESH_BINARY);
	return difference_help2;
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
				{
					tab[i*frame.cols+j]=(float)frame.at<uchar>(i,j);
					if (tab[i*frame.cols+j]>max) max=tab[i*frame.cols+j];
				}
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
			if (nr==31)
			{
				ofstream plik("wagi.txt", ios::app);
				for (int j=0; j<frame.rows; j++)
				{
					for (int i=0; i<frame.cols; i++)
						plik << Weights[j*frame.cols+i]<<"\t";// <<"\t"<< ((float)nonZero/numberOfPixels) << endl; 
					plik<<"\n";
				}
				plik.close();
				//cout<<Weights[i]<<"\t";
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

//w każdym z kwadratów o wymiarach 5x5cm sumujemy piksele różnicy
//im większa suma, tym większa waga obszaru
Mat FramesDifference::WeightsMatrixFourth(Mat frame)
{
	const short xDist=24, yDist=12;
	float piksOnXDist=(float)frame.cols/xDist;
	float piksOnYDist=(float)frame.rows/yDist;
	const int amount=xDist*yDist;//frame.rows*frame.cols;
	cout<<"amount: "<<amount<<endl;
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
			Weights[i]=1.0;
		//cout<<"wagi!!"<<sizeof(Weights);
		//cout<<"uwaga!"<<Weights.rows<<"\t";
		//cout<<Weights.cols<<endl;
		//cout<<"uwaga!"<<rowsWeights.rows<<"\t";
		//cout<<rowsWeights.cols<<endl;
		cout<<"hooop";
	}
	else
	{
		//int colMax=0, rowMax=0;
		static float max=0;
		float tab[amount]={0};
		static float tab2[amount];
		//float colTab[frame.rows]={0}, rowTab[frame.cols]={0};
		for (int i=0; i<yDist; i++)
		{
			for (int j=0; j<xDist; j++)
			{
				for (int k=0; k<piksOnYDist; k++)
				{
					for (int l=0; l<piksOnXDist; l++)
					{
						tab[i*xDist+j]+=(float)frame.at<uchar>(k+i*piksOnYDist,l+j*piksOnXDist);
					}
				}
			}
		}
		for (int i=0; i<yDist; i++)
		{
			for (int j=0; j<xDist; j++)
			{
				if (tab[i*xDist+j]>tab2[i*xDist+j])
				{
					tab2[i*xDist+j]=tab[i*xDist+j];
					if (tab2[i*xDist+j]>=max) max=tab2[i*xDist+j];
				}
			}
		}
		if(FramesDifference::ElapsedTime>=2*nr)
		{
			cout<<"!!"<<sizeof(tab2)<<"!!!"<<sizeof(Weights)<<"!!!!"<<amount;
			for (int i=0; i<(amount); i++)
			{
				//cout<<i<<"\t";
				Weights[i]*=5;
				Weights[i]+=(tab2[i]/max);
				Weights[i]/=6;
				//Weights[i]*=2;
				//cout<<" # "<<Weights[i];
				cout<<Weights[i]<<"\t";//<<tab2[i]/max<<"\t";
				//cout<<"wwww: "<<Weights[24*12]<<endl;
				tab2[i]=0;
			}
			cout<<"wwww: "<<Weights[24*12]<<endl;
			/*Weights[17]*=2;
			Weights[19]*=2;
			Weights[127]*=2;
			Weights[128]*=2;
			Weights[154]*=2;
			Weights[163]*=2;
			Weights[190]*=2;
			Weights[283]*=2;
			Weights[284]*=2;
			*/nr++;
			max=0;
			//#if 0
			if (nr==31)
			{
				ofstream plik("wagi4.txt", ios::app);
				for (int j=0; j<yDist; j++)
				{
					for (int i=0; i<xDist; i++)
						//cout<<Weights[j*xDist+i]<<"\t";
						plik << Weights[j*xDist+i]<<"\t";// <<"\t"<< ((float)nonZero/numberOfPixels) << endl; 
					//cout<<"\n";
					plik<<"\n";
				}
				plik.close();
				//cout<<Weights[i]<<"\t";
			}
			//#endif
		}
		#if 0
		for (int i=0; i<frame.rows; i++)
		{
			for (int j=0; j<frame.cols; j++)
			{
				if (tab[i*frame.cols+j]<(float)frame.at<uchar>(i,j))
				{
					tab[i*frame.cols+j]=(float)frame.at<uchar>(i,j);
					if (tab[i*frame.cols+j]>max) max=tab[i*frame.cols+j];
				}
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
				//Weights[i]*=5;
				Weights[i]+=(tab[i]/max);
				Weights[i]/=2;
				//cout<<" # "<<Weights[i];
			}
			nr++;
			if (nr==31)
			{
				ofstream plik("wagi.txt", ios::app);
				for (int j=0; j<frame.rows; j++)
				{
					for (int i=0; i<frame.cols; i++)
						plik << Weights[j*frame.cols+i]<<"\t";// <<"\t"<< ((float)nonZero/numberOfPixels) << endl; 
					plik<<"\n";
				}
				plik.close();
				//cout<<Weights[i]<<"\t";
			}
		}
		cout<<" # "<<Weights[220];
		#endif
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
	cout<<"\tmnozenie\t";
	return multiply(frame, Weights, yDist, xDist, piksOnYDist, piksOnXDist);
}
