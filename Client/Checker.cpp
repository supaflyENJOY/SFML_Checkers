#include <SFML/Graphics.hpp>
#include "Checker.h"
using namespace sf;

Checker::Checker() {
	created = false;
}

Checker::Checker(int _x, int _y, CheckerTeam _team, CheckerType _type, Texture *texture) {
	shape.setTexture(*texture);
	shape.setScale(0.52, 0.52);
	shape.setPosition(30, 30);
	x = _x;
	y = _y;
	team = _team;
	type = _type;
	shape.setPosition(30 + 56 * x, 30 + 56 * y);
	unsetMark();
	created = true;
}

CheckerTeam Checker::getTeam() {
	return team;
}

CheckerType Checker::getType() {
	return type;
}

void Checker::setMark() {
	shape.setColor(team == CheckerTeam::White ? Color::Color(210, 210, 210) : Color::Color(70, 70, 70));
}

void Checker::unsetMark() {
	shape.setColor(team == CheckerTeam::White ? Color::White : Color(40, 40, 40));
}

void Checker::Move(int _x, int _y) {
	x = _x;
	y = _y;
	if ((y == 7 && team == CheckerTeam::White) || (y == 0 && team == CheckerTeam::Black)) {
		type = CheckerType::King;
	}
	shape.setPosition(30 + 56 * x, 30 + 56 * y);
}

bool Checker::checkIntersection(int _x, int _y) {
	if (x == _x && y == _y) {
		return true;
	}
	return false;
}

void Checker::getPosition(int *_x, int *_y) {
	*_x = x;
	*_y = y;
}

Sprite Checker::getShape() {
	return shape;
}

bool Checker::operator==(Checker other) const {
	return other.checkIntersection(x, y);
}