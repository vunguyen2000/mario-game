#pragma once
#include "GameObject.h"


#define CARD_BBOX_WIDTH  26
#define CARD_BBOX_HEIGHT  26
#define CARD_STATE_MUSHROOM  1
#define CARD_STATE_STAR  2
#define CARD_ANI_MUSHROOM  0
#define CARD_ANI_STAR  1
#define CARD_STATE_HIDE  4

class CCard : public CGameObject
{
public:
	DWORD timeChange = 0;
	CCard() { timeChange = GetTickCount(); SetState(CARD_STATE_STAR); };
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

