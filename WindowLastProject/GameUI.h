#pragma once
#include <Windows.h>
#include "SpaceMath.h"
#include "HeapDebug.h"
#include "Game.h"

typedef enum class UIType {
	button = 1,
	none = 100
};

class GameUI {
protected:
	UIType uiType;
	bool enable;
public:

	static constexpr int GAMEUI_MAX = 100;
	static GameUI* UIArr[GAMEUI_MAX];
	static int GameUIUpdate;

	static shp::vec2f MousePos;
	static bool LBtnPressed;
	static bool RBtnPressed;

	GameUI() {
		uiType = UIType::none;
		enable = true;
	}

	virtual ~GameUI() {

	}

	void SetEnable(const bool& b);
	const bool& GetEnable() const;

	virtual void RenderObject(const HDC& hdc) const {
	}

	virtual void Event(const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam) {
	}

	virtual void Update(const float& delta) {
	}
};

class GameButton : GameUI{
public:
	shp::rect4f location;
	bool bePressed;
	float flow = 0;
	TCHAR str[128];

	void (*RenderFunc)(const GameButton* obj, const HDC&) = nullptr;
	void (*OnClick)(const GameButton*, const HWND&, const UINT&, const WPARAM&, const LPARAM&);

	GameObject* Parent = nullptr;

	GameButton();

	virtual ~GameButton();

	void SetSTR(const TCHAR* tstr) {
		if (enable) {

			if (wcslen(tstr) > 0) {
				wcscpy_s(str, tstr);
			}

			void* ptr = (void*)tstr;
			if (HeapDebugClass::HeapDebug[ptr] == true) {
				HeapDebugClass::HeapArrDelete(tstr);
			}
		}
	}

	virtual void RenderObject(const HDC& hdc) const {
		if (enable) {
			if (RenderFunc == nullptr) {
				Rectangle(hdc, location.fx, location.fy, location.lx, location.ly);
				RECT lrt = { location.fx, location.fy, location.lx, location.ly };
				SetTextColor(hdc, RGB(0, 0, 0));
				DrawText(hdc, str, lstrlen(str), &lrt, DT_CENTER);
			}
			else {
				RenderFunc(this, hdc);
			}
		}
	}

	virtual void Event(const HWND& hWnd, const UINT& iMessage, const WPARAM& wParam, const LPARAM& lParam) {
		if (enable) {
			if (OnClick == nullptr) {

			}
			else {
				if (LBtnPressed && shp::bPointInRectRange(MousePos, location)) {
					OnClick(this, hWnd, iMessage, wParam, lParam);
				}
			}
		}
	}

	virtual void Update(const float& delta) {
		if (enable) {
			flow += delta;
		}
	}

	void SetParent(GameObject* parent) {
		if (enable) {
			Parent = parent;
		}
	}
};