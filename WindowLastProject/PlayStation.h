#pragma once
#include <Windows.h>
#include <fstream>

#include "Game.h"
#include "Animation.h"
#include "HeapDebug.h"
#include "SpaceMath.h"
#include "Effect.h"
#include "Sprite.h"
#include "Music.h"

using namespace std;

typedef enum class NOTE_TYPE {
	None = 0,
	NormalNote = 1,
	LongNote = 2,
	SpaceLeftNote = 3,
	SpaceNote = 4,
	SpaceRightNote = 5
};

typedef enum class ROTPOS {
	bottom = 0,
	left = 1,
	top = 2,
	right = 3
};

typedef struct Note {
	float time = 0;
	float endtime = 0; // �� ��Ʈ�� ���..
	ROTPOS rotPos; // ȸ����ġ
	int pos; // ��ġ
	NOTE_TYPE ntype;

	//������� �ʰ� �ǽð����θ� �����ϴ� ����
	bool enable = true; // ���� ��Ʈ�� falseó����.
};

typedef struct KeyData {
	bool S_pressed = false;
	shp::vec2f S_dut = shp::vec2f(0, 0);
	bool SHit = false;

	bool D_pressed = false;
	shp::vec2f D_dut = shp::vec2f(0, 0);
	bool DHit = false;

	bool K_pressed = false;
	shp::vec2f K_dut = shp::vec2f(0, 0);
	bool KHit = false;

	bool L_pressed = false;
	shp::vec2f L_dut = shp::vec2f(0, 0);
	bool LHit = false;

	bool Space_pressed = false;
	shp::vec2f Space_dut = shp::vec2f(0, 0);
	bool SpaceHit = false;
};

class PlayStation : GameObject{
private:
	bool isplaying = true; // false�� ���� �� �ְ�
	bool autoplaying = false; // true�� input�� ��� �ڵ����� ����

	float Time;
	unsigned int Score; // 1000000
	unsigned int Combo;
	unsigned int MaxCombo;

	int Tempo;
	ROTPOS rotation;

	Note* noteArr = nullptr;
	int noteMAX = 0;
	int nextNoteNum = 0; // �ð��� �������� �� �ð��� �´� ��Ʈ�� ����Ű���� ��.

	//Show* ShowArr = nullptr;
	//int showMAX = 0;
	//int nextShowNum = 0;

	Sprite* BackGround;
	float Impact = 0;
	float AddImpact = 0.2f;

	int songSoundID = -1;

	int RimSoundID = 0;

	//������ ������--------------------------------------------
	int addScore = 1000000000;
	int perfectnum = 0;
	int goodnum = 0;
	int missnum = 0;

	float accelPower = 3;
	float noteWidthRate = 0.7f;
	float NoteVisibleLength = 1; // ���� ���� ��Ʈ���� ������ ����.
	static constexpr int readFutureNum = 30; // �̷� ��� ������
	int notelocArr[5][100] = { {} }; // (1, 2, 3, 4, space)�ټ����� ��ġ���� ���� �Ǵ��ϱ� ����
	KeyData keydata;
	float perfectCloseValue = 0.05f; // �󸶳� ��Ȯ�ϰ� ������ �� ���ΰ�? (��)
	float goodCloseValue = 0.1f; // �󸶳� ��Ȯ�ϰ� ������ �� ���ΰ�? (��)
	float missCloseValue = 0.2f; // �󸶳� ��Ȯ�ϰ� ������ �� ���ΰ�? (��)

	//ǥ�õǴ� good / perfect / miss
	char laststate = 'p'; // p : perfect / g : good / m : miss / n : none
	shp::vec2f stateFlow = shp::vec2f(0, 1);
	float statePow = 4;

	NumberEffect* NE;
	NumberEffect* ComboNE;

	//ȸ��flow
	static constexpr float RotatePower = 5;
	shp::vec2f RotateFlow = shp::vec2f(0, 0.5f);
	bool rotating = false;
	bool rotateLeft = false;

	//�ճ�Ʈ�� ��� ó���� ���ΰ�?
	bool isLongState[5] = {};

	//���� �� �ִ� ��Ʈ�� ǥ��
	shp::vec2f PushNote[4][2]; // 0 : ���� ��ġ, 1 : ���� ��ġ
	float pnW = 20;
	
	bool first = true;
public:
	// variable access
	PlayStation();

	virtual ~PlayStation();

	PlayStation* Init(const shp::rect4f& loc, const bool& autoplay, const int& layer);
	void FirstInit();

	void SetIsPlaying(const bool& bplaying);
	const bool& GetIsPlaying() const;

	void SetAutoMod(const bool& mod);
	const bool& GetAutoMod() const;

	void SetTime(const float& t);
	const float& GetTime() const;

	void SetScore(const int& score);
	const int& GetScore() const;

	void SetCombo(const int& com);
	const int& GetCombo() const;

	void SetMaxCombo(const int& maxcom);
	const int& GetMaxCombo() const;

	void SetTempo(const int& tempo);
	const int& GetTempo() const;

	void SetRotPos(const ROTPOS& rot);
	const ROTPOS& GetRotPos() const;

	void SetNextNoteNum(const int& num);
	const int& GetNextNoteNum() const;

	void SetMaxNoteNum(const int& num);
	const int& GetMaxNoteNum() const;

	void SetNote(const int& index, const Note& note);
	const Note& GetNote(const int& index) const;

	shp::rect4f GetPlayLoc() {
		shp::rect4f wloc = GetLocation();

		shp::vec2f center = wloc.getCenter();
		float w = 0.6f * wloc.geth()/2;
		shp::rect4f playloc = shp::rect4f(center.x - w, center.y - w,
			center.x + w, center.y + w);

		return playloc;
	}

	const shp::vec2f GetSPos(ROTPOS rot, int pos, const shp::rect4f& playloc);
	const shp::vec2f GetEPos(ROTPOS rot, int pos, const shp::rect4f& playloc);

	void Rotate(bool left);
	ROTPOS GetNextROTPOS(ROTPOS rot, bool left);

	void Rim();

	void Hit(char pan, Note note);

	void GoPosition(float position);

	void LoadMusic(const char* musicName);
	//save/load

	void LoadData(const char* filename);
	void SaveData(const char* filename);
	void LoadNotes(vector<Note> narr);

	//arrange data
	void Arrange();

	virtual void Update(const float& delta) override;

	virtual void Event(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) override;

	void DrawNote(HDC hdc, shp::rect4f playloc, Note note);
	virtual void Render(HDC hdc) override;

	void SendRequestPlayerScore();
};