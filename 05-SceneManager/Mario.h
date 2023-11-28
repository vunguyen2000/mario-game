#pragma once
#include "GameObject.h"
#include "Animations.h"
#include "debug.h"


class CMario : public CGameObject
{

public:
	CMario();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);

	void SetLevel(int l);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};