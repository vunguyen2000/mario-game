#pragma once
#include "GameObject.h"
#define MUSHROOM_BBOX_WIDTH  16
#define MUSHROOM_BBOX_HEIGHT  16

#define MUSHROOM_ANI_RIGHT  0
#define MUSHROOM_ANI_LEFT  1  

#define MUSHROOM_GRAVITY 0.0002f
#define MUSHROOM_SPEED 0.04f

#define BRICK_QUESTION_LEAF	1.5
class CMushRoom : public CGameObject
{
	int status;

public:
	bool green = false;
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

