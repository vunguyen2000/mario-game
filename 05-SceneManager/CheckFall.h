#pragma once

#include "GameObject.h"
#include "Animations.h"

#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

class CCheckFall : public CGameObject {
	float x;
	float y;
	bool isOnPlatform;
public:
	CCheckFall();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(int state);
	virtual int  IsCollidable() { return 1; }
	virtual int IsBlocking() { return 0; }
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
};
