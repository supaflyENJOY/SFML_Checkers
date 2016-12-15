#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <list>
#include "Checker.h"
using namespace sf;
using namespace std;

#pragma once

class Sockets;

class GameBoard {
private:
	Texture boardTexture;
	Sprite boardShape;
	list <Checker *> entity;
	int gameState;
	int subState;
	int teams[3];
	int myTeam;
	bool isOnline;
	Checker *selected;
	Sockets *socket;
	Texture *checkerTexture;
public:
	GameBoard(bool _isOnline, Texture *_checkerTexture);

	void Clear();

	void EndGame();

	void StartGame();

	void RemoveByXY(int x, int y);

	void ChangeState(int state);

	void MoveByXY(int x, int y, int new_x, int new_y);

	void SetSockets(Sockets *sock);

	void SetMyTeam(CheckerTeam team);

	void ProceedRightClick(int _x, int _y);

	void ProceedLeftClick(int _x, int _y);

	void Draw(RenderWindow *wnd);

};