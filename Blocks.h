#include "graphics.h"
#include <iostream>
#include "Drawer.h"

#pragma comment(lib, "graphics.lib")

using namespace std;

//This here holds anything that's related to the playarea and the blocks to play with, in a graphics and array manner,
//it holds any function that helps creating the board/blocks and interacting with it
//Tbh thats a lot of arrays kek.

int size = 30;
int boardX= 10, boardY = 25, boardPosX = 150, boardPosY = 50;
int spawnCoordinateX = 3, spawnCoordinateY = 0; 

struct Formation {
	int formation[4][4]; //General Formation template
	int left[4][4], right[4][4];
};

class Board {
public:
	int boardArray[25][10]; //Standard Tertris board size
	bool gameOver;

	Board(){
		for (int i = 0; i <boardY; i++) {
			for (int j = 0; j <boardX; j++) {
				boardArray[i][j] = 0;
			}
		}
		gameOver = false;
	}

	bool getGameOver(){
		return gameOver;
	}

	void DrawCusBoard(int left, int top, int down, int right){
		setcolor(7);
		setfillstyle(0,0);

		rectangle(left, top, down, right);
		floodfill(left+1,top+1,7);
	}

	void PrintBoardArray(){
		for (int i = 0; i <boardY; i++) {
			for (int j = 0; j <boardX; j++) {
				cout << boardArray[i][j] << " ";
			}
			cout << endl << endl;
		}
		cout << endl;
	}

	void ClearBoard(){
		setfillstyle(0,BLACK);
		floodfill(boardPosX+1,boardPosY+size*4+1,7);
	}
		
	void DrawBoardArray(){
		//For clearing the board
		setcolor(7);
		rectangle(boardPosX-1,boardPosY-1,boardPosX+size*boardX+1,boardPosY+size*boardY+1);

		ClearBoard();
		setcolor(WHITE);
		setcolor(WHITE);
		rectangle(boardPosX-2,boardPosY-2,boardPosX+size*boardX+2,boardPosY+size*boardY+2);

		rectangle(boardPosX-2,boardPosY+size*4-2,boardPosX+size*boardX+2,boardPosY+size*boardY+2);
		setcolor(7);
		rectangle(boardPosX-1,boardPosY-1,boardPosX+size*boardX+1,boardPosY+size*boardY+1);
		for (int i = boardY-1; i >=0; i--) {
			for (int j = 0; j <boardX; j++) {
				if(boardArray[i][j] >= 1){
					int x = boardPosX + size * j;
					int y = boardPosY + size * i;
					drawSquare(x,y,size,WHITE);
				}
			}
		}
	}

	int IsLineFull(int i){
		for (int j = 0; j <boardX; j++) {
			if (boardArray[i][j] == 0) {
				return 0;
			}
		}
		setcolor(12);
		rectangle(boardPosX,boardPosY+size*i,boardPosX+size*boardX,boardPosY+size*(i+1));
		setfillstyle(1,12);
		//delay(1000);
		floodfill(boardPosX+1,boardPosY+size*i+1,12);
		delay(50);
		return 1;
	}

	int IsLineClear(int i){
		for (int j = 0; j <boardX; j++) {
			if (boardArray[i][j] >= 1) {
				return 0;
			}
		}
		return 1;
	}

	void MoveLinesDown(int k){ //RECURSIVE
		for(int j = 0; j <boardX; j++){
			boardArray[k][j] = boardArray[k-1][j];
		}

		if(IsLineClear(k)){
			return;
		}

		MoveLinesDown(k-1);
	}

	int ClearLines(){
		int lines = 0;
		for (int i = 0; i <boardY; i++) {
			if(i == 3 && !IsLineClear(3)){
				gameOver = true;
			}
			if (IsLineFull(i)){
				lines++;
				MoveLinesDown(i);
			}
		}
		return lines;
	}
} ;

class Block {
protected:
	Formation rotate[4]; //4 arrays of rotations
	int pos, maxRotate; //rotation status and max number of rotate
	int x, y;
	float chance;

public:
	Formation currentRotate; //Current rotation array

	Block(){
		for (int i = 0; i <=3; i++) {
			for (int j = 0; j <=3; j++) {
				//Make sure every spaces in the array are 0, not NULL
				rotate[0].formation[i][j] = rotate[0].left[i][j] = rotate[0].right[i][j] = 0;
				rotate[1].formation[i][j] = rotate[1].left[i][j] = rotate[1].right[i][j] = 0;
				rotate[2].formation[i][j] = rotate[2].left[i][j] = rotate[2].right[i][j] = 0;
				rotate[3].formation[i][j] = rotate[3].left[i][j] = rotate[3].right[i][j] = 0;
				pos = 1;
				x = spawnCoordinateX;
				y = spawnCoordinateY;
				chance = 2;
			}
		}
	};

	int GetX(){
		return x;
	}

	void SetX(int x){
		this->x = x;
	}

	int GetY(){
		return y;
	}

	void SetY(int y){
		this->y = y;
	}

	float GetChance(){
		return chance;
	}

	void SetChance(float chance){
		this->chance = chance;
	}

	Formation getNextRotate(){
		if (pos == maxRotate){
			return rotate[0];
		}
		return rotate[pos];
	}

	Formation getPrevRotate(){
		if (pos-2 < 0){
			return rotate[3];
		}
		return rotate[pos-2];
	}

	void DrawBlock(int posX, int posY, int color){
		for (int i = 0; i <= 3; i++){
			for (int j = 0; j <= 3; j++) {
				if (currentRotate.formation[i][j] == 1 || currentRotate.formation[i][j] == 2) {
					int x = boardPosX + size*posX + size * j;
					int y = boardPosY + size*posY + size * i;
					drawSquare(x,y,size,color);

					//Debug mode
					//if (currentRotate.formation[i][j] == 2 && color != BLACK)
					//	drawSquare(x,y,size,YELLOW);
				}
			}
		}
	}

	void DrawBlock(int color){
		for (int i = 0; i <= 3; i++){
			for (int j = 0; j <= 3; j++) {
				if (currentRotate.formation[i][j] == 1 || currentRotate.formation[i][j] == 2) {
					int x = boardPosX + size*spawnCoordinateX + size * j;
					int y = boardPosY + size*spawnCoordinateY + size * i;
					drawSquare(x,y,size,color);

					//Debug mode
					//if (currentRotate.formation[i][j] == 2 && color != BLACK)
					//	drawSquare(x,y,size,YELLOW);
				}
			}
		}
	}

	void DrawCustomBlock(int posX, int posY, int color){
		int customSize = size / 2;
			for (int i = 0; i <= 3; i++){
			for (int j = 0; j <= 3; j++) {
				if (currentRotate.formation[i][j] == 1 || currentRotate.formation[i][j] == 2) {
					int x = posX + customSize * j;
					int y = posY + customSize * i;
					drawSquare(x,y,customSize,color);
				}
			}
		}
	}

	void SpinBlock(){
		DrawBlock(x,y,BLACK); //Remove the current block
		if (pos == maxRotate) {
			pos = 1;
		}
		else pos++;
		currentRotate = rotate[pos-1];
		DrawBlock(x,y,WHITE); //Redraw it
	}

	void RevSpinBlock(){
		DrawBlock(x,y,BLACK); //Remove the current block
		if (pos == 1) {
			pos = maxRotate;
		}
		else pos--;
		currentRotate = rotate[pos-1];
		DrawBlock(x,y,WHITE); //Redraw it
	}

	void MoveDown(){
		DrawBlock(x,y,BLACK);
		y++;
		DrawBlock(x,y,WHITE);
	}

	void MoveLeft(){
		DrawBlock(x,y,BLACK);
		x--;
		DrawBlock(x,y,WHITE);
	}

	void MoveRight(){
		DrawBlock(x,y,BLACK);
		x++;
		DrawBlock(x,y,WHITE);
	}

	void RemoveBlock(){
		DrawBlock(x,y,BLACK);
	}
};

class TBlock : public Block {
public:
	TBlock(){
		maxRotate = 4; //Set the number of max rotations (T block has 4 rotations)

		//Setting the rotation formation arrays
		rotate[0].formation[0][1] = 1;
		rotate[0].formation[1][0] = 2;
		rotate[0].formation[1][1] = 2;
		rotate[0].formation[1][2] = 2;
		rotate[0].left[0][1] = rotate[0].left[1][0] = 1;
		rotate[0].right[0][1] = rotate[0].right[1][2] = 1;

		rotate[1].formation[0][1] = 1;
		rotate[1].formation[1][0] = 2;
		rotate[1].formation[1][1] = 1;
		rotate[1].formation[2][1] = 2;
		rotate[1].left[0][1] = rotate[1].left[1][0] = rotate[1].left[2][1] = 1;
		rotate[1].right[0][1] = rotate[1].right[1][0] = rotate[1].right[2][1] = 1;

		rotate[2].formation[1][0] = 2;
		rotate[2].formation[1][1] = 1;
		rotate[2].formation[1][2] = 2;
		rotate[2].formation[2][1] = 2;
		rotate[2].left[1][0] = rotate[2].left[2][1] = 1;
		rotate[2].right[1][2] = rotate[2].right[2][1] = 1;

		rotate[3].formation[0][1] = 1;
		rotate[3].formation[1][1] = 1;
		rotate[3].formation[1][2] = 2;
		rotate[3].formation[2][1] = 2;
		rotate[3].left[0][1] = rotate[3].left[1][1] = rotate[3].left[2][1] = 1;
		rotate[3].right[0][1] = rotate[3].right[1][2] = rotate[3].right[2][1] = 1;

		//Set default rotation state
		currentRotate = rotate[pos-1];
	}
};

class IBlock : public Block {
public:
	IBlock(){
		maxRotate = 2;

		rotate[0].formation[0][1] = 1;
		rotate[0].formation[1][1] = 1;
		rotate[0].formation[2][1] = 1;
		rotate[0].formation[3][1] = 2;
		rotate[0].left[0][1] = rotate[0].left[1][1] = rotate[0].left[2][1] = rotate[0].left[3][1] = 1;
		rotate[0].right[0][1] = rotate[0].right[1][1] = rotate[0].right[2][1] = rotate[0].right[3][1] = 1;

		rotate[1].formation[2][0] = 2;
		rotate[1].formation[2][1] = 2;
		rotate[1].formation[2][2] = 2;
		rotate[1].formation[2][3] = 2;
		rotate[1].left[2][0] = 1;
		rotate[1].right[2][3] = 1;

		currentRotate = rotate[pos-1];
	}
};

class OBlock : public Block {
public:
	OBlock(){
		maxRotate = 1;

		rotate[0].formation[1][1] = 1;
		rotate[0].formation[1][2] = 1;
		rotate[0].formation[2][1] = 2;
		rotate[0].formation[2][2] = 2;
		rotate[0].left[1][1] = rotate[0].left[2][1] = 1;
		rotate[0].right[1][2] = rotate[0].right[2][2] = 1;

		currentRotate = rotate[pos-1];
	}
};

class LBlock : public Block {
public:
	LBlock(){
		maxRotate = 4;

		rotate[0].formation[0][2] = 1;
		rotate[0].formation[1][0] = 2;
		rotate[0].formation[1][1] = 2;
		rotate[0].formation[1][2] = 2;
		rotate[0].left[0][2] = rotate[0].left[1][0] = 1;
		rotate[0].right[0][2] = rotate[0].right[1][2] = 1;

		rotate[1].formation[0][0] = 2;
		rotate[1].formation[0][1] = 1;
		rotate[1].formation[1][1] = 1;
		rotate[1].formation[2][1] = 2;
		rotate[1].left[0][0] = rotate[1].left[1][1] = rotate[1].left[2][1] = 1;
		rotate[1].right[0][1] = rotate[1].right[1][1] = rotate[1].right[2][1] = 1;

		rotate[2].formation[1][0] = 1;
		rotate[2].formation[1][1] = 2;
		rotate[2].formation[1][2] = 2;
		rotate[2].formation[2][0] = 2;
		rotate[2].left[1][0] = rotate[2].left[2][0] = 1;
		rotate[2].right[1][2] = rotate[2].right[2][0] = 1;
		
		rotate[3].formation[0][1] = 1;
		rotate[3].formation[1][1] = 1;
		rotate[3].formation[2][1] = 2;
		rotate[3].formation[2][2] = 2;
		rotate[3].left[0][1] = rotate[3].left[1][1] = rotate[3].left[2][1] = 1;
		rotate[3].right[0][1] = rotate[3].right[1][1] = rotate[3].right[2][2] = 1;

		currentRotate = rotate[pos-1];
	}
};

class JBlock : public Block {
public:
	JBlock(){
		maxRotate = 4;

		rotate[0].formation[0][0] = 1;
		rotate[0].formation[1][0] = 2;
		rotate[0].formation[1][1] = 2;
		rotate[0].formation[1][2] = 2;
		rotate[0].left[0][0] = rotate[0].left[1][0] = 1;
		rotate[0].right[0][0] = rotate[0].right[1][2] = 1;
		
		rotate[1].formation[0][1] = 1;
		rotate[1].formation[1][1] = 1;
		rotate[1].formation[2][0] = 2;
		rotate[1].formation[2][1] = 2;
		rotate[1].left[0][1] = rotate[1].left[1][1] = rotate[1].left[2][0] = 1;
		rotate[1].right[0][1] = rotate[1].right[1][1] = rotate[1].right[2][1] = 1;

		rotate[2].formation[1][0] = 2;
		rotate[2].formation[1][1] = 2;
		rotate[2].formation[1][2] = 1;
		rotate[2].formation[2][2] = 2;
		rotate[2].left[1][0] = rotate[2].left[2][2] = 1;
		rotate[2].right[0][2] = rotate[2].right[1][2] = 1;

		rotate[3].formation[0][1] = 1;
		rotate[3].formation[0][2] = 2;
		rotate[3].formation[1][1] = 1;
		rotate[3].formation[2][1] = 2;
		rotate[3].left[0][1] = rotate[3].left[1][1] = rotate[3].left[2][1] = 1;
		rotate[3].right[0][2] = rotate[3].right[1][1] = rotate[3].right[2][1] = 1;

		currentRotate = rotate[pos-1];
	}
};

class SBlock : public Block {
public:
	SBlock(){
		maxRotate = 2;

		rotate[0].formation[0][1] = 1;
		rotate[0].formation[0][2] = 2;
		rotate[0].formation[1][0] = 2;
		rotate[0].formation[1][1] = 2;
		rotate[0].left[0][1] = rotate[0].left[1][0] = 1;
		rotate[0].right[0][2] = rotate[0].right[1][1] = 1;

		rotate[1].formation[0][1] = 1;
		rotate[1].formation[1][1] = 2;
		rotate[1].formation[1][2] = 1;
		rotate[1].formation[2][2] = 2;
		rotate[1].left[0][1] = rotate[1].left[1][1] = rotate[1].left[2][2] = 1;
		rotate[1].right[0][1] = rotate[1].right[1][2] = rotate[1].right[2][2] = 1;

		currentRotate = rotate[pos-1];
	}
};

class ZBlock : public Block {
public:
	ZBlock(){
		maxRotate = 2;

		rotate[0].formation[0][0] = 2;
		rotate[0].formation[0][1] = 1;
		rotate[0].formation[1][1] = 2;
		rotate[0].formation[1][2] = 2;
		rotate[0].left[0][0] = rotate[0].left[1][1] = 1;
		rotate[0].right[0][1] = rotate[0].right[1][2] = 1;

		rotate[1].formation[0][2] = 1;
		rotate[1].formation[1][1] = 1;
		rotate[1].formation[1][2] = 2;
		rotate[1].formation[2][1] = 2;
		rotate[1].left[0][2] = rotate[1].left[1][1] = rotate[1].left[2][1] = 1;
		rotate[1].right[0][2] = rotate[1].right[1][2] = rotate[1].right[2][1] = 1;

		currentRotate = rotate[pos-1];
	}
};

