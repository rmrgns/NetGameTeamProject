#pragma once
#include "Game.h"

class Lobby : GameObject
{
private:
	unsigned int lobbyNum = 0;

public:
	void SendEnterPlayStation();
};

