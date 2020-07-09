#include "graphics.h"
#include <iostream>
#include "Blocks.h"
#include <time.h>
#include "Timer.h"
#include "Score.h"
#include "SoundManager.h"
#include "Animation.h"

using namespace std;

int cusPosX = boardPosX + boardX * size + 50;
int cusPosY = boardPosY + 50;
int holdPosX = boardPosX - 100;
int holdPosY = boardPosY + 50;

bool debugKey = false;

//Name: Engine
//Type: Big Boss MKIII
//Description: The whole f*cking self-made Game Engine
//Special Move: run the Game
//Gender: Female
//Birthdate: 5th May 2020
//Bloodtype: ...
//Favorite BattleGround: the graphics.h window
//Favorite Food: RAM
//Favorite Drink: none
//Favorite Attack Move: making the developer looks hella confused with her functions
//Battlecry quote: Oho? You're coming at me? Instead of running away you are coming straight at a self-made Tetris Game Engine?

class Engine{
private:
	Sound soundManager;
	Animation ani;
	Board board;
	Block blocks[7];
	Block controlBlock, nextBlock, holdBlock;
	int ranPointer, holdTrigger, penaltyTick, level, lines, nextLvlReq, chain;
	bool isPaused, isGameOver;
	float limit1, delay, limit2, tick, chance1, chance2, chance3, chance4, chance5, chance6, chance7;
	Timer timer;
	Score score;

public:
	//Constructor: starts the timer, gives default value, calculate the chances, draw the board, draw any custom boards, make a block.
	Engine(){
		timer.start();
		holdTrigger = 0;
		limit1 = timer.elapsedMilliseconds();
		tick = 30;
		delay = 60/tick*1000;
		penaltyTick = 0;
		level = 1;
		lines = 0;
		nextLvlReq = 5 * level + lines;
		chain = 0;
		isPaused = false;
		isGameOver = false;
		blocks[0] = TBlock();
		blocks[1] = IBlock();
		blocks[2] = OBlock();
		blocks[3] = LBlock();
		blocks[4] = JBlock();
		blocks[5] = SBlock();
		blocks[6] = ZBlock();
		CalculateChances();
		//board.boardArray[23][2] = 1; //Debug
		board.DrawBoardArray();
		board.DrawCusBoard(holdPosX-2,holdPosY-2,holdPosX+size/2*4+2,holdPosY+size/2*4+2);
		board.DrawCusBoard(cusPosX-2,cusPosY-2,cusPosX+size/2*4+2,cusPosY+size/2*4+2);
		//blocks[0].DrawBlock(WHITE);
		CreateNextBlock();

	}

	//run RNG, make block, rebalances the chances (so that one block cannot be too greedy and comes to play 5 times in a row ffs)
	void CreateNextBlock(){
		ranPointer = (float)((rand())/float((RAND_MAX)) * 16);
		cout << ranPointer<< endl;
		//nextBlock = IBlock();
		if(ranPointer >= 0 && ranPointer <= chance1){
			nextBlock = blocks[0];
			ChanceController(0);
		}
		else if (ranPointer > chance1 && ranPointer <= chance2){
			nextBlock = blocks[1];
			ChanceController(1);
		}
		else if (ranPointer > chance2 && ranPointer <= chance3){
			nextBlock = blocks[2];
			ChanceController(2);
		}
		else if (ranPointer > chance3 && ranPointer <= chance4){
			nextBlock = blocks[3];
			ChanceController(3);
		}
		else if (ranPointer > chance4 && ranPointer <= chance5){
			nextBlock = blocks[4];
			ChanceController(4);
		}
		else if (ranPointer > chance5 && ranPointer <= chance6){
			nextBlock = blocks[5];
			ChanceController(5);
		}
		else if (ranPointer > chance6){
			nextBlock = blocks[6];
			ChanceController(6);
		}


	}

	//Make a playable block from the last block just created, make another one, tell the board array that there is a block that's moving here
	void CreateBlock(){
		controlBlock = nextBlock;
		nextBlock.DrawCustomBlock(cusPosX,cusPosY,BLACK);
		//controlBlock = JBlock();
		controlBlock.DrawBlock(WHITE);

		CreateNextBlock();
		nextBlock.DrawCustomBlock(cusPosX,cusPosY,WHITE);

		for (int i = 0; i <=3; i++) {
			for (int j = 0; j <=3; j++) {
				board.boardArray[i+spawnCoordinateY][j+spawnCoordinateX] = controlBlock.currentRotate.formation[i][j];
			}
		}
	}

	//Switching between the controlling block and the holding block
	void SwitchBlock(){
		RemoveOldPos();

		Block temp = holdBlock;
		holdBlock = controlBlock;
		controlBlock = temp;

		controlBlock.SetX(spawnCoordinateX);
		controlBlock.SetY(spawnCoordinateY);

		controlBlock.DrawBlock(WHITE);

		for (int i = 0; i <=3; i++) {
			for (int j = 0; j <=3; j++) {
				board.boardArray[i+spawnCoordinateY][j+spawnCoordinateX] = controlBlock.currentRotate.formation[i][j];
			}
		}
	};

	//Make block, draws score, draws current lvl, draws the lines needed to get to the next lvl
	void GameStart(){
		CreateBlock();
		UpdateScore();
		UpdateLevel();
		UpdateLines();
	}

	//Working with Timer. if limit2 (actively getting the milisecond elapsed from Timer.h) minus limit1 (Gotten a time point somewhere) greater than delay
	void InitTimer(){
		if (!isPaused){
			limit2 = timer.elapsedMilliseconds();
			if ((limit2 - limit1) > delay)
			{
				if(CheckGround(controlBlock.currentRotate)){
					if(penaltyTick>level){
						InitMoveDown();
						penaltyTick = 0;
					}
					else {
						penaltyTick++;
						cout<< "Penalty tick: " << penaltyTick << endl;
					}
				}
				else {
					InitMoveDown();
					penaltyTick = 0;
				}
				limit1 = timer.elapsedMilliseconds();
			}
		}

	}

	void InitPrintBoard(){
		board.PrintBoardArray();
	}

	void RemoveOldPos(){
		for (int i = 0; i <= 3; i++){
			for (int j = 0; j <= 3; j++) {
				if(board.boardArray[controlBlock.GetY()+i][controlBlock.GetX()+j] == controlBlock.currentRotate.formation[i][j] && board.boardArray[controlBlock.GetY()+i][controlBlock.GetX()+j] >= 1){
					board.boardArray[controlBlock.GetY()+i][controlBlock.GetX()+j] = 0;
				}
			}
		}
	}

	void AddNewPos(){
		for (int i = 0; i <= 3; i++){
			for (int j = 0; j <= 3; j++) {
				if(board.boardArray[controlBlock.GetY()+i][controlBlock.GetX()+j] == 0 && controlBlock.currentRotate.formation[i][j] >= 1){
					board.boardArray[controlBlock.GetY()+i][controlBlock.GetX()+j] = controlBlock.currentRotate.formation[i][j];
				}
			}
		}
	}

	void CalculateChances(){
		chance1 = blocks[0].GetChance();
		chance2 = chance1 + blocks[1].GetChance();
		chance3 = chance2 + blocks[2].GetChance();
		chance4 = chance3 + blocks[3].GetChance();
		chance5 = chance4 + blocks[4].GetChance();
		chance6 = chance5 + blocks[5].GetChance();
	}

	void ChanceController(int blockID){
		blocks[blockID].SetChance(blocks[blockID].GetChance()-1);

		for (int i = 0; i <= 6; i++){
			if (i != blockID){
				blocks[i].SetChance(blocks[i].GetChance() + (double)1/6);
			}
		}

		CalculateChances();
	}

	int CheckGround(Formation form){
		for (int i = 0; i <= 3; i++){
			for (int j = 0; j <= 3; j++) {
				if(form.formation[i][j] == 2) {
					if(board.boardArray[controlBlock.GetY()+i+1][controlBlock.GetX() + j] >= 1) {
						return 1;
					}
					if(controlBlock.GetY()+i+1 > boardY - 1) {
						return 1;
					}
				}
			}
		}
		return 0;
	}

	int CheckAbove(Formation form){
		for (int i = 0; i <= 3; i++){
			for (int j = 0; j <= 3; j++) {
				if(form.formation[i][j] >= 1) {
					if(board.boardArray[controlBlock.GetY()+i-1][controlBlock.GetX() + j] >= 1) {
						return 1;
					}
				}
			}
		}
		return 0;
	}

	int CheckBorder(Formation form, int lr){
		switch (lr){
		case 0:{ //LEFT
			for (int i = 0; i <= 3; i++){
				for (int j = 0; j <= 3; j++) {
					if(form.left[i][j] == 1) {
						if(board.boardArray[controlBlock.GetY()+i][controlBlock.GetX() + j - 1] >= 1){
							return 1;
						}
						if(controlBlock.GetX()+j-1 < 0){
							ani.PlaySideBumpL();
							return 1;
						}
					}
				}
			}
			return 0;
			   }
		case 1:{ //RIGHT
			for (int i = 0; i <= 3; i++){
				for (int j = 0; j <= 3; j++) {
					if(form.right[i][j] == 1) {
						if(board.boardArray[controlBlock.GetY()+i][controlBlock.GetX() + j + 1] >= 1){
							return 1;
						}
						if(controlBlock.GetX()+j+1 > boardX - 1){
							ani.PlaySideBumpR();
							return 1;
						}
					}
				}
			}
			return 0;
			   }


		}
	}

	int CheckSpinable(Formation formation){
		int l = 0,r = 0,u = 2;
		for (int i = 0; i <= 3; i++){
			for (int j = 0; j <= 3; j++) {
				if(formation.left[i][j] == 1) {
					if(controlBlock.GetY()+i > boardY - 1){
						l = 2;
						r = 2;
						goto CheckDown;
					}
					if(board.boardArray[controlBlock.GetY() + i][controlBlock.GetX() + j] >= 1){
						l = 1;
						goto CheckRight;
					}
					if(controlBlock.GetX() + j < 0){
						l = 1;
						goto CheckRight;
					}
				}
			}
		}

CheckRight:
		for (int i = 0; i <= 3; i++){
			for (int j = 0; j <= 3; j++) {
				if(formation.right[i][j] == 1) {
					if(board.boardArray[controlBlock.GetY() + i][controlBlock.GetX() + j] >= 1){
						r = 1;
						goto CheckDown;
					}
					if(controlBlock.GetX() + j > boardX - 1){
						r = 1;
						goto CheckDown;
					}
				}
			}
		}

CheckDown:
		for (int i = 0; i <= 3; i++){
			for (int j = 0; j <= 3; j++) {
				if(formation.formation[i][j] == 2) {
					if(board.boardArray[controlBlock.GetY() + i][controlBlock.GetX() + j] >= 1){
						goto IsAboveOccupied;
					}
					if(controlBlock.GetY() + i > boardPosY - 1) {
IsAboveOccupied:
						if(CheckAbove(formation)){
							u = 1;
							goto Finale;
						}
						else {
							u = 0;
							goto Finale;
						}
					}

				}
			}
		}

Finale:
		if(l == 0 && r == 0 && (u == 0 || u == 2))
			return 1;
		if(l == 0 && CheckBorder(formation,0))
			l = 1;
		if(r == 0 && CheckBorder(formation,1))
			r = 1;
		if ((l == 1 || l == 2) && (r == 1 || r == 2))
			return 0;
		if(l == 0 && r == 0 && u == 1)
			return 0;
		if(l == 0)
			controlBlock.SetX(controlBlock.GetX()-1);
		if(r == 0)
			controlBlock.SetX(controlBlock.GetX()+1);
		if(u == 0)
			controlBlock.SetY(controlBlock.GetY()-1);
		return 1;
	}

	void UpdateScore(){
		score.DrawScore(boardPosX+boardX*size+20,boardPosY+boardY*size-50);
	}

	void UpdateLevel(){
		setcolor(WHITE);
		char lvl[10];
		sprintf(lvl,"Level %d",level);
		outtextxy(boardPosX-100,boardPosY+boardY*size-size*10,lvl);
	}

	void UpdateLines(){
		setcolor(WHITE);
		char lineLeft[50];
		sprintf(lineLeft,"%d / %d",lines,nextLvlReq);
		outtextxy(boardPosX-100,boardPosY+boardY*size-size*9,lineLeft);
	}

	void InitSpinBlock(){
		RemoveOldPos();
		controlBlock.RemoveBlock();
		if(CheckSpinable(controlBlock.getNextRotate())){
			controlBlock.SpinBlock();
		}
		AddNewPos();
		controlBlock.DrawBlock(controlBlock.GetX(),controlBlock.GetY(),WHITE);
	}

	void InitRevSpinBlock(){
		RemoveOldPos();
		controlBlock.RemoveBlock();
		if(CheckSpinable(controlBlock.getPrevRotate())){
			controlBlock.RevSpinBlock();
		}
		AddNewPos();
		controlBlock.DrawBlock(controlBlock.GetX(),controlBlock.GetY(),WHITE);
	}

	void InitMoveDown(){
		if(!CheckGround(controlBlock.currentRotate)){
			RemoveOldPos();
			controlBlock.MoveDown();
			AddNewPos();
		}
		else if (CheckGround(controlBlock.currentRotate)){
			holdTrigger = 0;
			controlBlock.RemoveBlock();
			int add = board.ClearLines();
			if (add > 0) {
				int Num1 = 100, Num2 = 100, tempNum;
				lines = lines + add;
				for (int i = 1; i <= add; i++){
					tempNum = Num1 + Num2;
					Num1 = Num2;
					Num2 = tempNum;
				}
				score.SetScore(score.GetScore()+tempNum+tempNum*0.5*chain);
				UpdateLines();
				if(chain > 0){
					char temp[20];
					if(chain == 1 ) sprintf(temp,"1 Chain!");
					else sprintf(temp,"%d Chain!", chain);
					outtextxy(boardPosX-100,boardPosY+boardY*size-size*7,temp);
				}	
				chain++;
				ani.PlayBigBump();
				soundManager.PLaySFXClearSound();
			}
			else {
				chain = 0;
				outtextxy(boardPosX-100,boardPosY+boardY*size-size*7,"                      ");
				ani.PlayGroundBump();
				soundManager.PlaySFXHitSound();
			}
			UpdateScore();
			isGameOver = board.getGameOver();
			if(!isGameOver){
				InitNextLevel();
				board.DrawBoardArray();
				CreateBlock();
			}
			else if(isGameOver){
				InitStop();
			}
		}
	}

	void InitInstantDown(){  //RECURSIVE
		RemoveOldPos();
		if(!CheckGround(controlBlock.currentRotate)){
			controlBlock.SetY(controlBlock.GetY()+1);
			score.SetScore(score.GetScore()+2);
			InitInstantDown();
		}
		else if (CheckGround(controlBlock.currentRotate)){
			controlBlock.DrawBlock(controlBlock.GetX(),controlBlock.GetY(),WHITE);
			AddNewPos();
			InitMoveDown();
		}
	}

	void InitMoveLeft(){
		RemoveOldPos();
		if(!CheckBorder(controlBlock.currentRotate,0)) {
			controlBlock.MoveLeft();
		}
		AddNewPos();
	}

	void InitMoveRight(){
		RemoveOldPos();
		if(!CheckBorder(controlBlock.currentRotate,1)) {
			controlBlock.MoveRight();
		}
		AddNewPos();
	}

	void InitHoldBlock(){
		if(holdTrigger == 0){
			holdBlock.DrawCustomBlock(holdPosX,holdPosY,BLACK);

			if(holdBlock.currentRotate.formation[0][0] < 0){
				holdBlock = controlBlock;
				RemoveOldPos();
				board.ClearBoard();
				CreateBlock();
			}
			else {
				SwitchBlock();
			}

			holdTrigger = 1;
			board.DrawBoardArray();
			holdBlock.DrawCustomBlock(holdPosX,holdPosY,WHITE);
		}
	}

	void InitNextLevel(){
		if (lines >= nextLvlReq){
			tick *= 1.5;
			delay = 60/tick*1000;
			level++;
			nextLvlReq = lines + level * 5;
			UpdateLevel();
			UpdateLines();
		}
	}

	void InitPause(){
		if (!isPaused){
			isPaused = true;
			cout<<"Paused!"<<endl;
		}
		else {
			isPaused = false;
			cout<<"Unpaused!"<<endl;
		}

	}

	void InitStop(){
		isPaused = true;
		if(isGameOver){
			if(score.GetScore() > score.GetHiScore()){
				outtextxy((boardPosX+boardX*size)/2+100,boardPosY+boardY*size+50,"New highscore!");
				score.SetHiScore(score.GetScore());
			}
			outtextxy((boardPosX+boardX*size)/2,boardPosY+boardY*size+50,"GAME OVER!");
		}
		else outtextxy((boardPosX+boardX*size)/2,boardPosY+boardY*size+50,"QUITTED!");
		outtextxy((boardPosX+boardX*size)/2,boardPosY+boardY*size+80,"Press 1 to play again");
		outtextxy((boardPosX+boardX*size)/2,boardPosY+boardY*size+110,"Press 2 to quit");
	}

	void DebugInput(){
		if(debugKey == true) {
			debugKey = false;
			cout<<"Debug key OFF"<<endl;
		}

		else  {
			debugKey = true;
			cout<<"Debug key ON"<<endl;
		}
	}

	int Input(char c){
		if(debugKey == true)
			cout <<"Key pressed: " << c << endl;

		if(!isPaused && !isGameOver)
			switch (c)
		{
			case 'W':
				InitSpinBlock();
				break;
			case 'Q':
				InitRevSpinBlock();
				break;
			case 'S':
				score.SetScore(score.GetScore()+1);
				UpdateScore();
				InitMoveDown();
				limit1 = timer.elapsedMilliseconds();
				break;
			case 'A':
				InitMoveLeft();
				break;
			case 'D':
				InitMoveRight();
				break;
			case 'R':
				InitHoldBlock();
				break;
			case ' ':
				InitInstantDown();
				break;
			case 'L':
				InitPrintBoard();
				break;
			case 'P':
				InitPause();
				break;
			case '`':
				cout << "Game quitted!" << endl;
				InitStop();
				isGameOver = true;
				break;
			case 'O':
				DebugInput();
				break;
			case '=':
				tick *= 1.5;
				delay = 60/tick*1000;
				level++;
				cout <<"Current tick: " << tick << endl;
				break;
			case '-':
				if(tick * 2/3 >= 30 )
					tick *= (double)2/3;
				delay = 60/tick*1000;
				level--;
				cout <<"Current tick: " << tick << endl;
				break;
		}

		else if(isPaused && !isGameOver)
			switch (c)
		{
			case 'L':
				InitPrintBoard();
				break;
			case 'P':
				InitPause();
				break;
			case 'O':
				DebugInput();
				break;
			case '=':
				tick *= 1.5;
				delay = 60/tick*1000;
				level++;
				cout <<"Current tick: " << tick << endl;
				break;
			case '-':
				tick *= 2/3;
				delay = 60/tick*1000;
				level++;
				cout <<"Current tick: " << tick << endl;
				break;
		}

		else if (isGameOver) {
			switch (c)
			{
			case '1':
				return 0;
			case '2':
				return 1;
			}
		}
		return 2;
	}

};	