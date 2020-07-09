#include "graphics.h"
#include <iostream>
#pragma comment(lib, "graphics.lib")

//This is the main header for drawing anything repeatedly onto the graphics screen (TODO: move anything that draws from the other functions to here if there's time)
//There is not much use to this one but then, Animation came along, and is in need of it, lots from it. So i guess we're keeping this then...
#ifndef DRAWER
#define DRAWER

void drawSquare(int x, int y, int length, int color) {
	setcolor(color);
	rectangle(x,y,x+length,y+length);
	line(x,y,x+length,y+length);
	line(x,y+length,x+length,y);
}

void drawFadeoutLine(int x1, int y1, int x2, int y2, int offset) {
	delay(offset);
	setcolor(DARKGRAY);
	line(x1,y1,x2,y2);
	delay(50);
	setcolor(LIGHTGRAY);
	line(x1,y1,x2,y2);
	delay(50);
	setcolor(WHITE);
	line(x1,y1,x2,y2);
	delay(500);
	setcolor(LIGHTGRAY);
	line(x1,y1,x2,y2);
	delay(100);
	setcolor(DARKGRAY);
	line(x1,y1,x2,y2);
	delay(100);
	setcolor(BLACK);
	line(x1,y1,x2,y2);
}

void drawBigFadeoutLine(int x1, int y1, int x2, int y2,  int offset) {
	delay(offset);
	setcolor(DARKGRAY);
	line(x1,y1,x2,y2);
	delay(50);
	setcolor(LIGHTRED);
	line(x1,y1,x2,y2);
	delay(50);
	setcolor(RED);
	line(x1,y1,x2,y2);
	delay(500);
	setcolor(LIGHTRED);
	line(x1,y1,x2,y2);
	delay(100);
	setcolor(DARKGRAY);
	line(x1,y1,x2,y2);
	delay(100);
	setcolor(BLACK);
	line(x1,y1,x2,y2);
}

void drawFadeoutBorderL(int x1, int y1, int x2, int y2) {
	setcolor(DARKGRAY);
	line(x1,y1,x1+10,y1);
	line(x1,y1,x2,y2);
	line(x2,y2,x2+10,y2);
	delay(25);
	setcolor(LIGHTGRAY);
	line(x1,y1,x1+10,y1);
	line(x1,y1,x2,y2);
	line(x2,y2,x2+10,y2);
	delay(25);
	setcolor(WHITE);
	line(x1,y1,x1+10,y1);
	line(x1,y1,x2,y2);
	line(x2,y2,x2+10,y2);
	delay(250);
	setcolor(LIGHTGRAY);
	line(x1,y1,x1+10,y1);
	line(x1,y1,x2,y2);
	line(x2,y2,x2+10,y2);
	delay(100);
	setcolor(DARKGRAY);
	line(x1,y1,x1+10,y1);
	line(x1,y1,x2,y2);
	line(x2,y2,x2+10,y2);
	delay(100);
	setcolor(BLACK);
	line(x1,y1,x1+10,y1);
	line(x1,y1,x2,y2);
	line(x2,y2,x2+10,y2);
}

void drawFadeoutBorderR(int x1, int y1, int x2, int y2) {
	setcolor(DARKGRAY);
	line(x1,y1,x1-10,y1);
	line(x1,y1,x2,y2);
	line(x2,y2,x2-10,y2);
	delay(25);
	setcolor(LIGHTGRAY);
	line(x1,y1,x1-10,y1);
	line(x1,y1,x2,y2);
	line(x2,y2,x2-10,y2);
	delay(25);
	setcolor(WHITE);
	line(x1,y1,x1-10,y1);
	line(x1,y1,x2,y2);
	line(x2,y2,x2-10,y2);
	delay(250);
	setcolor(LIGHTGRAY);
	line(x1,y1,x1-10,y1);
	line(x1,y1,x2,y2);
	line(x2,y2,x2-10,y2);
	delay(100);
	setcolor(DARKGRAY);
	line(x1,y1,x1-10,y1);
	line(x1,y1,x2,y2);
	line(x2,y2,x2-10,y2);
	delay(100);
	setcolor(BLACK);
	line(x1,y1,x1-10,y1);
	line(x1,y1,x2,y2);
	line(x2,y2,x2-10,y2);
}

#endif