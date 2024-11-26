#include "Page.h"
#include "Network.h"

extern vector<Sprite*> SpriteData;
extern RECT rt;
//LogoPage::LogoPage()
//{
//	SetSprite(SpriteData[15]);
//}
//
//LogoPage::~LogoPage()
//{
//}
//
//LogoPage* LogoPage::Init(const shp::rect4f& loc, const int& layer)
//{
//	SetLocation(loc);
//	SetLayer(layer);
//	return this;
//}
//
//void LogoPage::FirstInit()
//{
//	if (enable) {
//		if (checkInitialize && gm != nullptr) {
//			GameManager* GM = (GameManager*)gm;
//			GM->AddObject((GameObject*)HeapDebugClass::HeapNew<FlowerEf>()->Init(GetLocation(), SpriteData[16], 2));
//
//			//int n = Music::AddSound("Sound\\LogoMusic.mp3", false, false);
//			//Music::ConnectSound(0, n);
//			//Music::PlayOnce(0);
//
//			checkInitialize = false;
//		}
//	}
//}
//
//void LogoPage::Update(const float& delta)
//{
//	if (enable) {
//		FirstInit();
//
//		//if (flow.x > flow.y && wait) {
//		if (wait) {
//			//다음 페이지 넘어가기
//			Music::ClearSoundsAndChannels();
//			NextPage();
//			SetEnable(false);
//		}
//
//		/*if (flow.x > flow.y) {
//			wait = true;
//			flow.x = 0;
//		}
//		else {
//			flow.x += delta;
//		}*/
//	}
//}
//
//void LogoPage::Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
//{
//}
//
//void LogoPage::Render(HDC hdc)
//{
//	if (enable) {
//		Sprite* spr = GetSprite();
//		if (spr != nullptr) {
//			if (wait) {
//				float w = 200;
//				shp::vec2f cen = GetLocation().getCenter();
//				spr->DrawSpriteAlphaBlend(hdc, cen.x - w, cen.y - w, 2 * w, 2 * w, 255);
//			}
//			else {
//				float trate = AnimClass::EaseIn(flow.x / flow.y, 4);
//
//
//				if (trate > 1) {
//					trate = 1;
//				}
//				float w = 200;
//				shp::vec2f cen = GetLocation().getCenter();
//				spr->DrawSpriteAlphaBlend(hdc, cen.x - w, cen.y - w, 2 * w, 2 * w, 255 * (trate));
//			}
//		}
//	}
//}
//
//void LogoPage::NextPage()
//{
//	if (enable) {
//		GameManager* GM = (GameManager*)gm;
//		//GM->AddObject((GameObject*)HeapDebugClass::HeapNew<EditStation>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), 1));
//		GM->AddObject((GameObject*)HeapDebugClass::HeapNew<TitlePage>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), 1));
//	}
//}

/// <summary>
/// 
/// </summary>
TitlePage::TitlePage()
{
}

TitlePage::~TitlePage()
{
	if (HeapDebugClass::HeapDebug[GameStartButton] == true) {
		HeapDebugClass::HeapDelete<GameButton>(GameStartButton);
	}

	if (HeapDebugClass::HeapDebug[NextMenu] == true) {
		HeapDebugClass::HeapDelete<GameButton>(NextMenu);
	}

	if (HeapDebugClass::HeapDebug[PrevMenu] == true) {
		HeapDebugClass::HeapDelete<GameButton>(PrevMenu);
	}

	if (HeapDebugClass::HeapDebug[SelectBtn] == true) {
		HeapDebugClass::HeapDelete<GameButton>(SelectBtn);
	}
}

TitlePage* TitlePage::Init(const shp::rect4f& loc, const int& layer)
{
	AddMusicData("momijinosakamichi.ogg", "Momijinosakamichi.txt");
	//AddMusicData("otherOperation5.mp3", "momi.txt");
	//AddMusicData("Raseed Short Ver.mp3", "RaSeed_H.txt");
	//AddMusicData("ChartreuseGreen_H.mp3", "ChartreuseGreen_H.txt");
	//AddMusicData("Hiatus_Departure.mp3", "HiatusDeparture_H.txt");
	SetLocation(loc);
	SetLayer(layer);
	
	return this;
}

void TitlePage::FirstInit()
{
	if (checkInitialize && gm != nullptr) {
		int n = Music::AddSound("Sound\\MelDrum3_Master.mp3", false, false);
		Music::ConnectSound(0, n);
		Music::PlayOnce(0);

		TitleSprite = SpriteData[17];

		GameUI::LBtnPressed = false;

		shp::rect4f loc = GetLocation();
		shp::vec2f cen = loc.getCenter();
		float w = loc.getw() / 8;
		float h = loc.geth() / 8;

		GameStartButton = HeapDebugClass::HeapNew<GameButton>();
		GameStartButton->location = shp::rect4f(cen.x - 2*w, cen.y + w, cen.x + 2 * w, cen.y + 2*w);
		GameStartButton->SetSTR(L"GameStart");
		GameStartButton->OnClick = IFClickGameStart;
		GameStartButton->SetParent((GameObject*)this);

		NextMenu = HeapDebugClass::HeapNew<GameButton>();
		NextMenu->location = shp::rect4f(loc.lx - 2 * w, cen.y + w, loc.lx, cen.y + 2 * w);
		NextMenu->SetSTR(L"Next");
		NextMenu->OnClick = IFClickNext;
		NextMenu->SetParent((GameObject*)this);

		PrevMenu = HeapDebugClass::HeapNew<GameButton>();
		PrevMenu->location = shp::rect4f(loc.fx, cen.y + w, loc.fx + 2 * w, cen.y + 2 * w);
		PrevMenu->SetSTR(L"Prev");
		PrevMenu->OnClick = IFClickPrev;
		PrevMenu->SetParent((GameObject*)this);

		SelectBtn = HeapDebugClass::HeapNew<GameButton>();
		SelectBtn->location = shp::rect4f(cen.x - w, loc.ly - w, cen.x + w, loc.ly);
		SelectBtn->SetSTR(L"Select");
		SelectBtn->OnClick = IFClickSelect;
		SelectBtn->SetParent((GameObject*)this);

		for (int i = 0; i < 6; ++i) {
			if (i % 2 == 0) {
				IconImage[i] = SpriteData[18];
			}
			else {
				IconImage[i] = SpriteData[19];
			}
		}

		checkInitialize = false;
	}
	
}

void TitlePage::Update(const float& delta)
{
	
	


	if (enable) {
		FirstInit();
		
		if (checkGameStart == false) {
			GameStartButton->Update(delta);
		}
		else {
			NextMenu->Update(delta);
			PrevMenu->Update(delta);
			SelectBtn->Update(delta);
		}

		if (checkGameStart) {
			if (TitleAppearFlow.x + delta > TitleAppearFlow.y) {
				TitleAppearFlow.x = TitleAppearFlow.y;
			}
			else {
				TitleAppearFlow.x += delta;
			}

			if (iconChangeFlow.x + delta > iconChangeFlow.y) {
				iconChangeFlow.x = iconChangeFlow.y;
			}
			else {
				iconChangeFlow.x += delta;
			}
		}
	}
}

void TitlePage::Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (enable) {
		if (checkGameStart == false) {
			GameStartButton->Event(hWnd, iMessage, wParam, lParam);
		}
		else {
			NextMenu->Event(hWnd, iMessage, wParam, lParam);
			PrevMenu->Event(hWnd, iMessage, wParam, lParam);
			SelectBtn->Event(hWnd, iMessage, wParam, lParam);

			if (wParam == 'S' || wParam == VK_LEFT) {
				IFClickPrev(PrevMenu, hWnd, iMessage, wParam, lParam);
			}
			if (wParam == 'L' || wParam == VK_RIGHT) {
				IFClickNext(NextMenu, hWnd, iMessage, wParam, lParam);
			}
			if (wParam == VK_SPACE) {
				IFClickSelect(SelectBtn, hWnd, iMessage, wParam, lParam);
			}
			if (wParam >= '1' && wParam <= '9')
			{
				Network::GetInst()->musicIndex = int(wParam) - 49;
				if (Network::GetInst()->musicDataSet.size() <= Network::GetInst()->musicIndex)
					Network::GetInst()->musicIndex = 0;
			}
			if (wParam == 'q')
			{
				string id = "1234";
				cout << id << endl;
				SendCheckLoginAndMusicDownload(id, id);
			}
		}
	}
}

void TitlePage::Render(HDC hdc)
{
	if (enable) {
		shp::rect4f loc = GetLocation();
		float trate = AnimClass::EaseOut(TitleAppearFlow.x / TitleAppearFlow.y, 3);

		float w = loc.getw() / 8;
		float h = loc.geth() / 8;
		shp::rect4f titlelocdest = shp::rect4f(loc.fx, loc.fy, loc.fx + 2*h, loc.fy + 2*h);
		shp::vec2f cen = loc.getCenter();
		shp::rect4f titleloccheckInitialize = shp::rect4f(cen.x - 4*h, cen.y - 4*h, cen.x + 4*h, cen.y + 4*h);

		shp::rect4f rrt = shp::rect4f(
			titleloccheckInitialize.fx + trate*(titlelocdest.fx- titleloccheckInitialize.fx), 
			titleloccheckInitialize.fy + trate * (titlelocdest.fy - titleloccheckInitialize.fy),
			titleloccheckInitialize.lx + trate * (titlelocdest.lx - titleloccheckInitialize.lx),
			titleloccheckInitialize.ly + trate * (titlelocdest.ly - titleloccheckInitialize.ly)
		);
		TitleSprite->DrawSprite(hdc, rrt.fx, rrt.fy, rrt.getw(), rrt.geth());

		if (checkGameStart == false) {
			GameStartButton->RenderObject(hdc);
		}
		else {
			float mrate = AnimClass::EaseIO(iconChangeFlow.x / iconChangeFlow.y, 5);
			if ((0 <= mrate && mrate <= 1) == false) {
				if (mrate < 0) {
					mrate = 0;
				}
				else {
					mrate = 1;
				}
			}

			NextMenu->RenderObject(hdc);
			PrevMenu->RenderObject(hdc);
			SelectBtn->RenderObject(hdc);

			float cenx = loc.getCenter().x + mdir * loc.getw() - mdir * loc.getw() * mrate;
			IconImage[iconNum]->DrawSprite(hdc, cenx - 2 * w, cen.y - 2 * w, 4 * w, 4 * w);

			float subcenx = loc.getCenter().x - mdir * loc.getw() * mrate;
			int prevnum = 0;
			if (mdir > 0) {
				prevnum = iconNum;
				if (prevnum - 1 >= 0) {
					prevnum -= 1;
				}
				else {
					prevnum = 5;
				}
			}
			else {
				prevnum = iconNum;
				if (iconNum + 1 < 6) {
					iconNum += 1;
				}
				else {
					iconNum = 0;
				}
			}
			IconImage[prevnum]->DrawSprite(hdc, subcenx - 2 * w, cen.y - 2 * w, 4 * w, 4 * w);
		}
	}
}

void TitlePage::Select(char menuChar)
{
	if (enable) {
		selectCommand = menuChar;
		Music::ClearSoundsAndChannels();
		NextPage();
		SetEnable(false);
	}
}

void TitlePage::NextPage()
{
	if (enable) {
		switch(selectCommand) {
			case 'p':
			{
				GameManager* GM = (GameManager*)gm;
				//GM->Clear();
				PlayStation* ps = HeapDebugClass::HeapNew<PlayStation>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), false, 1);
				ps->LoadMusic(Network::GetInst()->musicDataSet[Network::GetInst()->musicIndex].musicName.c_str());
				ps->LoadData(Network::GetInst()->musicDataSet[Network::GetInst()->musicIndex].noteName.c_str());

				GM->AddObject((GameObject*)ps);
					
		}
				break;
			case 'e':
			{
				GameManager* GM = (GameManager*)gm;
				GM->AddObject((GameObject*)HeapDebugClass::HeapNew<EditStation>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), 1));
			}
				break;
		}
	}
}

void TitlePage::MoveMenu(int dir)
{
	if (enable) {
		mdir = dir;
		iconChangeFlow.x = 0;

		if (dir > 0) {
			if (iconNum + 1 < 6) {
				iconNum += 1;
			}
			else {
				iconNum = 0;
			}
		}
		else {
			if (iconNum - 1 >= 0) {
				iconNum -= 1;
			}
			else {
				iconNum = 5;
			}
		}
	}
}

void TitlePage::GameStart()
{
	if (enable) {
		checkGameStart = true;
	}
}

const int& TitlePage::GetIconNum()
{
	if (enable) {
		return iconNum;
	}
}

void TitlePage::SendCheckLoginAndMusicDownload(string id, string password)
{
	Network::GetInst()->SendCheckLoginAndMusicDownload(id, password);
}

void TitlePage::SendEnterPlayStation()
{
	Network::GetInst()->SendEnterPlayStation(this);
}

void TitlePage::AddMusicData(const string& musicName, const string& dataName)
{
	string music = "Sound/" + musicName;
	string data = "NoteData/" + dataName;
	Network::GetInst()->musicDataSet.push_back({ music, data });
}

void TitlePage::PrintMusicData() const
{
}

void IFClickGameStart(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam)
{
	TitlePage* tp = (TitlePage*)obj->Parent;
	tp->GameStart();
}

void IFClickNext(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam)
{
	GameUI::LBtnPressed = false;
	TitlePage* tp = (TitlePage*)obj->Parent;
	tp->MoveMenu(1);
}

void IFClickPrev(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam)
{
	GameUI::LBtnPressed = false;
	TitlePage* tp = (TitlePage*)obj->Parent;
	tp->MoveMenu(-1);
}

void IFClickSelect(const GameButton* obj, const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam)
{
	GameUI::LBtnPressed = false;
	TitlePage* tp = (TitlePage*)obj->Parent;
	if (tp->GetIconNum() % 2 == 0) {
		tp->SendEnterPlayStation();
		//tp->Select('p');
	}
	else {
		tp->Select('e');
	}
	
}
