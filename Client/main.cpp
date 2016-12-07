#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>  

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
	Queen
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
	list<Checker> entity;
	int gameState;
	int subState;
	int teams[2];
	list<Checker>::iterator selected;
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

	void StartGame() {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 4; j++) {
				entity.push_back(Checker(j * 2 + !(i % 2), i, CheckerTeam::White, CheckerType::Default));
				entity.push_back(Checker(j * 2 + (i % 2), 7 - i, CheckerTeam::Black, CheckerType::Default));
			}
		}
		teams[0] = 8;
		teams[1] = 8;
		gameState = CheckerTeam::White;
		subState = 1;
		cout << "Game started!" << endl;
	}

	void ProceedMouse(int _x, int _y) {
		cout << "ProceedMouse" << endl;
		if (/*gameState == 1 && */_x >= 30 && _x < 478 && _y >= 30 && _y < 478) {
			int gX = (_x - 30) / 56;
			int gY = (_y - 30) / 56;
			cout << "state:1 " << endl;
			if (subState == 1) {
				cout << "sub:1 " << endl;
				selected = entity.end();

				for (list<Checker>::iterator s = entity.begin(); s != entity.end(); ++s)
					if (s->checkIntersection(gX, gY) && s->getTeam() == gameState) {
						selected = s;
						break;
					}
				if (selected != entity.end()) {
					subState = 2;
					cout << "Found checker! " << endl;
				}
			} else if (subState == 2) {
				cout << "sub:2 " << endl;
				bool toRemove = false;
				bool invalid = false;
				for (list<Checker>::iterator s = entity.begin(); s != entity.end(); ++s) {
					if (s->checkIntersection(gX, gY)) {
						if (s->getTeam() != selected->getTeam()) {
							toRemove = true;
							entity.erase(s);
						} else {
							invalid = true;
						}
						break;
					}
				}
				if (invalid) return;
				if (toRemove == true) {
					teams[3 - gameState]--;
				}
				cout << "toRemove:" << toRemove << endl;
				cout << "Move:" << gX << " " << gY << endl;
				cout << "Move:" << selected->getTeam() << endl;
				selected->Move(gX, gY);
				gameState = 3 - gameState;
				subState = 1;
			}
		}
	}

	void Draw(RenderWindow *wnd) {
		wnd->draw(boardShape);
		for (list<Checker>::iterator ci = entity.begin(); ci != entity.end(); ++ci)
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
					gb.ProceedMouse(pos.x, pos.y);
				}
			}
		}

		window.clear();
		gb.Draw(&window);
		window.display();
	}

	return 0;
}