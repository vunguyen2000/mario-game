#pragma once
#include "GameObject.h"

#define EFFECT_P_BBOX_WIDTH  16
#define EFFECT_P_BBOX_HEIGHT  16
#define EFFECT_P_AFTER_BBOX_WIDTH  16
#define EFFECT_P_AFTER_BBOX_HEIGHT  7

#define EFFECT_P_STATE_BEFORE  0
#define EFFECT_P_STATE_AFTER  1

class CEffect : public CGameObject
{
public:
	CEffect();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
	int GetState() { return state; };
};

