#pragma once

#include "GameObject.h"
#include "Animations.h"

#define COIN_BBOX_WIDTH  10
#define COIN_BBOX_HEIGHT 15


#define COIN_STATE_IDLE		0
#define COIN_STATE_UP		1
#define COIN_STATE_HIDE		2


#define COIN			100

class CCoin : public CGameObject {
	int status;
	bool hide = false;
	bool show;
public:
	CCoin(int status);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();
	virtual void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};