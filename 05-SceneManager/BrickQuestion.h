#include "GameObject.h"
#include "Mario.h"
#include "Game.h"
#include "PlayScene.h"

#define BRICK_QUESTION_BBOX_WIDTH	16
#define BRICK_QUESTION_BBOX_HEIGHT	16
#define BRICK_QUESTION_STATUS_COIN		1

#define BRICK_QUESTION_STATUS_EFFECT		4
#define BRICK_QUESTION_ANI_BEFORE	0
#define BRICK_QUESTION_ANI_AFTER	1

class CBrickQuestion : public CGameObject
{
	int status;
	bool status_before = true;
	bool status_after = false;

public:
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
};
