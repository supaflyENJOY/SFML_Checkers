#include <SFML/Graphics.hpp>
#include <iostream>
#include "Checker.h"
#include "GameBoard.h"
#include "Sockets.h"

using namespace sf;
using namespace std;


GameBoard::GameBoard(bool _isOnline) {
	boardTexture.loadFromFile("res/board.png");
	boardShape = Sprite(boardTexture);
	gameState = 0;
	subState = 0;
	isOnline = _isOnline;
}

void GameBoard::Clear() {
	entity.clear();
}

void GameBoard::SetMyTeam(CheckerTeam team) {
	myTeam = team;
}

void GameBoard::EndGame() {
	socket->sendMessage(MessageType::EndGame);
	Clear();
	cout << "Game end!" << endl;
	if(!isOnline) StartGame();
}

void GameBoard::StartGame() {
	teams[CheckerTeam::White] = 0;
	teams[CheckerTeam::Black] = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			entity.push_back(new Checker(j * 2 + (1-i % 2), i, CheckerTeam::White, CheckerType::Default));
			entity.push_back(new Checker(j * 2 + (i % 2), 7 - i, CheckerTeam::Black, CheckerType::Default));
			teams[CheckerTeam::White]++;
			teams[CheckerTeam::Black]++;
		}
	}
	gameState = CheckerTeam::White;
	subState = 1;
	cout << "Game started!" << endl;
}

void GameBoard::ProceedRightClick(int _x, int _y) {
	if ((isOnline == false || gameState == myTeam) && subState == 2) {
		selected->unsetMark();
		subState = 1;
	}
}

void GameBoard::ChangeState(int state) {
	gameState = state;
}

void GameBoard::SetSockets(Sockets *sock) {
	socket = sock;
}

void GameBoard::RemoveByXY(int x, int y) {
	for (auto s : entity) {
		if (s->checkIntersection(x, y)) {
			CheckerTeam t = s->getTeam();
			teams[t]--;
			entity.remove(s);
			if (teams[t] == 0) {
				EndGame();
			}
			break;
		}
	}
}

void GameBoard::MoveByXY(int x, int y, int new_x, int new_y) {
	cout << x << " " << y << " " << new_x << " " << new_y << endl;
	for (auto s : entity) {
		if (s->checkIntersection(x, y)) {
			s->Move(new_x, new_y);
			break;
		}
	}
}

void GameBoard::ProceedLeftClick(int _x, int _y) {
	if ((isOnline == false || gameState == myTeam) &&_x >= 30 && _x < 478 && _y >= 30 && _y < 478) {
		int gX = (_x - 30) / 56;
		int gY = (_y - 30) / 56;
		if (subState == 1) {
			cout << "sub: 1" << endl;
			selected = NULL;
			for (auto s : entity)
				if (s->checkIntersection(gX, gY) && s->getTeam() == gameState) {
					selected = s;
					selected->setMark();
					break;
				}
			if (selected != NULL) {
				subState = 2;
			}
		} else if (subState == 2) {
			cout << "sub: 2" << endl;
			int x, y, type = selected->getType(), team = selected->getTeam();
			selected->getPosition(&x, &y);
			int x_diff = (gX - x);
			int y_diff = (gY - y);
			cout << x_diff << " " << y_diff << endl;
			if (abs(x_diff) != abs(y_diff) || x_diff == 0) return;
			cout << x << " " << y << endl;
			int cx = x + copysign(1, x_diff), cy = y + copysign(1, y_diff);
			if (type == CheckerType::Default) {
				if (x_diff > 2) return;
				Checker* toRemove = NULL;
				cout << cx << " " << cy << endl;
				bool invalid = false, remove = false;;
				for (auto s : entity) {
					if (s->checkIntersection(cx, cy)) {
						if (s->getTeam() != selected->getTeam()) {
							toRemove = s;
							remove = true;
						} else {
							invalid = true;
						}
						break;
					}
				}
				if (invalid) return;
				if ((abs(x_diff) == 1 && remove == false && ((team == CheckerTeam::White && gY > y) || (team == CheckerTeam::Black && gY < y))) || (abs(x_diff) == 2 && remove == true)) {
					char s[3];
					selected->Move(gX, gY);
					selected->unsetMark();
					if (remove == true) {
						teams[3 - gameState]--;
						int ttx, tty;
						toRemove->getPosition(&ttx, &tty);
						socket->sendMessage(MessageType::Remove, ttx, tty);
						entity.remove(toRemove);
					}
					socket->sendMessage(MessageType::Move, x, y, gX, gY);
					gameState = 3 - gameState;
					socket->sendMessage(MessageType::ChangeState, gameState);
					subState = 1;
					if (teams[gameState] == 0) {
						EndGame();
					}
				}
			} else if (type == CheckerType::King) {
				Checker* toRemove = NULL;
				int tx, ty;
				int dist_c = abs(x_diff) + abs(y_diff);
				cout << cx << " " << cy << endl;
				bool invalid = false, remove = false;;
				for (auto s : entity) {
					s->getPosition(&tx, &ty);
					int dist_t = abs(tx - x) + abs(ty - y);
					if (abs(tx - x) == abs(ty - y) && tx != x && cx == x + copysign(1, tx - x) && cy == y + copysign(1, ty - y) && dist_t < dist_c) {
						if (s->getTeam() != selected->getTeam()) {
							if (remove) {
								remove = false;
								invalid = true;
								cout << "more";
								break;
							} else {
								toRemove = s;
								remove = true;
								cout << tx << " " << ty << endl;
							}
						} else {
							invalid = true;
						}

					}
				}
				cout << "inv";
				if (invalid) return;
				selected->unsetMark(); 
				char s[10];
				if (remove == true) {
					int tx, ty;
					toRemove->getPosition(&tx, &ty);
					gX = tx + copysign(1, tx - x);
					gY = ty + copysign(1, ty - y);
					selected->Move(gX, gY);
					socket->sendMessage(MessageType::Remove, tx, ty);
					socket->sendMessage(MessageType::Move, x, y, gX, gY);
					teams[3 - gameState]--;
					entity.remove(toRemove);
				} else {
					selected->Move(gX, gY);
					socket->sendMessage(MessageType::Move, x, y, gX, gY);
				}
				gameState = 3 - gameState;
				socket->sendMessage(MessageType::ChangeState, gameState);
				subState = 1;
				if (teams[gameState] == 0) {
					EndGame();
				}
			}

		}
	}
}

void GameBoard::Draw(RenderWindow *wnd) {
	wnd->draw(boardShape);
	for (auto s : entity) {
		if (s->created) wnd->draw(s->getShape());

	}
	/*for (vector<Checker>::reverse_iterator ci = entity.rbegin(); ci != entity.rend(); ++ci) {
	}*/
}