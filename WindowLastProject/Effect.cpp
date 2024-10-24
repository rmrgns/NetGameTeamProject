#include "Effect.h"

SizeEffect* SizeEffect::Init(Sprite* spr, shp::vec2f pos, shp::vec2f wh, float maxtime, float power, int layer)
{
	Pos = pos;
	WH = wh;
	flow = shp::vec2f(0, maxtime);
	Power = power;
	SetSprite(spr);
	SetLayer(layer);

	return this;
}

void SizeEffect::Update(const float& delta)
{
	if (enable) {
		flow.x += delta;
		if (flow.x > flow.y) {
			SetEnable(false);
		}
		else {
			float rate = AnimClass::EaseOut(flow.x / flow.y, Power);
			shp::rect4f loc = shp::rect4f(Pos.x - WH.x * rate, Pos.y - WH.y * rate,
				Pos.x + WH.x * rate, Pos.y + WH.y * rate);
			SetLocation(loc);
		}
	}
}

void SizeEffect::Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (enable) {

	}
}

void SizeEffect::Render(HDC hdc)
{
	if (enable) {
		Sprite* spr = GetSprite();
		shp::rect4f loc = GetLocation();
		if (spr != nullptr && flow.y != 0) {
			float rate = AnimClass::EaseOut(flow.x / flow.y, Power);
			float arate = AnimClass::EaseOut(flow.x / flow.y, 3);
			spr->DrawSpriteAlphaBlend(hdc, loc.fx, loc.fy, loc.getw(), loc.geth(), 255 * (1.0f - arate));
		}
	}
}

NumberEffect* NumberEffect::Init(Sprite* spr, shp::vec2f pos, shp::vec2f wh, float maxtime, float power, int layer)
{
	SetSprite(spr);
	Pos = pos;
	WH = wh;
	flow = shp::vec2f(0, maxtime);
	Power = power;
	SetLayer(layer);
	return this;
}

void NumberEffect::SetNumber(const int& number)
{
	if (enable) {
		Number = number;
		int num = Number;
		for (int i = 9; i >= 0; --i) {
			int n = num / pow(10, i);
			numberset[i] = n;
			if (n == 0) {
				
			}
			else {
				num -= n * pow(10, i);
			}
		}
		flow.x = 0;
	}
}

const int& NumberEffect::GetNumber() const
{
	if (enable) {
		return Number;
	}
	else {
		return 0;
	}
}

void NumberEffect::Update(const float& delta)
{
	if (enable) {
		flow.x += delta;
		if (flow.x > flow.y) {
			flow.x = flow.y;
		}
		else {
			float rate = 1;
			shp::rect4f loc = shp::rect4f(Pos.x - WH.x * rate, Pos.y - WH.y * rate,
				Pos.x + WH.x * rate, Pos.y + WH.y * rate);
			SetLocation(loc);
		}
	}
}

void NumberEffect::Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (enable) {

	}
}

void NumberEffect::Render(HDC hdc)
{
	if (enable) {
		Sprite* spr = GetSprite();
		shp::rect4f loc = GetLocation();
		if (spr != nullptr && flow.y != 0) {
			float rate = AnimClass::EaseOut(flow.x / flow.y, Power);
			float lw = loc.getw();
			float w = lw / 10;
			for (int i = 9; i >= 0; --i) {
				int frame = numberset[i] - 1;
				if (frame < 0) {
					frame = 9;
				}

				spr->DrawFrameAlphaBlend(hdc, frame, loc.lx - w*(i+1), loc.fy, w , loc.geth(), 127 + 128 * (1.0f - rate));
			}
		}
	}
}

SpreadEffect_Keep::SpreadEffect_Keep()
{
}

SpreadEffect_Keep::~SpreadEffect_Keep()
{
}

SpreadEffect_Keep* SpreadEffect_Keep::Init(const shp::rect4f& loc, const int& layer)
{
	SetLocation(loc);
	int m = (loc.getw() > loc.geth()) ? loc.getw() : loc.geth();
	m /= 2;
	radius = m;
	center = loc.getCenter();
	SetLayer(layer);
	return this;
}

void SpreadEffect_Keep::Update(const float& delta)
{
	constexpr float sizrate = 0.2f;

	if (enable) {
		powerFlow.x -= delta;
		if (powerFlow.x < 0) {
			powerFlow.x = 0;
		}

		if (PartChangeFunc == nullptr) {
			for (int i = 0; i < partup; ++i) {
				Part* p = PartArr[i];
				p->pos.x += p->movDir.dx * AnimClass::EaseIn(powerFlow.x / powerFlow.y, impactPow);
				p->scale = sizrate * shp::get_distance(p->pos, center);
			}
		}
		else {
			for (int i = 0; i < partup; ++i) {
				Part* p = PartArr[i];
				PartChangeFunc(this, p);
			}
		}
	}
}

void SpreadEffect_Keep::Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (enable) {

	}
}

void SpreadEffect_Keep::Render(HDC hdc)
{
	if (enable) {
		if (mainpoly == nullptr) {
			if (PolyStamp != NULL) {
				SIZE sz;
				BOOL res = ::GetBitmapDimensionEx(PolyStamp, &sz);
				HDC bitdc = CreateCompatibleDC(hdc);
				HBITMAP OldBit = (HBITMAP)SelectObject(bitdc, PolyStamp);

				for (int i = 0; i < partup; ++i) {
					Part* p = PartArr[i];
					shp::rect4f siz = shp::rect4f(p->pos.x - p->scale, p->pos.y - p->scale,
						p->pos.x + p->scale, p->pos.y + p->scale);
					Sprite::DrawBITMAPAlphaBlend(hdc, siz.fx, siz.fy, siz.getw(), siz.geth(), p->alpha, bitdc, sz.cx, sz.cy);
				}
				
				SelectObject(bitdc, OldBit);
				DeleteDC(bitdc);
			}
		}
	}
}

FlowerEf::FlowerEf()
{
}

FlowerEf::~FlowerEf()
{
}

FlowerEf* FlowerEf::Init(const shp::rect4f& loc, Sprite* spr, const int& layer)
{
	pos = loc.getCenter();
	SetSprite(spr);
	SetLocation(loc);
	SetLayer(layer);
	return this;
}

void FlowerEf::Update(const float& delta)
{
	if (enable) {
		flow.x += delta;
		if (flow.x > flow.y) {
			SetEnable(false);
		}
	}
}

void FlowerEf::Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (enable) {

	}
}

void FlowerEf::Render(HDC hdc)
{
	if (enable) {
		float trate = AnimClass::EaseIO(flow.x / flow.y, power);
		ang = shp::angle2f::useRadian(destAng.radian * trate);
		Sprite* spr = GetSprite();
		if (spr != nullptr) {
			float r = radius / 8 + (radius/2) * (1.0f-trate);
			float yr = r * 1.12f;
			spr->DrawTransfromFrame(hdc, 0, pos.x - r/2, pos.y - yr/2, r, yr, ang.radian, pos);

			RenderSub(hdc, pos, ang, radius, fcount, trate);
		}
	}
}

void FlowerEf::RenderSub(HDC hdc, shp::vec2f pos, shp::angle2f angle, float R, int stack, float rate)
{
	if (stack == 0) {
		return;
	}
	else {
		stack -= 1;
	}
	shp::vec2f subpos;
	shp::angle2f subangle;
	float r = R / 3;
	float sr = r * (1.0f - rate);
	float syr = r * 1.12f;
	for (int i = 0; i < polynum; ++i) {
		if (stack % 2 == 0) {
			subangle = shp::angle2f::useRadian(ang.radian - 2 * i * shp::PI / polynum);
		}
		else {
			subangle = shp::angle2f::useRadian(ang.radian + 2 * i * shp::PI / polynum);
		}
		
		subpos = pos + shp::vec2f(15*sr*rate * subangle.dx, 15*sr*rate * subangle.dy);

		spr->DrawTransfromFrame(hdc, 0, subpos.x-sr/2, subpos.y- syr /2, sr, syr, subangle.radian, subpos);

		RenderSub(hdc, subpos, subangle, sr, stack, rate);
	}
}
