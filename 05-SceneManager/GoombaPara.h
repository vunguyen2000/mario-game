#pragma once
#include "GameObject.h"

#define GOOMBA_WALKING_SPEED 0.05f;
#define GOOMBA_GRAVITY	0.002f


#define GOOMBA_JUMP_BBOX_WIDTH 20
#define GOOMBA_JUMP_BBOX_HEIGHT 24
#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 15
#define GOOMBA_BBOX_HEIGHT_DIE 9

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_DIE_DOWN 201

#define GOOMBA_ANI_JUMP 0
#define GOOMBAPARA_ANI_WALKING 1
#define GOOMBAPARA_ANI_DIE 2
#define GOOMBA_LEVEL_JUMP 2
#define GOOMBA_LEVEL_WALKING 1

#define GOOMBA_JUMP	1000
#define GOOMBA_JUMP_SPEED	0.25f
#define GOOMBA_START_X 5


class CGoombaPara : public CGameObject
{
	DWORD checkJump;
	int level;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CGoombaPara();
	void SetLevel(int a) { level = a; };
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	int GetLevel() { return level; };
	virtual void SetState(int state);

};