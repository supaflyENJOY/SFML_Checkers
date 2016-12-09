#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <list>  
#include <cmath>  

#include "Checker.h"
#include "GameBoard.h"
#include "Sockets.h"
#include "main.h"


using namespace sf;
using namespace std;

int width = 504;
int height = 504;

// test param cuz no menu
bool online = true;

int main() {
	RenderWindow window(VideoMode(width, height), "Checkers");
	GameBoard gb = GameBoard(online);
	Sockets sockets("149.154.66.55", 6969);
	gb.SetSockets(&sockets);
	if (online == false) {
		gb.StartGame();
	} else {
		sockets.SetGameBoard(&gb);
		sockets.Connect();
	}

	while (window.isOpen()) {
		Vector2i pos = Mouse::getPosition(window);
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
			if (event.type == Event::MouseButtonPressed) {
				if (event.key.code == Mouse::Left) {
					gb.ProceedLeftClick(pos.x, pos.y);
				}
				if (event.key.code == Mouse::Right) {
					gb.ProceedRightClick(pos.x, pos.y);
				}
			}
		}

		window.clear();
		gb.Draw(&window);
		window.display();
	}

	return 0;
}