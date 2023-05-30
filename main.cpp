#include <iostream>
#include <memory>

#include "./utils/checkML.h"
#include "./game/Game.h"

#include "Tracker.h"

void start() {

	Tracker::Instance();

	Game g(8000);
	g.init();
	g.start();
	g.close();

	Tracker::End();
}

int main(int, char**) {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	start();

	return 0;
}

