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

void Sockets::sendMessage(MessageType type, int param1, int param2, int param3, int param4) {
	if (!connected) return;
	char msg[5];
	msg[0] = type;
	msg[1] = param1;
	msg[2] = param2;
	msg[3] = param3;
	msg[4] = param4;
	if (socket.send(msg, 5) != Socket::Done) {
		cout << "error sending msg" << endl;
	}
}
void Sockets::Connect() {
	if (socket.connect(IPADDRESS, PORT) != Socket::Done) {
		cout << "Couldn't connect to server!" << endl;
		return;
	}
	connected = true;
	thread->launch();
}

void Sockets::SetGameBoard(GameBoard *_gb) {
	gb = _gb;
}

void Sockets::completeConnection(void) {
	static string oldMsg = "";
	while (true) {
		char data[10];
		size_t received;
		if (socket.receive(data, 5, received) == Socket::Done) {
			if(data[0] == MessageType::Clear) {
				gb->Clear();
			} else if (data[0] == MessageType::StartGame) {
				gb->StartGame();
			} else if (data[0] == MessageType::Move) {
				gb->MoveByXY(data[1],data[2],data[3],data[4]);
			} else if (data[0] == MessageType::Remove) {
				gb->RemoveByXY(data[1], data[2]);
			} else if (data[0] == MessageType::ChangeTeam) {
				gb->SetMyTeam((CheckerTeam)data[1]);
			} else if (data[0] == MessageType::ChangeState) {
				gb->ChangeState(data[1]);
			}
			cout << (int)data[0] << endl;
		}
	}
}