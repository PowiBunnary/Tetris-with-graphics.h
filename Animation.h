#include "graphics.h"
#include <thread>
#include "Drawer.h"
//#include "Blocks.h"
#pragma comment(lib, "graphics.lib")

using namespace std;

//Ever heard or Multi-threading?
//Basically it can have multiple functions to run at the same time, instead of the usual code 1 -> code 2 -> function 1 -> function 2 -> code 3 -> code 4, it goes like this
//	code 1 ->	code 2 ->	function 1 ->	code 4
//							function 2
//							code 3
//So there, Animation class does like whatever-it-feels-like without hampering the codes

class Animation {
public:
	void PlayGroundBump(){
		thread line1 (drawFadeoutLine, boardPosX + size, boardPosY +boardY * size + 10, boardPosX + boardX*size - size, boardPosY +boardY * size + 10, 0);
		line1.detach();
		thread line2 (drawFadeoutLine, boardPosX + size * 2, boardPosY +boardY * size + 20, boardPosX + boardX*size - size * 2, boardPosY +boardY * size + 20, 25);
		line2.detach();
		thread line3 (drawFadeoutLine, boardPosX + size * 3, boardPosY +boardY * size + 30, boardPosX + boardX*size - size * 3, boardPosY +boardY * size + 30, 50);
		line3.detach();
	}

	void PlayBigBump(){
		thread line1 (drawBigFadeoutLine, boardPosX + size, boardPosY +boardY * size + 10, boardPosX + boardX*size - size, boardPosY +boardY * size + 10, 0);
		line1.detach();
		thread line2 (drawBigFadeoutLine, boardPosX + size, boardPosY +boardY * size + 20, boardPosX + boardX*size - size, boardPosY +boardY * size + 20, 25);
		line2.detach();
		thread line3 (drawBigFadeoutLine, boardPosX + size, boardPosY +boardY * size + 30, boardPosX + boardX*size - size, boardPosY +boardY * size + 30, 50);
		line3.detach();
		thread line4 (drawBigFadeoutLine, boardPosX + size, boardPosY +boardY * size + 40, boardPosX + boardX*size - size, boardPosY +boardY * size + 40, 75);
		line4.detach();
		thread line5 (drawBigFadeoutLine, boardPosX + size, boardPosY +boardY * size + 50, boardPosX + boardX*size - size, boardPosY +boardY * size + 50, 100);
		line5.detach();
	}

	void PlaySideBumpL(){
		thread line1 (drawFadeoutBorderL,boardPosX-20,boardPosY+size*4,boardPosX-20,boardPosY+boardY*size);
		line1.detach();
	}

	void PlaySideBumpR(){
		thread line1 (drawFadeoutBorderR,boardPosX+boardX*size+20,boardPosY+size*4,boardPosX+boardX*size+20,boardPosY+boardY*size);
		line1.detach();
	}
};