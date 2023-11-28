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
#define MARIO_GRAVITY			0.002f

#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_WALKING_SPEED		0.15f 
#define MARIO_WALKING_SPEED_ENDMAP		0.08f 
#define MARIO_WALKING_SPEED_PLUS		0.002f 
#define MARIO_WALKING_SPEED_PLUS1		0.1f 

class CMario : public CGameObject
{
	int level;
	int untouchable;
	float start_x;			
	float start_y;
	bool checkidle = true;	//ki?m tra tr?ng th�i ??ng im

public:
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx, float& min_ty,
		float& nx, float& ny, float& rdx, float& rdy);

	void SetLevel(int l);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};