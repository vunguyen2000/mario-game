#include "GameObject.h"
#include "Mario.h"
#include "Game.h"
#include "PlayScene.h"

#define BRICK_QUESTION_BBOX_WIDTH	16
#define BRICK_QUESTION_BBOX_HEIGHT	16


#define BRICK_QUESTION_STATUS_EFFECT		4
#define BRICK_QUESTION_ANI_BEFORE	0
#define BRICK_QUESTION_ANI_AFTER	1

#define BRICK_QUESTION_COUNT_Y	3
#define BRICK_QUESTION_COUNT_TIME	2

#define BRICK_QUESTION_STATE_AFTER	123
#define BRICK_QUESTION_ANI_COIN	7001

#define BRICK_QUESTION_ANI_LEAF	6024
#define BRICK_MUSHROOM_GREEN	0
#define BRICK_QUESTION_MUSHROOM	1.2
#define BRICK_QUESTION_ANI_MUSHROOM	6022

#define BRICK_QUESTION_STATUS_SPECIAL		0
#define BRICK_QUESTION_STATUS_COIN		1
#define BRICK_QUESTION_STATUS_MUSHROOM		2
#define BRICK_QUESTION_STATUS_LEAF		3


class CBrickQuestion : public CGameObject
{
	int status;
	int timeAni = 0;
	bool status_before = true;
	bool status_after = false;
	bool check = false;

public:
	bool isJump = true;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	CBrickQuestion(int status);
	virtual void SetState(int state);
	int GetStatus()
	{
		return status;
	}
	void SetStatus(int status)
	{
		this->status = status;
	}
	bool GetBefore()
	{
		return status_before;
	}
	void SetBefore(bool status)
	{
		status_before = status;
	}
	bool GetAfter()
	{
		return status_after;
	}
	void SetAfter(bool status)
	{
		status_after = status;
	}
	void SetUp(bool status)
	{
		check = status;
	}
	bool GetUp()
	{
		return check;
	}
};
