#pragma once
#include "GameObject.h"


#define CARD_BBOX_WIDTH  26
#define CARD_BBOX_HEIGHT  26

class CCard : public CGameObject
{
public:
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

