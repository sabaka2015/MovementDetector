/* Author: Mateusz Osiński
 * The program is detecting movements in video
 */
#include <time.h> 
#include <cstdlib>
 
#include "FramesDifference.h"

//this function gives an arm to the video. 
//the function returns fps 
//if we choose a live transmition, fps is counted- the get function do not work
float captureArm (int choice, string nameOfFile, VideoCapture *arm)
{
	float fps;
	if (choice==1)
	{
		*arm=VideoCapture(nameOfFile);
		fps=arm->get(CV_CAP_PROP_FPS);
	}
	else
	{
		*arm=VideoCapture(0); //0- default camera
		//start time
		time_t start, end;
		Mat lostFrames;
		time(&start);
		//grab 30 frames
		for(int i = 0; i < 30; i++)
		{
			(*arm) >> lostFrames;
		}
		//end Time
		time(&end);
		//total time
		double seconds = difftime (end, start);
		// Calculate frames per second
		cout<<seconds<<endl;
		fps  = 30 / seconds;
	}
	return fps; 
}
 

int main ()
{
	//capture the video
	cout<<"Nagrany film: 1, na żywo: 0 : ";
	short choice;
	cin>>choice;
	
	string nameOfFile;
	if (choice==1)
	{
		cout<<"Podaj nazwę pliku: ";
		cin>>nameOfFile;
	}
	
	VideoCapture arm;
	float fps; //frames per second
	
	fps=captureArm(choice, nameOfFile/*"dlon.avi"*/, &arm);

	if (!arm.isOpened())
	{
		cout<<"brak pliku lub niewłaściwe nawiązanie połączenia z kamerą\n";
		return -1;
	}
	
	
	cout<<"1- odejmowanie nastepujacych po sobie klatek"
			"\n0- odejmowanie od poczatkowej klatki: ";
	
	short substractionType;
	cin>>substractionType;
	cout<<"Czy uwzględnić współczynniki wagowe (uwzglądnienie historii)"
			" 1- tak, 0- nie : ";
	bool ifWeights;
	cin>>ifWeights;
	short typeOfSum=1;
	if (ifWeights)
	{
		cout<<"współczynniki wagowe modyfikowane na bierząco- 1\n"
				"co pewien czas- 0 : ";
		cin>>typeOfSum;
	}
	short ifFirst=substractionType*typeOfSum;
	
	cout<<"\nscenariusz:\nGauss: 1 \nMedian: 2\nBox: 3: \nDilat: 4" 
			"\nSobel: 5 \nThresBoxThres: 6 ";
	int scenario;
	cin>>scenario;
	#if 0
	cout<<"\nscenariusz:\nGauss: 1 \nMedian: 2\nBox: 3: \nDilat: 4" 
			"\nSobel: 5 \nThresBoxThres: 6 \nHistory: 7 \nWagi: 8 "
			"\nWagi Drugie: 9 \nWagi Trzecie: 10 ";
	int scenario;
	cin>>scenario;
	#endif
	cout<<"Czy zapisać film? : ";
	bool record;
	cin>>record;
	string recordName="usunMnie.avi";
	if (record)
	{
		cout<<"Podaj nazwę pliku docelowego (bez rozszerzenia): ";
		cin>>recordName;
		recordName+=".avi";
	}
	cout<<"Podaj nazwę pliku do zapisu danych (bez rozszerzenia): ";
	string txtExport;
	cin>>txtExport;
	txtExport+=".txt";
	
	//read the first frame
	Mat olderFrame, youngerFrame, olderFrameConv, youngerFrameConv;
	Mat firstFrameConv;
	arm>>olderFrame;
	//olderFrame=imread("roznice/frodo2/nothres/dilat1/d00.bmp");
	cvtColor(olderFrame, olderFrameConv, CV_RGB2GRAY); //conversion from RGB to other color space- to save the memory
	//to Luv-4B, to gray-4B, to XYZ-4B, to XSV-4B
	olderFrameConv.copyTo(firstFrameConv);
	cout << "klatki" << fps << endl;
	//distance between frames in ms
	double framesDistance = 1000 / fps;
	//making a window
	namedWindow("window", WINDOW_AUTOSIZE);
	char zapis='1';
	char zapis2='0';
	
	//zapis danych do pliku
	//fstream file;
	//file.open("oldvers.txt");
	//int licznik=0;
	
	VideoWriter writer (/*"nagr_filmy/basic+gauss21x21_thres6_frodo2.avi"*/recordName, CV_FOURCC('M','J','P','G'), fps, Size(640,480), false);
	
	while (true)
	{
		//if(licznik>30) break;
		//licznik++;
		if(!arm.grab()) break;
		arm>>youngerFrame;
		//odczyt
		#if 0
		if (zapis2<='3')
		{
			string plik="roznice/frodo2/nothres/dilat1/d";
			plik+=zapis2;//static_cast<char>(zapisanych);
			plik+=zapis;
			plik+=".bmp";
			cout<<plik<<endl;
			youngerFrame=imread(plik);
			if (zapis<'9') zapis++;
			else
			{
				zapis2++;
				zapis='0';
			}
		}
		#endif
		//conversion from RGB to Gray scale
		cvtColor(youngerFrame, youngerFrameConv, CV_RGB2GRAY);
		//cvtColor(olderFrame, olderFrameConv, CV_RGB2GRAY);
		//object of the new class- difference between following frames
		FramesDifference difference=FramesDifference
			(olderFrameConv, youngerFrameConv, firstFrameConv, 
				scenario, txtExport, substractionType, ifWeights, typeOfSum);
		
			//youngerFrame/*Conv*/.copyTo(olderFrame/*Conv*/);
		youngerFrameConv.copyTo(olderFrameConv);
		//cvtColor(olderFrame, olderFrameConv, CV_RGB2GRAY);
		imshow("window", difference.getDifference());
		if (record)
			writer<<difference.getDifference();
		//zapis
		#if 0
		if (zapis2<='9')
		{
			string plik="roznice/frodo2/thres8/frodo2_";
			plik+=zapis2;//static_cast<char>(zapisanych);
			plik+=zapis;
			plik+=".jpg";
			cout<<plik<<endl;
			imwrite(plik, difference.getDifference()/*youngerFrame*/);
			if (zapis<'9') zapis++;
			else
			{
				zapis2++;
				zapis='0';
			}
		}
		#endif
		//alarm if time without movement is longer than 20s
		cout<<"przed counter ";
		//cout<<endl<<(FramesDifference::counterZero*framesDistance)<<endl;
		cout<<"przed alarm ";
		if (FramesDifference::counterZero*framesDistance>=20000)
		{
			FramesDifference::alarm();
		}
		FramesDifference::ElapsedTime+=(framesDistance/1000); //elapsed time
		if (ifFirst==0)
		{
			static int nr=1;
			if(FramesDifference::ElapsedTime>=2*nr)
				{
					youngerFrameConv.copyTo(firstFrameConv);
					nr++;
				}
		}
		cout<<"\t"<<FramesDifference::ElapsedTime<<"\t";
		int stop = waitKey(framesDistance);
		if (stop+1) break; //breaking with any key
	}
	//file.close();
	//cvReleaseVideoWriter(&writer);
	//system("gnuplot -p -e \"plot 'basicThres15.txt'\"");
	//making gnuplot script
	ofstream skrypt("skrypt.txt");
	skrypt << "set yrange [0:0.005]\n";
	//skrypt << "set xrange [0:"<<(FramesDifference::ElapsedTime+0.1)<<"]\n";
	skrypt << "plot \"" << txtExport <<"\" with linespoints pt 2 ps 2 lt 2 lw 1";
	skrypt.close();
	system("gnuplot -p -e \"load 'skrypt.txt'\"");
	return 0;
	 
}


//uwagi:
//zmienić colsWeights i rowsWeights na zwykłe tablice floatów
