#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include "Checker.h"
using namespace sf;
using namespace std;

#pragma once

class Sockets;

class GameBoard {
private:
	Texture boardTexture;
	Sprite boardShape;
	vector<Checker> entity;
	int gameState;
	int subState;
	int teams[2];
	int myTeam;
	bool isOnline;
	Checker *selected;
	Sockets *socket;
public:
	GameBoard(bool _isOnline);

	void Clear();

	void EndGame();

	void StartGame();

	void SetSockets(Sockets *sock);

	void SetMyTeam(CheckerTeam team);

	void ProceedRightClick(int _x, int _y);

	void ProceedLeftClick(int _x, int _y);

	void Draw(RenderWindow *wnd);

};