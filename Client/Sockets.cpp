#include <iostream>
#include <SFML/Network.hpp>
#include "Sockets.h"
#include "GameBoard.h"

using namespace sf;
using namespace std;

#pragma once

Sockets::Sockets(string ip, int port) {
	IPADDRESS = ip;
	PORT = port;
	thread = new Thread([this] {completeConnection(); });
}

Sockets::Sockets() {}

void Sockets::sendMessage(char *msg, int len) {
	if (socket.send(msg, len) != Socket::Done) {
		cout << "error sending msg" << endl;
	}
}
void Sockets::Connect() {
	if (socket.connect(IPADDRESS, PORT) != Socket::Done) {
		cout << "Couldn't connect to server!" << endl;
		return;
	}
	thread->launch();
}

void Sockets::SetGameBoard(GameBoard *_gb) {
	gb = _gb;
}

void Sockets::completeConnection(void) {
	static string oldMsg = "";
	while (true) {
		char data[100];
		size_t received;
		if (socket.receive(data, 100, received) == Socket::Done) {
			data[received] = 0;
			if (!strcmp(data, "clear")) {
				gb->Clear();
			} else if (!strcmp(data, "newgame")) {
				gb->StartGame();
			}
			cout << data << endl;
		}
	}
}