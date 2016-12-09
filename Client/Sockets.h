#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
using namespace sf;
using namespace std;

#pragma once

class GameBoard;

enum MessageType {
	Move,
	Remove,
	Clear,
	StartGame,
	EndGame,
	ChangeTeam,
	ChangeState
};

class Sockets {
private:
	unsigned short PORT;
	string IPADDRESS;
	TcpSocket socket;
	Mutex globalMutex;
	Thread* thread = 0;
	GameBoard* gb;
	bool connected = false;
public:
	Sockets(string ip, int port);
	Sockets();

	void Connect();
	void SetGameBoard(GameBoard *_gb);

	void sendMessage(MessageType type, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0);

	void completeConnection(void);
};