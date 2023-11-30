#pragma once
#include "GameObject.h"
#define FIRE_FLOWER_BBOX_WIDTH  8
#define FIRE_FLOWER_BBOX_HEIGHT 9

#define FIRE_FLOWER_STATE_HIDE 111
#define FIRE_FLOWER_STATE_ATTACK	222

#define FIRE_FLOWER_X_DEFAULT 70
#define FIRE_FLOWER_SPEED 0.04f

#define FIRE_FLOWER_ANI_ATTACK 0
class CFireFlower : public CGameObject
{
	bool checkAppear;

public:
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);

	CFireFlower()
	{
		SetState(FIRE_FLOWER_STATE_HIDE);
	}
	bool GetAppear()
	{
		return checkAppear;
	}
	void SetAppear(bool temp)
	{
		checkAppear = temp;
	}
};

