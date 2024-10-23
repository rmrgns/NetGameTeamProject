#include "GameUI.h"

void GameUI::SetEnable(const bool& b) {
	enable = b;
}

const bool& GameUI::GetEnable() const
{
	return enable;
}

GameButton::GameButton()
{
	if (GameUIUpdate+1 < GAMEUI_MAX) {
		UIArr[GameUIUpdate] = this;
		GameUIUpdate += 1;
	}
	else {
		for (int i = 0; i < GAMEUI_MAX; ++i) {
			if (UIArr[i] == nullptr) {
				UIArr[i] = this;
				break;
			}
		}
	}
}

GameButton::~GameButton()
{
}
