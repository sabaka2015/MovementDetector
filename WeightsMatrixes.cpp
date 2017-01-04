#include "FramesDifference.h"

Mat FramesDifference::HistoryMatrix(Mat frame)
{
	Mat weights;
	if (ElapsedTime==0)
	{
		weights=frame;
	}
	else
	{
		weights=weights/2+frame/2;
	}
	
	return weights;
}

//in this algorithm we have two weights matrixes- one for columns and one for rows
//each pixel is multiplying by two appropriate arrays
Mat FramesDifference::WeightsMatrix(Mat frame, Mat old, Mat young)
{
	Mat difference;
	absdiff(old, young, difference);
	//initialization of the matrixes
	if (ElapsedTime==0)
	{
		colsWeights=new float[difference.rows];
		rowsWeights=new float[difference.cols];
		for (int i=0; i<difference.rows; i++)
			colsWeights[i]=1;
		for (int i=0; i<difference.cols; i++)
			rowsWeights[i]=1;	
	}
	//updating the matrixes
	else
	{
		int colMax=0, rowMax=0;
		float colTab[difference.rows]={0}, rowTab[difference.cols]={0};
		for (int i=0; i<difference.rows; i++)
		{
			for (int j=0; j<difference.cols; j++)
			{
				colTab[i]+=(float)difference.at<uchar>(i,j);
			}
			if (colTab[i]>colMax) colMax=colTab[i];
		}
		for (int i=0; i<difference.rows; i++)
		{
			colsWeights[i]+=(colTab[i]/colMax);
			colsWeights[i]/=2;
		}
		
		for (int i=0; i<difference.cols; i++)
		{
			for (int j=0; j<difference.rows; j++)
			{
				rowTab[i]+=(float)difference.at<uchar>(j,i);	
			}
			if (rowTab[i]>rowMax) rowMax=rowTab[i];
		}
		for (int i=0; i<difference.cols; i++)
		{
			rowsWeights[i]+=(rowTab[i]/rowMax);
			rowsWeights[i]/=2;
		}
	}
	return multiply(frame, colsWeights, rowsWeights);
}

//in this algorithm we have one particular weight for each pixel
Mat FramesDifference::WeightsMatrixSecond(Mat frame, Mat old, Mat young)
{
	Mat difference;
	absdiff(old, young, difference);
	int amount=difference.rows*frame.cols;
	//initialization of the matrix
	if (ElapsedTime==0)
	{
		Weights=new float[amount];
		for (int i=0; i<amount; i++)
			Weights[i]=1;
	}
	//updating the matrix
	else
	{
		int max=0;
		float tab[amount]={0};
		for (int i=0; i<difference.rows; i++)
		{
			for (int j=0; j<difference.cols; j++)
			{
				tab[i*difference.cols+j]=(float)difference.at<uchar>(i,j);
				if (tab[i*difference.cols+j]>max) max=tab[i*difference.cols+j];
			}
		}
		for (int i=0; i<amount; i++)
		{
			Weights[i]*=2;
			Weights[i]+=(tab[i]/max);
			Weights[i]/=3;
		}
	}
	return multiply(frame, Weights);
}

//in this algorithm we have one particular weight for each pixel
//The Weights matrix is updating after every firstTimeOfLive
Mat FramesDifference::WeightsMatrixThird(Mat frame, Mat first, Mat young, float firstTimeOfLive)
{
	Mat difference;
	absdiff(first, young, difference);
	int amount=difference.rows*difference.cols;
	static int nr=1;
	//initialization of the matrix
	if (ElapsedTime==0)
	{
		Weights=new float[amount];
		for (int i=0; i<amount; i++)
			Weights[i]=1;
	}
	//updating the matrix
	else
	{
		int max=0;
		float tab[amount]={0};
		for (int i=0; i<difference.rows; i++)
		{
			for (int j=0; j<difference.cols; j++)
			{
				if (tab[i*difference.cols+j]<(float)difference.at<uchar>(i,j))
				{
					tab[i*difference.cols+j]=(float)difference.at<uchar>(i,j);
					if (tab[i*difference.cols+j]>max) max=tab[i*difference.cols+j];
				}
			}
		}
		if(FramesDifference::ElapsedTime>=firstTimeOfLive*nr)
		{
			for (int i=0; i<amount; i++)
			{
				Weights[i]*=5;
				Weights[i]+=(tab[i]/max);
				Weights[i]/=6;
			}
			nr++;
			//do wyrzucenia z ostatecznej wersji programu
			#if 0
			if (nr==31)
			{
				ofstream plik("wagi.txt", ios::app);
				for (int j=0; j<difference.rows; j++)
				{
					for (int i=0; i<difference.cols; i++)
						plik << Weights[j*difference.cols+i]<<"\t";// <<"\t"<< ((float)nonZero/numberOfPixels) << endl; 
					plik<<"\n";
				}
				plik.close();
				//cout<<Weights[i]<<"\t";
			}
			#endif
		}
	}
	return multiply(frame, Weights);
}

//we are summing values of pixels in 5x5cm areas
//the bigger sum, the bigger area value
//The Weights matrix is updating after every firstTimeOfLive
Mat FramesDifference::WeightsMatrixFourth(Mat frame, Mat first, Mat young, short& yDistHelp, short& xDistHelp, float& piksOnYDist, float& piksOnXDist, float firstTimeOfLive)//(Mat first, Mat young)
{
	Mat difference;
	absdiff(first, young, difference);
	const short xDist=24, yDist=12;
	xDistHelp=xDist;
	yDistHelp=yDist;
	piksOnXDist=(float)difference.cols/xDist;
	piksOnYDist=(float)difference.rows/yDist;
	const int amount=xDist*yDist;
	static int nr=1;
	//initialization of the matrix
	if (ElapsedTime==0)
	{
		Weights=new float[amount];
		for (int i=0; i<amount; i++)
			Weights[i]=1.0;
	}
	//updating the matrix
	else
	{
		static float max=0;
		float tab[amount]={0};
		static float tab2[amount];
		for (int i=0; i<yDist; i++)
		{
			for (int j=0; j<xDist; j++)
			{
				for (int k=0; k<piksOnYDist; k++)
				{
					for (int l=0; l<piksOnXDist; l++)
					{
						tab[i*xDist+j]+=(float)difference.at<uchar>(k+i*piksOnYDist,l+j*piksOnXDist);
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
		if(FramesDifference::ElapsedTime>=firstTimeOfLive*nr)
		{
			for (int i=0; i<(amount); i++)
			{
				Weights[i]*=3;
				Weights[i]+=(tab2[i]/max);
				Weights[i]/=4;
				tab2[i]=0;
			}
			nr++;
			max=0;
			//do wyrzucenia z ostatecznej wersji programu
			#if 0
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
			#endif
		}
	}
	return multiply(frame, Weights, yDist, xDist, piksOnYDist, piksOnXDist);
}

