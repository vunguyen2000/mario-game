#pragma once
#include "GameObject.h"
#include "Animations.h"
#include "debug.h"

#define MARIO_STATE_IDLE			0
#define	MARIO_LEVEL_SMALL	1
#define MARIO_STATE_DIE				400
#define MARIO_ANI_DIE				8
#define MARIO_ANI_SMALL_IDLE_RIGHT		2
#define MARIO_ANI_SMALL_IDLE_LEFT			3
#define MARIO_ANI_SMALL_WALKING_RIGHT		6
#define MARIO_ANI_SMALL_WALKING_LEFT		7
#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15
#define MARIO_ANI_SMALL_FLY_RIGHT		25
#define MARIO_ANI_SMALL_FLY_LEFT		26
#define MARIO_GRAVITY			0.002f
#define MARIO_GEARING_MAX	0.051f
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_WALKING_SPEED		0.15f 
#define MARIO_WALKING_SPEED_ENDMAP		0.08f 
#define MARIO_WALKING_SPEED_PLUS		0.002f 
#define MARIO_WALKING_SPEED_PLUS1		0.1f 

#define MARIO_DIE_DEFLECT_SPEED	 0.5f
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_JUMP_HIGH			310

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_SPEED_Y_HIGH		0.625f

#define	MARIO_LEVEL_BIG		2

#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_ANI_BIG_IDLE_RIGHT		0
#define MARIO_ANI_BIG_IDLE_LEFT			1
#define MARIO_ANI_SMALL_IDLE_RIGHT		2
#define MARIO_ANI_SMALL_IDLE_LEFT			3
#define MARIO_ANI_FIRE_IDLE_RIGHT		9
#define MARIO_ANI_FIRE_IDLE_LEFT			10

#define MARIO_ANI_BIG_WALKING_RIGHT			4
#define MARIO_ANI_BIG_WALKING_LEFT			5

#define MARIO_ANI_BIG_RUN_RIGHT		17
#define MARIO_ANI_BIG_RUN_LEFT		18
#define MARIO_STATE_WALKING_RIGHT_FAST	110
#define MARIO_STATE_WALKING_LEFT_FAST	210

#define MARIO_ANI_BIG_GEARING_RIGHT		22 //LẤY ĐÀ PHẢI
#define MARIO_ANI_BIG_GEARING_LEFT		21 //LẤY ĐÀ TRÁI

#define MARIO_ANI_BIG_SIT_RIGHT		46
#define MARIO_ANI_BIG_SIT_LEFT		47
#define MARIO_SIT_BBOX_HEIGHT 18

#define MARIO_UNTOUCHABLE_TIME 3000
class CMario : public CGameObject
{
	int level;
	int untouchable;
	float start_x;			
	float start_y;
	float camX_update = 0;
	DWORD untouchable_start;
	DWORD timeReset;

public:
	int checkjumping = 0;
	bool checkidle = true;
	bool checkFree = false;
	bool sit = false;
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void Reset();
	virtual void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx, float& min_ty,
		float& nx, float& ny, float& rdx, float& rdy);
	int GetLevel() { return level; };
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);

	void SetLevel(int l) { level = l; }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
};