#pragma once
#include "GameObject.h"

#define KOOPAS_GRAVITY 0.002f;
#define KOOPAS_WALKING_SPEED 0.03f;
#define KOOPAS_JUMP_SPEED_Y 0.25f;
#define KOOPAS_BBOX_WIDTH 15
#define KOOPAS_BBOX_HEIGHT 26
#define KOOPAS_BBOX_HEIGHT_DIE 15
#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_HIDE 203
#define KOOPAS_STATE_JUMP 204
#define KOOPAS_STATE_VERTICAL 205

#define KOOPASPARA_ANI_WALKING_LEFT 3
#define KOOPASPARA_ANI_WALKING_RIGHT 2
#define KOOPASPARA_ANI_JUMP_LEFT 1
#define KOOPASPARA_ANI_JUMP_RIGHT 0
#define KOOPASPARA_ANI_TURN 4


#define KOOPAS_TIME_JUMP 2000
class CKoopasPara :public CGameObject
{
	DWORD checkJump;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CKoopasPara();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	virtual void SetState(int state);
};

