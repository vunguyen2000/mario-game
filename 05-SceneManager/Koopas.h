#pragma once
#include "GameObject.h"

#define KOOPAS_WALKING_SPEED 0.02f;
#define KOOPAS_RUN_SPEED 0.3f;
#define KOOPAS_GRAVITY 0.001f;
#define KOOPAS_RUN_SPEED_Y 0.5f;

#define KOOPAS_BBOX_WIDTH 15
#define KOOPAS_BBOX_HEIGHT 26
#define KOOPAS_BBOX_HEIGHT_DIE 15

#define KOOPAS_STATE_WALKING 100

//Trang thai die
#define KOOPAS_STATE_DIE 200
#define KOOPAS_STATE_HOLD 201

//Trang thai die nhung co vx
#define KOOPAS_STATE_THROW 202
#define KOOPAS_STATE_HIDE 203

#define KOOPAS_ANI_WALKING_LEFT 1
#define KOOPAS_ANI_WALKING_RIGHT 0
#define KOOPAS_ANI_DIE 3
#define KOOPAS_ANI_TURN 2



class CKoopas : public CGameObject
{
	float tempbacky;
	bool back = false;
	bool flagBack = false;
	DWORD isRevive;
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CKoopas();
	virtual void SetState(int state);
};

