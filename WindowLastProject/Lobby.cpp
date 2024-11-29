#include "Lobby.h"

Lobby* Lobby::Init(const shp::rect4f& loc, const int& layer)
{
	SetLocation(loc);
	SetLayer(layer);

	return this;
}

void Lobby::SendEnterPlayStation()
{

}
