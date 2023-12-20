#pragma once

#include "GameObject.h"
#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

#define BRICK_BROKEN 111

#define BRICK_BROKEN_STATE_HIDE 1
#define BRICK_BROKEN_STATE_COIN 3
#define BRICK_BROKEN_STATE_SHOW 2


#define BRICK_BROKEN_STATUS_NORMAL 100
#define BRICK_BROKEN_STATUS_COIN 200
class CBrickBroken : public CGameObject
{

public:
	DWORD timeout = -1;
	CBrickBroken();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
	virtual void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void BrokenAnimation();
};

