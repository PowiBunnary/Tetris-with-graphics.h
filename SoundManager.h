#include<windows.h>    //used for PlaySound function
#include<fstream>    //used for file handling

#pragma comment(lib, "winmm.lib")

//This class was supposed to use the PlaySound() function at the first place, but apparently any sound started playing while there is another one being played,
//the playing sound will stop and play the called one.
//So i use this mciSendString thingy i found on the internet and holysh*t it works!
//Note: MCI stands for Media Control Interface. Read https://en.wikipedia.org/wiki/Media_Control_Interface
//Credits to zapslpat for the SFXs (https://www.zapsplat.com/)

class Sound{
public:
	Sound(){
		mciSendString(L"open \"Mono\\hit1.wav\" type waveaudio alias hit1", NULL, 0, NULL);
		mciSendString(L"open \"Mono\\hit2.wav\" type waveaudio alias hit2", NULL, 0, NULL);
		mciSendString(L"open \"Mono\\hit3.wav\" type waveaudio alias hit3", NULL, 0, NULL);
		mciSendString(L"open \"Mono\\clearline1.wav\" type waveaudio alias clear1", NULL, 0, NULL);
		mciSendString(L"open \"Mono\\clearline2.wav\" type waveaudio alias clear2", NULL, 0, NULL);
		//mciSendString(L"open \"Mono\\audio.mp3\" type mpegvideo alias theme1", NULL, 0, NULL);
		//mciSendString(L"play theme1 from 0", NULL, 0, NULL);

		//hitSound1 = L"Mono\\LR_Chm Soft Normal.wav";
		//hitSound2 = L"Mono\\LR_Chm Fast.wav";
	}

	void StopMusic(){
		mciSendString(L"stop theme1", NULL, 0, NULL);
	}

	void PlaySFXHitSound(){
		int temp = rand()%3+1;

		switch (temp){
		case 1:
			mciSendString(L"play hit1 from 0", NULL, 0, NULL);
			break;
		case 2:
			mciSendString(L"play hit2 from 0", NULL, 0, NULL);
			break;
			case 3:
			mciSendString(L"play hit3 from 0", NULL, 0, NULL);
			break;
		}
	}

	void PLaySFXClearSound(){
		int temp = rand()%2+1;

		switch (temp){
		case 1:
			mciSendString(L"play clear1 from 0", NULL, 0, NULL);
			break;
		case 2:
			mciSendString(L"play clear2 from 0", NULL, 0, NULL);
			break;
		}
	}
};