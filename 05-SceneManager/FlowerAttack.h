#pragma once
#include "GameObject.h"
#define FLOWER_RED_BBOX_WIDTH  16
#define FLOWER_RED_BBOX_HEIGHT 32

#define FLOWER_GREEN_BBOX_WIDTH  16
#define FLOWER_GREEN_BBOX_HEIGHT 24

#define FLOWER_RED_ANI_LEFT_DOWN  0
#define FLOWER_RED_ANI_LEFT_UP 1
#define FLOWER_RED_ANI_RIGHT_DOWN  2
#define FLOWER_RED_ANI_RIGHT_UP 3
#define FLOWER_GREEN_ANI_LEFT_DOWN  4
#define FLOWER_GREEN_ANI_LEFT_UP 5
#define FLOWER_GREEN_ANI_RIGHT_DOWN  6
#define FLOWER_GREEN_ANI_RIGHT_UP 7
#define FLOWER_GREEN_ANI_UP 8

#define FLOWER_RED				100
#define FLOWER_GREEN			200
#define FLOWER_NORMAL  300 
class CFlowerAttack : public CGameObject
{
	int status;
	int shootY = 1;
	DWORD isTime = 0;
	DWORD isTimeAgain = 1;
	bool Up = true;
	bool shooting = false;
	bool shooted = false;
public:
	CFlowerAttack(int status) { this->status = status; };
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void Appear()
	{
		isTime = GetTickCount();
	}

};

