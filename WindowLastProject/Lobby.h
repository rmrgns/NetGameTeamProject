#pragma once
#include "Game.h"

class Lobby : GameObject
{
private:
	unsigned int lobbyNum = 0;

public:

	Lobby* Init(const shp::rect4f& loc, const int& layer);

	void SendEnterPlayStation();
};

