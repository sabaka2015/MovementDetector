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
		cout<<"Podaj nazwę pliku z filmem do odtworzenia: ";
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
	
	float firstTimeOfLive; //time of live of the firstFrame
	cout<<"1- odejmowanie nastepujacych po sobie klatek"
			"\n0- odejmowanie od poczatkowej klatki: ";
	
	short substractionType;
	cin>>substractionType;
	if (substractionType==0)
	{
		cout<<"Co ile sekund zmieniać początkową klatkę?: ";
		cin>>firstTimeOfLive;
	}
	cout<<"Czy uwzględnić współczynniki wagowe (uwzglądnienie historii)"
			" 1- tak, 0- nie : ";
	bool ifWeights;
	cin>>ifWeights;
	short typeOfSum=1;
	if (ifWeights)
	{
		cout<<"1- współczynniki wagowe modyfikowane na bierząco\n"
				"0- co pewien czas : ";
		cin>>typeOfSum;
	}
	
	if (ifWeights)
		cout<<"\nWybierz scenariusz:\nGauss: 1 \nBox: 3: ";
	else
		cout<<"\nWybierz scenariusz:\nGauss: 1 \nMedian: 2\nBox: 3: \nDilat: 4" 
				"\nSobel: 5 \nThresBoxThres: 6: \nCanny: 7 \nProgowanie: 8 ";
	int scenario;
	cin>>scenario;
	
	if (ifWeights)
	{
		if ((typeOfSum==0)&&(substractionType!=0))
		{
			cout<<"Co ile sekund modyfikować współczynniki wagowe?: ";
			cin>>firstTimeOfLive;
			cout<<"0- współczynniki wagowe dla obszarów o rozmiarach ok 5x5cm"
				"\n6- współczynniki wagowe dla każdego piksela : ";
			short delayedWeightsType;
			cin>>delayedWeightsType;
			scenario+=delayedWeightsType;
		}
		else
		{
			cout<<"0- współczynniki wagowe dla kolumn i wierszy"
				"\n6- dla każdego piksela : ";
			short liveWeightsType;
			cin>>liveWeightsType;
			scenario+=liveWeightsType;
		}
	}
	short ifFirst=substractionType*typeOfSum;
	
	
	
	cout<<"Czy utworzyć film z wynikiem działania programu? : ";
	bool record;
	cin>>record;
	string recordName="usunMnie.avi";
	if (record)
	{
		cout<<"Podaj nazwę pliku docelowego dla utworzonego filmu (bez rozszerzenia): ";
		cin>>recordName;
		recordName+=".avi";
	}
	cout<<"Podaj nazwę pliku do zapisu danych tekstowych (bez rozszerzenia): ";
	string txtExport;
	cin>>txtExport;
	txtExport+=".txt";
	
	//read the first frame
	Mat olderFrame, youngerFrame, olderFrameConv, youngerFrameConv;
	Mat firstFrameConv;
	arm>>olderFrame;
	
	//conversion from RGB to gray color space
	cvtColor(olderFrame, olderFrameConv, CV_RGB2GRAY); 
	
	olderFrameConv.copyTo(firstFrameConv);
	cout << "klatki" << fps << endl;
	//distance between frames in ms
	double framesDistance = 1000 / fps;
	//creating a window
	namedWindow("window", WINDOW_AUTOSIZE);
	//char zapis='1';
	//char zapis2='0';
	
	VideoWriter writer (recordName, CV_FOURCC('M','J','P','G'),
									fps, Size(640,480), false);
	
	//a loop in which the video is analysing and printing in window
	while (true)
	{
		if(!arm.grab()) break;
		arm>>youngerFrame;
		
		//conversion from RGB to Gray scale
		cvtColor(youngerFrame, youngerFrameConv, CV_RGB2GRAY);

		//object of the new class- difference between following frames
		FramesDifference difference=FramesDifference
			(olderFrameConv, youngerFrameConv, firstFrameConv, 
				scenario, txtExport, substractionType, ifWeights, 
					typeOfSum, firstTimeOfLive);
		
		youngerFrameConv.copyTo(olderFrameConv);
		
		imshow("window", difference.getDifference());
		if (record)
			writer<<difference.getDifference();
		
		//zapis ten fragment zostanie wyrzucony w ostatecznej wersji programu
		//#if 0
		if ((FramesDifference::ElapsedTime>=29.8)&&(FramesDifference::ElapsedTime<=29.9))
		{
			//string plik="roznice/frodo2/thres8/frodo2_";
			//plik+=zapis2;//static_cast<char>(zapisanych);
			//plik+=zapis;
			//plik+=".jpg";
			//cout<<plik<<endl;
			string plik="boxWeights.jpg";
			imwrite(plik, difference.getDifference()/*youngerFrame*/);
			//if (zapis<'9') zapis++;
			//else
			//{
			//	zapis2++;
			//	zapis='0';
			//}
		}
		//#endif
		//alarm raczej nie jest potrzebny, można wyrzucić w ostatecznej wersji programu
		//alarm if time without movement is longer than 20s
		if (FramesDifference::counterZero*framesDistance>=20000)
		{
			FramesDifference::alarm();
		}
		
		//elapsed time
		FramesDifference::ElapsedTime+=(framesDistance/1000); 
		if (ifFirst==0)
		{
			static int nr=1;
			if(FramesDifference::ElapsedTime>=firstTimeOfLive*nr)
				{
					youngerFrameConv.copyTo(firstFrameConv);
					nr++;
				}
		}
		cout<<"elapsed time: "<<FramesDifference::ElapsedTime<<endl;
		//breaking the loop with any key
		int stop = waitKey(framesDistance);
		if (stop+1) break; 
	}
	
	//ten fragment tworzy wykres zaraz po zamknięciu filmu
	//nie będzie go w ostatecznej wersji programu
	//making gnuplot script
	#if 0
	ofstream skrypt("skrypt.txt");
	skrypt << "set yrange [0:0.005]\n";
	//skrypt << "set xrange [0:"<<(FramesDifference::ElapsedTime+0.1)<<"]\n";
	skrypt << "plot \"" << txtExport <<"\" with linespoints pt 2 ps 2 lt 2 lw 1";
	skrypt.close();
	system("gnuplot -p -e \"load 'skrypt.txt'\"");
	#endif
	return 0;
	 
}
