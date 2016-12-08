#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
using namespace sf;
using namespace std;

#pragma once

class GameBoard;

class Sockets {
private:
	unsigned short PORT;
	string IPADDRESS;
	TcpSocket socket;
	Mutex globalMutex;
	Thread* thread = 0;
	GameBoard* gb;
public:
	Sockets(string ip, int port);
	Sockets();

	void Connect();
	void SetGameBoard(GameBoard *_gb);

	void sendMessage(char *msg, int len);

	void completeConnection(void);
};