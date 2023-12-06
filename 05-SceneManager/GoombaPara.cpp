#include "GoombaPara.h"
#include "Brick.h"
#include "Goomba.h"
#include <algorithm>
#include "Mario.h"
#include "PlayScene.h"
CGoombaPara::CGoombaPara()
{
	SetState(GOOMBA_STATE_WALKING);
	level = GOOMBA_LEVEL_JUMP;
	checkJump = GetTickCount();
}
void CGoombaPara::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (dynamic_cast<CGoomba*>(coObjects->at(i)))
		{
			continue;
		}

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);

}

void CGoombaPara::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (level == GOOMBA_LEVEL_WALKING)
	{
		left = x;
		top = y;
		right = x + GOOMBA_BBOX_WIDTH;
		bottom = y + GOOMBA_BBOX_HEIGHT;
	}
	if (state == GOOMBA_STATE_DIE)
	{
		right = left = bottom = top = 0;
	}
	if (level == GOOMBA_LEVEL_JUMP)
	{
		left = x;
		top = y;
		right = x + GOOMBA_JUMP_BBOX_WIDTH;
		bottom = y + GOOMBA_JUMP_BBOX_HEIGHT;
	}

}

void CGoombaPara::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (GetTickCount() - checkJump >= GOOMBA_JUMP && level == GOOMBA_LEVEL_JUMP)
	{
		vy = -GOOMBA_JUMP_SPEED;
		checkJump = GetTickCount();
	}

	CGameObject::Update(dt);

	vy += GOOMBA_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != GOOMBA_STATE_DIE_DOWN && state != GOOMBA_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);


	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{

		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);


		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (ny != 0) vy = 0;
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (nx != 0)
			{
				vx = -vx;
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	if (x < 0)
	{
		x = GOOMBA_START_X;
		vx = -vx;
	}
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float limitRight = mario->x + 80;
	float limitLeft = mario->x - 80;
	if(x> limitRight) vx = -GOOMBA_WALKING_SPEED
	if(x< limitLeft) vx = GOOMBA_WALKING_SPEED
	
}

void CGoombaPara::Render()
{
	int ani = -1;
	if (level == GOOMBA_LEVEL_JUMP)
	{
		ani = GOOMBA_ANI_JUMP;
	}else
	ani = GOOMBAPARA_ANI_WALKING;
	if (state == GOOMBA_STATE_DIE) {
		ani = GOOMBAPARA_ANI_DIE;
	}
	animation_set->at(ani)->Render(x, y);

}

void CGoombaPara::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
		break;
	case GOOMBA_STATE_DIE:
		vx = 0;
		vy = -0.2f;
		break;
	}
}
