#include <iostream>
#include <fstream>
#include "graphics.h"
#pragma comment(lib, "graphics.lib")

using namespace std;

//This is simple~ all this class ever do is read/write a file its highscore, the rest of it being the good-old gets-sets.

class Score{
private:
	int score, hiScore;
	fstream f;

public:
	Score(){
		f.open("high.txt");
		f >> hiScore;
		score = 0;
		f.close();
	}

	int GetScore(){
		return score;
	}

	void SetScore(int score){
		this->score = score;
	}

	int GetHiScore(){
		return hiScore;
	}

	void SetHiScore(int score){
		this->hiScore = score;
		f.open("high.txt");
		f << hiScore;
		f.close();
	}

	void DrawScore(int x, int y){
		char str1[99], str2[99];
		sprintf(str1,"Score: %07u",score);
		sprintf(str2,"Hi-score: %07u",hiScore);
		
		setcolor(WHITE);
		outtextxy(x,y,str1);
		outtextxy(x,y+30,str2);
	}
};