#include <stdlib.h>
#include <stdio.h>
#include "graphics.h"
#include <iostream>
#include <time.h>

const int MaxI = 35;
const int  MaxJ = 13;
//ma tran chua
int Board[MaxI][MaxJ];
int fullrowaddress[35];
#pragma comment(lib, "graphics.lib")
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
using namespace std;
struct Point
{
	int x, y;
}
a[8], b[8];
// mang chua gach.
int blocks[] = {
	// T block
	1, 0, 1, 1, 1, 2, 2, 1,
	0, 1, 1, 0, 1, 1, 2, 1,
	0, 1, 1, 0, 1, 1, 1, 2,
	0, 1, 1, 1, 1, 2, 2, 1,
	//L block
	0, 2, 1, 0, 1, 1, 1, 2,
	0, 1, 1, 1, 2, 1, 2, 2,
	1, 0, 1, 1, 1, 2, 2, 0,
	0, 0, 0, 1, 1, 1, 2, 1,
	//Square block
	0, 0, 0, 1, 1, 0, 1, 1,
	//Z block
	1, 1, 1, 2, 2, 0, 2, 1,
	0, 1, 1, 1, 1, 2, 2, 2,
	//I block
	0, 1, 1, 1, 2, 1, 3, 1,
	1, 0, 1, 1, 1, 2, 1, 3,
};
int * blockarray;
int fallingBlockNum;
int fallingBlockVersion = 0; //values 0 to 4
int fallingBlockRow = 0; //the current row of falling block
int fallingBlockCol = 0; //the current column of falling block
int startdelay = 200;
int motiondelay;
int scoreInc = 5; //score for each line clearance
int myscore = 0; //score
void nextBlock() {
	srand(time(NULL));
	fallingBlockNum = rand() % 5;
	if (fallingBlockNum == 0 || fallingBlockNum == 1)
		fallingBlockVersion = rand() % 4;
	if (fallingBlockNum == 2 || fallingBlockNum == 3)
		fallingBlockVersion = rand() % 2;
	else
		fallingBlockVersion = 0;
	fallingBlockRow = 0;
	fallingBlockCol = 5;
}

//Lay block gach tuong ung voi so' random
int * getFallingBlockArray() {
	int a = fallingBlockNum * 10 + fallingBlockVersion;
	switch (a) {
	case 0:
		return ((int *)& blocks[0]);
	case 1:
		return ((int *)& blocks[8]);
	case 2:
		return ((int *)& blocks[16]);
	case 3:
		return ((int *)& blocks[24]);
	case 10:
		return ((int *)& blocks[32]);
	case 11:
		return ((int *)& blocks[40]);
	case 12:
		return ((int *)& blocks[48]);
	case 13:
		return ((int *)& blocks[56]);
	case 20:
		return ((int *)& blocks[64]);
	case 30:
		return ((int *)& blocks[72]);
	case 31:
		return ((int *)& blocks[80]);
	case 40:
		return ((int *)& blocks[88]);
	case 41:
		return ((int *)& blocks[96]);
	}
	return ((int *)& blocks[64]);
}
//Ham ve gach 
void drawNewBlockVersion() {
	int i, r, c;
	blockarray = getFallingBlockArray();
	for (i = 0; i < 8; i += 2) {
		r = fallingBlockRow + blockarray[i];
		c = fallingBlockCol + blockarray[i + 1];
		Board[r][c] = 1;
		setcolor(WHITE);
		rectangle(2 + c * 13, 2 + r * 13, 2 + c * 13 + 13, 2 + r * 13 + 13); // ve nhung van chua hieu
	}
}
void clearOldBlockVersion() {
	int i, r, c;
	for (i = 0; i < 8; i += 2) {
		r = fallingBlockRow + blockarray[i];
		c = fallingBlockCol + blockarray[i + 1];
		Board[r][c] = 0;
		setcolor(BLACK);
		rectangle(2 + c * 13, 2 + r * 13, 2 + c * 13 + 13, 2 + r * 13 + 13); // van chua hieu khuc nay
	}
}
int isAtBottom() {
	int i, max = 0, ti, tj;
	for (i = 0; i < 8; i += 2)
		if (blockarray[i] > max)				//check da cham. day' chua
		{
			max = blockarray[i];				// blockarray la kieu *int  
		}
	if (fallingBlockRow + max >= 34)
		return 1;
	for (i = 0; i < 8; i += 2) {					//check xem da co gach chua, 				
		ti = blockarray[i] + fallingBlockRow;
		tj = blockarray[i + 1] + fallingBlockCol;
		if (Board[ti + 1][tj] == 2)
			return 1;
	}
	return 0;
}

void drawagain() {
	for (int i = MaxI - 1; i > 0; i--)
	{
		for (int j = 0; j < MaxJ; j++)
		{
			if (Board[i][j] == 2)
			{
				setcolor(WHITE);
				rectangle(2 + j * 13, 2 + i * 13, 2 + j * 13 + 13, 2 + i * 13 + 13);
			}
		}
	}
}
void checklines2() {
	int i = MaxI - 1;
	int  fullrow = 0, a = 0, flag = 0, score =0;
	for (i; i > 0; i--) {
		int count = 0;
		for (int j = 0; j < MaxJ; j++)  //check hang co full ko, moi cot co value bang 2
			count += Board[i][j];  
		if (count == 26)
		{
			fullrow++; //tong so hang full
			fullrowaddress[a] = i;//luu lai, hang nao full
			a++;
			score++;
		}
		else if (count == 0)
		{
			flag = i;// row stop cuoi cung`
			break;
		}
	}
	int k = fullrow - 1;
	while (fullrowaddress[k] != 0 && k >=0 ) {
		int tmp = fullrowaddress[k];// 
		for (tmp; tmp >= flag; tmp--)// bat dau nguoc. 
		{
			for (int j = 0; j < MaxJ; j++)
			{
				setcolor(BLACK);
				rectangle(2 + j * 13, 2 + tmp * 13, 2 + j * 13 + 13, 2 + tmp * 13 + 13);
				Board[tmp][j] = 0;			
				if (Board[tmp - 1][j] == 2)
				{
					setcolor(BLACK);
					rectangle(2 + j * 13, 2 + (tmp-1) * 13, 2 + j * 13 + 13, 2 + (tmp - 1) * 13 + 13);
					Board[tmp][j] = 2;
				}
			}
		}
		k--;
	}
	drawagain();
}

/*int main() {
	int gd, gm;
	int color = 15, tmpversion;
	gd = DETECT;
	initgraph(&gd, &gm, NULL);
	char c;
	float  delay = 0.3, speed = 0.3;
	int timer = 0;
	char timehalving = 0;
	rectangle(0, 0, 13 * 13 + 6, 35 * 13 + 6);// ve khung

	

	do {
		//time_t rawtime;
		//struct tm *timeStr;
		//char second[3];
		//string secondStr;
		//time(&rawtime);
		//timeStr = localtime(&rawtime);
		//strftime(second,3, "%S", timeStr);
		//secondStr = string() +second[0]  + second[1];
		//timer = stoi(secondStr);
		
		drawNewBlockVersion();

		//timer += 1;
		//cout << timer;
		//if (timer % 2 == 0) {
		//	clearOldBlockVersion();
		//	fallingBlockRow++;
		//	drawNewBlockVersion();
		//}
		
		if (kbhit())
			{
				c = toupper(getch());
				clearOldBlockVersion();
				switch (c)
				{
				case 'W':// Doi chieu cho block 
					if (fallingBlockNum == 0 || fallingBlockNum == 1)			// 2 block T,L co 4 chieu
						tmpversion = (fallingBlockVersion + 1) % 4;
					else if (fallingBlockNum == 4 || fallingBlockNum == 3)		// 2 block Z,I co 2 chieu
						tmpversion = (fallingBlockVersion + 1) % 2;

					if (fallingBlockNum != 2) {
						clearOldBlockVersion();
						fallingBlockVersion = tmpversion;
						blockarray = getFallingBlockArray();
					}
					system("cls");
					break;
				case 'D':// Sang phai
					fallingBlockCol++;
					system("cls");
					break;

				case 'A':// Sang trai
					fallingBlockCol--;
					system("cls");
					break;

				case 'S'://Xuong duoi
					fallingBlockRow++;
					system("cls");

				}		
				drawNewBlockVersion();
				

				for (int i = 0; i < MaxI; i++)
				{
					int count = 0;
					for (int j = 0; j < MaxJ; j++)
					{
						//cout << Board[i][j];
						count += Board[MaxI - 1][j];
						if (count == 26)
							checklines2();
					}
				}
		}
		if (isAtBottom()) {
			for (int i = 0; i < 8; i += 2) {
				Board[fallingBlockRow + blockarray[i]][fallingBlockCol + blockarray[i + 1]] = 2;  // <=> Board[r][c]
			}
			nextBlock();
			drawNewBlockVersion();
		}
	} while (1);

	getch();
	return 0;

}*/