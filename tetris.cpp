#include "graphics.h"
#include <iostream>
#include "Engine.h"
#include <ctime>
#include <thread>
#pragma comment(lib, "graphics.lib")
using namespace std;

void drawCirl(int x, int y,int time){
	delay (time);
	setcolor(WHITE);
	circle(x,y,50);
	delay(1000);
	setcolor(BLACK);
	circle(x,y,50);
}

int main(){
	char c = NULL;

	//initgraph(&gd, &gm, "");

	initwindow (720,960);

	/*Board board;
	board.DrawBoard();
	JBlock t;
	t.DrawBlock(WHITE);*/

Restart:
	setfillstyle(1,0);
	floodfill(1,1,4);
	outtextxy(boardPosX+boardX*size+30,boardPosY+boardY*(size-5)-180,"A/D: Move left/right");
	outtextxy(boardPosX+boardX*size+30,boardPosY+boardY*(size-5)-150,"W: Spin block");
	outtextxy(boardPosX+boardX*size+30,boardPosY+boardY*(size-5)-120,"S: Move down");
	outtextxy(boardPosX+boardX*size+30,boardPosY+boardY*(size-5)-90,"Spacebar: Instant move down");
	outtextxy(boardPosX+boardX*size+30,boardPosY+boardY*(size-5)-60,"R: Hold block");
	outtextxy(boardPosX+boardX*size+30,boardPosY+boardY*(size-5)-30,"P: Pause the game");
	outtextxy(boardPosX+boardX*size+30,boardPosY+boardY*(size-5),"~: Stop game");

	int k = 2;
	Engine en;
	en.GameStart();

	cout<<"L: (Debug) Print board's array"<<endl;
	cout<<"O: (Debug) Show key inputs"<<endl;
	cout<<"+: (Debug) Increase speed"<<endl;
	cout<<"-: (Debug) Decrease speed"<<endl;

	do {
		en.InitTimer();
		if (kbhit())
		{
			c = toupper(getch());
			k = en.Input(c);
		}
	} while (k == 2);

	if (k == 0)
		goto Restart;
	return 0;
}