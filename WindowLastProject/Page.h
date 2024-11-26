#pragma once
#include "Game.h"
#include "GameUI.h"
#include "Effect.h"
#include "EditStation.h"

#include <vector>
#include <string>

typedef struct PageMusicData {
	string musicName;
	string noteName;
};


//class LogoPage : GameObject{
//	bool checkInitialize = true;
//	bool wait = false;
//	//shp::vec2f flow = shp::vec2f(0, 3);
//public:
//	LogoPage();
//
//	virtual ~LogoPage();
//
//	LogoPage* Init(const shp::rect4f& loc, const int& layer);
//	void FirstInit();
//
//	virtual void Update(const float& delta) override;
//
//	virtual void Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) override;
//
//	virtual void Render(HDC hdc) override;
//
//	void NextPage();
//};

class TitlePage : GameObject {
	bool checkInitialize = true;
	//bool wait = false;
	//shp::vec2f flow = shp::vec2f(0, 3);

	Sprite* TitleSprite = nullptr;
	shp::vec2f TitleAppearFlow = shp::vec2f(0, 1);

	GameButton* GameStartButton = nullptr;

	GameButton* NextMenu = nullptr;
	GameButton* PrevMenu = nullptr;
	GameButton* SelectBtn = nullptr;

	Sprite* IconImage[6] = {};
	shp::vec2f iconChangeFlow = shp::vec2f(0, 0.5f);
	int mdir = 1;
	int iconNum = 0;

	bool checkGameStart = false;
	char selectCommand = 'p';

	
public:
	TitlePage();

	virtual ~TitlePage();

	TitlePage* Init(const shp::rect4f& loc, const int& layer);
	void FirstInit();

	virtual void Update(const float& delta) override;

	virtual void Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) override;

	virtual void Render(HDC hdc) override;

	void Select(char menuChar);

	void NextPage();

	void MoveMenu(int dir);

	void GameStart();

	const int& GetIconNum();

	// Network�� CheckLoginAndMusicDownload �Լ��� �����ϴ� �Լ�
	void SendCheckLoginAndMusicDownload(string id, string password);

	// Network�� SendEnterPlayStation �Լ��� �����ϴ� �Լ�
	void SendEnterPlayStation();
	void setSelectCommand(char cmd) { selectCommand = cmd; }

	// PageMusicDataSet �޼���
	void AddMusicData(const string& musicName, const string& dataName); // �� �߰� �Լ� ����
	void PrintMusicData() const; // ����� ���� ����ϴ� �Լ� ����

	
};

void IFClickGameStart(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);

void IFClickNext(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);

void IFClickPrev(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);

void IFClickSelect(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam);
