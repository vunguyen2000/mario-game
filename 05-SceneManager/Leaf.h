#pragma once
#include "GameObject.h"
#define LEAF_BBOX_WIDTH  16
#define LEAF_BBOX_HEIGHT  14

#define LEAF_ANI_RIGHT  0
#define LEAF_ANI_LEFT  1

#define LEAF_GRAVITY 0.00001f
class CLeaf : public CGameObject
{
	int status;

public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

