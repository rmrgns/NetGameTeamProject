#pragma once
#include "Game.h"
#include "SpaceMath.h"
#include "Sprite.h"
#include "Animation.h"

enum class partshape {
	circle,
	rectangle,
	triangle,
	polygon
};

typedef struct Part {
	shp::vec2f pos;
	shp::angle2f movDir;

	partshape pshape = partshape::circle;
	shp::angle2f angle = shp::angle2f::useNumAngle(0);
	float scale = 100;
	int PASiz = 0;

	int alpha = 255;
};

class SizeEffect : GameObject {
private:
	shp::vec2f flow;
	shp::vec2f Pos;
	shp::vec2f WH;
	float Power = 3;
public:
	SizeEffect() {

	}

	virtual ~SizeEffect() {

	}

	SizeEffect* Init(Sprite* spr, shp::vec2f pos, shp::vec2f wh, float maxtime, float power, int layer);

	virtual void Update(const float& delta) override;

	virtual void Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) override;

	virtual void Render(HDC hdc) override;
};

class NumberEffect : GameObject {
private:
	int Number = 0;
	shp::vec2f flow;
	shp::vec2f Pos;
	shp::vec2f WH;
	float Power = 3;

	int numberset[10] = {};
public:
	NumberEffect() {

	}

	virtual ~NumberEffect() {
	}

	NumberEffect* Init(Sprite* spr, shp::vec2f pos, shp::vec2f wh, float maxtime, float power, int layer);

	void SetNumber(const int& number);

	const int& GetNumber() const;

	virtual void Update(const float& delta) override;

	virtual void Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) override;

	virtual void Render(HDC hdc) override;

	void Clear() {
		SetEnable(false);
	}
};

class SpreadEffect_Keep : GameObject {
	static constexpr int PART_MAX = 100;
	Part* PartArr[PART_MAX] = {};
	int partup = 0;

	shp::vec2f powerFlow = shp::vec2f(0, 1);
	float basicPow = 0.1f;
	float radius = 100;

	shp::vec2f* mainpoly = nullptr;
	HBITMAP PolyStamp = NULL;

	shp::vec2f center;
	float impactPow = 5;
	void (*PartChangeFunc)(SpreadEffect_Keep* ef, Part* p) = nullptr;
public:
	SpreadEffect_Keep();

	virtual ~SpreadEffect_Keep();

	SpreadEffect_Keep* Init(const shp::rect4f& loc, const int& layer);

	virtual void Update(const float& delta) override;

	virtual void Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) override;

	virtual void Render(HDC hdc) override;

	void Impact() {
		if (enable) {
			powerFlow.x = powerFlow.y;
		}
	}
};

class FlowerEf : GameObject {
	shp::vec2f pos;
	int fcount = 2;
	shp::angle2f ang = shp::angle2f::useRadian(0);
	shp::angle2f destAng = shp::angle2f::useRadian(2*shp::PI);
	shp::vec2f flow = shp::vec2f(0, 3);
	float power = 3;

	float radius = 500;
	int polynum = 6;
public:
	FlowerEf();

	virtual ~FlowerEf();

	FlowerEf* Init(const shp::rect4f& loc, Sprite* spr, const int& layer);

	virtual void Update(const float& delta) override;

	virtual void Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) override;

	virtual void Render(HDC hdc) override;
	void RenderSub(HDC hdc, shp::vec2f pos, shp::angle2f angle, float R, int stack, float rate);
};