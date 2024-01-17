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

#define MARIO_JUMP_SPEED_Y		0.4f
#define MARIO_JUMP_SPEED_Y_HIGH		0.3f

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

#define	MARIO_LEVEL_FOX		4
#define MARIO_ANI_FOX_IDLE_RIGHT		35
#define MARIO_ANI_FOX_IDLE_LEFT		36
#define MARIO_ANI_FOX_WALKING_RIGHT		37
#define MARIO_ANI_FOX_WALKING_LEFT		38
#define MARIO_ANI_FOX_RUN_RIGHT		39
#define MARIO_ANI_FOX_RUN_LEFT		40
#define MARIO_ANI_FOX_JUMP_RIGHT		41
#define MARIO_ANI_FOX_JUMP_LEFT		42
#define MARIO_ANI_FOX_GEARING_RIGHT		43 
#define MARIO_ANI_FOX_GEARING_LEFT		44 
#define MARIO_FOX_BBOX_WIDTH  21
#define MARIO_FOX_BBOX_HEIGHT 27

#define MARIO_ANI_BIG_SIT_RIGHT		46
#define MARIO_ANI_BIG_SIT_LEFT		47
#define MARIO_ANI_FIRE_SIT_RIGHT		49
#define MARIO_ANI_FIRE_SIT_LEFT		48 
#define MARIO_ANI_FOX_SIT_RIGHT		50 
#define MARIO_ANI_FOX_SIT_LEFT		51 
#define MARIO_ANI_FOX_HOLDKOOPAS_RIGHT		64
#define MARIO_ANI_FOX_HOLDKOOPAS_LEFT		65 
#define MARIO_ANI_FOX_HOLDKOOPAS_WALK_RIGHT		66
#define MARIO_ANI_FOX_HOLDKOOPAS_WALK_LEFT		67 
#define MARIO_ANI_FOX_FLY_RIGHT		56
#define MARIO_ANI_FOX_FLY_LEFT		57 
#define MARIO_ANI_FOX_LANDING_RIGHT		58
#define MARIO_ANI_FOX_LANDING_LEFT		59 
#define MARIO_ANI_FOX_ATTACK		45 

#define MARIO_ANI_SMALL_RUN_RIGHT		23
#define MARIO_ANI_SMALL_RUN_LEFT		24
#define MARIO_ANI_SMALL_GEARING_RIGHT		27
#define MARIO_ANI_SMALL_GEARING_LEFT		28
#define MARIO_ANI_SMALL_HOLDKOOPAS_RIGHT		68
#define MARIO_ANI_SMALL_HOLDKOOPAS_LEFT		69 
#define MARIO_ANI_SMALL_HOLDKOOPAS_WALK_RIGHT		70
#define MARIO_ANI_SMALL_HOLDKOOPAS_WALK_LEFT		71 

#define MARIO_STATE_DRAIN_1			503
#define MARIO_STATE_DRAIN_2			504

#define MARIO_ANI_BIG_DRAIN		72
#define MARIO_ANI_FIRE_DRAIN		73 
#define MARIO_ANI_FOX_DRAIN		74 
#define MARIO_ANI_SMALL_DRAIN		75 

#define MARIO_JUMP_DEFLECT_SPEED 0.3f
#define MARIO_STATE_FLY				501
#define MARIO_WALKING_FLY	0.1f 
#define MARIO_STATE_LANDING				502
#define MARIO_WALKING_RUN_MAX		0.25f 
#define MARIO_TIME_ATTACK	400

#define TIME_FLY	3000

#define MARIO_FLY_LEVEL_SPEED 200
#define MARIO_FLY_LEVEL 5
#define LANDING_LIMIT 0.05
#define LANDING_LIMIT_SPEED 0.04
#define MARIO_UNTOUCHABLE_TIME 3000

#define MARIO_TIME_DRAIN 1000
#define MARIO_TIME_DRAIN_1_SPEED	0.05f
#define MARIO_TIME_DRAIN_2_SPEED	0.05f
#define MARIO_TIME_DRAIN_1_X 2103
#define MARIO_TIME_DRAIN_1_Y 270
#define MARIO_TIME_DRAIN_1_Y_DEFAULT 298

#define MARIO_TIME_DRAIN_2_X 2330
#define MARIO_TIME_DRAIN_2_Y 118
#define MARIO_TIME_DRAIN_2_Y_DEFAULT 90

class CMario : public CGameObject
{
	int level;
	float start_x;			
	float start_y;
	float camX_update = 0;
	DWORD timeDrain = 0;
	DWORD untouchable_start;
	DWORD timeReset;

public:
	int checkjumping = 0;
	int untouchable;
	bool checkidle = true;
	DWORD timeLimitJump;
	bool checkFree = false;
	bool sit = false;
	bool holdKoopas = false;
	bool holdKoopasCol = false;	
	int startRun = 0;	
	int stopRun = 0;	
	bool flyCan = false;	//trạng thái bay
	int timeFly = 0;	//kiểm tra thời gian bay
	bool landingCheck = false;	//kiểm tra hạ cánh
	int levelFly = 0;
	DWORD attackCheck = 0;
	bool attack = false;	//trạng thái tấn công
	int card = 0;
	bool checkEnd = false;
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
	int GetSpeed()
	{
		return levelFly;
	};
};