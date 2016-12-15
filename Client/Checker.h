#include <SFML/Graphics.hpp>
using namespace sf;

#pragma once

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
	Sprite shape;
public:
	bool created;
	Checker();

	Checker(int _x, int _y, CheckerTeam _team, CheckerType _type, Texture *texture);

	CheckerTeam getTeam();

	CheckerType getType();

	void setMark();

	void unsetMark();

	void Move(int _x, int _y);

	bool checkIntersection(int _x, int _y);

	void getPosition(int *_x, int *_y);

	Sprite getShape();

	bool operator==(Checker other) const;
};