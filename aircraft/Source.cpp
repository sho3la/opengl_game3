#include <iostream>
#include "mainCore.h"
#include "Game.h"
using namespace std;



int main()
{
	mainCorePtr x = mainCore::get();
	if(x->init())
		x->start(GamePtr(new Game()));

	return 0;
}