#include "KoopasPara.h"
#include <algorithm>
#include "PlayScene.h"
#include "Game.h"

CKoopasPara::CKoopasPara()
{
	SetState(KOOPAS_STATE_WALKING);
}

void CKoopasPara::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);

}

void CKoopasPara::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_WALKING)
		bottom = y + KOOPAS_BBOX_HEIGHT;
	if (state == KOOPAS_STATE_HIDE)
	{
		left = top = right = bottom = 0;
	}
}

void CKoopasPara::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if ( state != KOOPAS_STATE_VERTICAL)
		vy += dt * KOOPAS_GRAVITY;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != KOOPAS_STATE_HIDE)
		CalcPotentialCollisions(coObjects, coEvents);

	float tempy = y + dy;
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

		if (state != KOOPAS_STATE_HIDE)
		{
			x += min_tx * dx + nx * 0.4f;
			if (nx < 0)
				y += min_ty * dy + ny * 0.4f;

		}
		if (ny != 0) vy = 0;
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

		}
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		if (x < 0)
		{
			x = 5;
			vx = -vx;
		}
	}

}

void CKoopasPara::Render()
{

	int ids = CGame::GetInstance()->GetCurrentScene()->GetId();
	int ani = KOOPASPARA_ANI_WALKING_LEFT;
	
		 if (state == KOOPAS_STATE_VERTICAL)
		{
			ani = 6;
		}
		 else
		 {
			 if (vx > 0) ani = KOOPASPARA_ANI_WALKING_RIGHT;
			 else if (vx < 0) ani = KOOPASPARA_ANI_WALKING_LEFT;
		 }
	animation_set->at(ani)->Render(x, y);

}
void CKoopasPara::SetState(int state)
{
	{
		LPSCENE scence = CGame::GetInstance()->GetCurrentScene();
		CMario* mario = ((CPlayScene*)scence)->GetPlayer();

		CGameObject::SetState(state);
		switch (state)
		{
		case KOOPAS_STATE_VERTICAL:
			vy = KOOPAS_WALKING_SPEED;
			nx = -1;
			break;
		case KOOPAS_STATE_WALKING:
			if (nx > 0)
			{
				vx = KOOPAS_WALKING_SPEED;

			}
			else
			{
				vx = -KOOPAS_WALKING_SPEED;
			}
			break;
		}
}