#pragma once

//#include "Common.h"



struct LoginAndMusicDownloadPacket
{
	string song;
	string note;
};

struct PlayerScorePacket
{
	unsigned int index = 0;
	unsigned int score = 0;
};

