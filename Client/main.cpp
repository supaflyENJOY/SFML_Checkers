#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>  
#include <cmath>  

using namespace sf;
using namespace std;

int width = 504;
int height = 504;

//30 first
//422 last

enum CheckerTeam {
	None,
	White,
	Black
};
enum CheckerType {
	Default,
	King
};

class Checker {
private:
	int x;
	int y;
	CheckerTeam team;
	CheckerType type;
	CircleShape shape;
public:
	bool created;
	Checker() {
		created = false;
	}

	Checker(int _x, int _y, CheckerTeam _team, CheckerType _type) {
		shape = CircleShape(26);
		x = _x;
		y = _y;
		team = _team;
		type = _type;
		shape.setPosition(30 + 56 * x, 30 + 56 * y);
		shape.setFillColor(team == CheckerTeam::White ? Color::White : Color::Black);

		created = true;
	}

	CheckerTeam getTeam() {
		return team;
	}

	CheckerType getType() {
		return type;
	}

	void setMark() {
		shape.setFillColor(team == CheckerTeam::White ? Color::Yellow : Color::Color(70,70,70));
	}

	void unsetMark() {
		shape.setFillColor(team == CheckerTeam::White ? Color::White : Color::Black);
	}

	void Move(int _x, int _y) {
		x = _x;
		y = _y;
		shape.setPosition(30 + 56 * x, 30 + 56 * y);
	}

	bool checkIntersection(int _x, int _y) {
		if (x == _x && y == _y) {
			return true;
		}
		return false;
	}

	void getPosition(int *_x, int *_y) {
		*_x = x;
		*_y = y;
	}

	CircleShape& getShape() {
		return shape;
	}

	bool operator==(Checker other) const {
		return other.checkIntersection(x, y);
	}
};

class GameBoard {
private:
	Texture boardTexture;
	Sprite boardShape;
	vector<Checker> entity;
	int gameState;
	int subState;
	int teams[2];
	Checker *selected;
public:
	GameBoard() {
		boardTexture.loadFromFile("res/board.png");
		boardShape = Sprite(boardTexture);
		gameState = 0;
		subState = 0;
	}

	void Clear() {
		entity.clear();
	}

	void EndGame() {
		Clear();
		StartGame();
	}

	void StartGame() {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 4; j++) {
				entity.push_back(Checker(j * 2 + !(i % 2), i, CheckerTeam::White, CheckerType::Default));
				entity.push_back(Checker(j * 2 + (i % 2), 7 - i, CheckerTeam::Black, CheckerType::Default));
				teams[0]++;
				teams[1]++;
			}
		}
		gameState = CheckerTeam::White;
		subState = 1;
		cout << "Game started!" << endl;
	}
	void ProceedRightClick(int _x, int _y) {
		if (subState == 2) {
			selected->unsetMark();
			subState = 1;
		}
	}

	void ProceedLeftClick(int _x, int _y) {
		if (/*gameState == 1 && */_x >= 30 && _x < 478 && _y >= 30 && _y < 478) {
			int gX = (_x - 30) / 56;
			int gY = (_y - 30) / 56;
			if (subState == 1) {
				cout << "sub: 1" << endl;
				selected = NULL;
				for (vector<Checker>::iterator s = entity.begin(); s != entity.end(); ++s)
					if (s->checkIntersection(gX, gY) && s->getTeam() == gameState) {
						selected = &*s;
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
				if (type == CheckerType::Default) {
					if (x_diff > 2) return;
					vector<Checker>::iterator toRemove;
					int cx = x + copysign(1, x_diff), cy = y + copysign(1, y_diff);
					cout << cx << " " << cy << endl;
					bool invalid = false, remove = false;;
					for (vector<Checker>::iterator s = entity.begin(); s != entity.end(); ++s) {
						if (s->checkIntersection(cx,cy)) {
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
						selected->Move(gX, gY);
						selected->unsetMark();
						if (remove == true) {
							teams[3 - gameState]--;
							entity.erase(toRemove);
						}
						gameState = 3 - gameState;
						subState = 1;
						if (teams[gameState] == 0) {
							EndGame();
						}
					}
				}/* else if (type == CheckerType::King) {
					return !checkIntersection(_x, _y) && x == y && x == 1;
				}*/

			}
		}
	}

	void Draw(RenderWindow *wnd) {
		wnd->draw(boardShape);
		for (vector<Checker>::iterator ci = entity.begin(); ci != entity.end(); ++ci)
			wnd->draw(ci->getShape());
	}

};


int main() {
	RenderWindow window(VideoMode(width, height), "Checkers");

	GameBoard gb = GameBoard();
	gb.StartGame();

	while (window.isOpen()) {
		Vector2i pos = Mouse::getPosition(window);
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
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